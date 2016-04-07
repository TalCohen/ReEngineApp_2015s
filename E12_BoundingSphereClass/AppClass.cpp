#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("MyBoundingSphereClass example"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	//Initialize positions
	m_v3O1 = vector3(-2.5f, 0.0f, 0.0f);
	m_v3O2 = vector3(2.5f, 0.0f, 0.0f);

	//Load Models
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper");

	std::vector<vector3> vertexList = m_pMeshMngr->GetVertexList("Steve");

	vector3 v3Min, v3Max;
	if (vertexList.size() > 0)
	{
		v3Min = vertexList.at(0);
		v3Max = vertexList.at(0);
	}

	for (uint i = 0; i < vertexList.size(); i++)
	{
		vector3 vertex = vertexList.at(i);
		if (vertex.x < v3Min.x)
		{
			v3Min.x = vertex.x;
		}
		else if (vertex.x > v3Max.x)
		{
			v3Max.x = vertex.x;
		}

		if (vertex.y < v3Min.y)
		{
			v3Min.y = vertex.y;
		}
		else if (vertex.y > v3Max.y)
		{
			v3Max.y = vertex.y;
		}

		if (vertex.z < v3Min.z)
		{
			v3Min.z = vertex.z;
		}
		else if (vertex.z > v3Max.z)
		{
			v3Max.z = vertex.z;
		}
	}

	m_v3Center1 = (v3Max + v3Min) / 2.0f;
	m_fRadius1 = glm::distance(m_v3Center1, v3Min);

	m_pSphere1 = new PrimitiveClass();
	m_pSphere1->GenerateSphere(m_fRadius1, 10, REGREEN);


	vertexList.clear();
	vertexList = m_pMeshMngr->GetVertexList("Creeper");

	v3Min = vector3(0.0f);
	v3Max = vector3(0.0f);

	// CREEPER
	if (vertexList.size() > 0)
	{
		v3Min = vertexList.at(0);
		v3Max = vertexList.at(0);
	}

	for (uint i = 0; i < vertexList.size(); i++)
	{
		vector3 vertex = vertexList.at(i);
		if (vertex.x < v3Min.x)
		{
			v3Min.x = vertex.x;
		}
		else if (vertex.x > v3Max.x)
		{
			v3Max.x = vertex.x;
		}

		if (vertex.y < v3Min.y)
		{
			v3Min.y = vertex.y;
		}
		else if (vertex.y > v3Max.y)
		{
			v3Max.y = vertex.y;
		}

		if (vertex.z < v3Min.z)
		{
			v3Min.z = vertex.z;
		}
		else if (vertex.z > v3Max.z)
		{
			v3Max.z = vertex.z;
		}
	}

	m_v3Center2 = (v3Max + v3Min) / 2.0f;
	m_fRadius2 = glm::distance(m_v3Center2, v3Min);

	m_pSphere2 = new PrimitiveClass();
	m_pSphere2->GenerateSphere(m_fRadius2, 10, REGREEN);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	ArcBall();

	//Set the model matrices for both objects and Bounding Spheres
	m_pMeshMngr->SetModelMatrix(glm::translate(m_v3O1) * ToMatrix4(m_qArcBall), "Steve");
	m_pMeshMngr->SetModelMatrix(glm::translate(m_v3O2), "Creeper");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	bool areColliding = false;
	m_pMeshMngr->Print("x: " + std::to_string(m_v3Center1.x) + " ", RERED);
	m_pMeshMngr->Print("y: " + std::to_string(m_v3Center1.y) + " ", RERED);
	m_pMeshMngr->Print("z: " + std::to_string(m_v3Center1.z) + " ", RERED);

	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Model = m_pMeshMngr->GetModelMatrix("Steve") * glm::translate(m_v3Center1);

	m_pSphere1->Render(m4Projection, m4View, m4Model);



	m4Projection = m_pCameraMngr->GetProjectionMatrix();
	m4View = m_pCameraMngr->GetViewMatrix();
	m4Model = m_pMeshMngr->GetModelMatrix("Creeper") * glm::translate(m_v3Center2);

	m_pSphere1->Render(m4Projection, m4View, m4Model);
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields

	if (m_pSphere1 != nullptr)
	{
		delete m_pSphere1;
		m_pSphere1 = nullptr;
	}

	if (m_pSphere2 != nullptr)
	{
		delete m_pSphere2;
		m_pSphere2 = nullptr;
	}
}