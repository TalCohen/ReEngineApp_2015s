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
	//Load Models
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
	m_pMeshMngr->LoadModel("Minecraft\\Creeper.obj", "Creeper");

	std::vector<vector3> vertexList = m_pMeshMngr->GetVertexList("Steve");
	m_pSphere1 = new MyBoundingSphereClass(vertexList);

	//Creeper
	vertexList = m_pMeshMngr->GetVertexList("Creeper");
	m_pSphere2 = new MyBoundingSphereClass(vertexList);

	//Initialize positions
	m_pSphere1->UpdatePosition(vector3(-2.5f, 0.0f, 0.0f));
	m_pSphere2->UpdatePosition(vector3(2.5f, 0.0f, 0.0f));

	// Set model matrices
	m_pSphere1->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Steve"));
	m_pSphere2->SetModelMatrix(m_pMeshMngr->GetModelMatrix("Creeper"));
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
	m_pMeshMngr->SetModelMatrix(glm::translate(m_pSphere1->GetPosition()) * ToMatrix4(m_qArcBall), "Steve");
	m_pMeshMngr->SetModelMatrix(glm::translate(m_pSphere2->GetPosition()), "Creeper");

	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Collision check goes here
	vector3 v3Temp = vector3(m_m4Steve * vector4( m_pSphere1->GetCenter(), 1.0f));
	//vector3 v3Temp1 = vector3(m_m4Creeper * vector4(m_pSphere2->GetCenter(), 1.0f));
	bool bAreColliding = m_pSphere1->IsColliding(m_pSphere2);

	m_m4Steve = m_pMeshMngr->GetModelMatrix("Steve") * glm::translate(m_pSphere1->GetCenter());
	if(bAreColliding)
		m_pMeshMngr->AddSphereToQueue(m_m4Steve * glm::scale(vector3(m_pSphere1->GetRadius() * 2.0f)), RERED, WIRE);
	else
		m_pMeshMngr->AddSphereToQueue(m_m4Steve * glm::scale(vector3(m_pSphere1->GetRadius() * 2.0f)), REWHITE, WIRE);

	m_m4Creeper = m_pMeshMngr->GetModelMatrix("Creeper") * glm::translate(m_pSphere2->GetCenter());
	if (bAreColliding)
		m_pMeshMngr->AddSphereToQueue(m_m4Creeper * glm::scale(vector3(m_pSphere2->GetRadius() * 2.0f)), RERED, WIRE);
	else
		m_pMeshMngr->AddSphereToQueue(m_m4Creeper * glm::scale(vector3(m_pSphere2->GetRadius() * 2.0f)), REWHITE, WIRE);

	m_pMeshMngr->Print("x:" + std::to_string(v3Temp.x ) + " ", RERED);
	m_pMeshMngr->Print("y:" + std::to_string(v3Temp.y) + " ", RERED);
	m_pMeshMngr->Print("z:" + std::to_string(v3Temp.z) + " ", RERED);
	m_pMeshMngr->PrintLine("");

	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	if (bAreColliding)
		m_pMeshMngr->PrintLine("They are colliding! >_<", RERED);
	else
		m_pMeshMngr->PrintLine("They are not colliding! =)", REGREEN);
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
	matrix4 m4Model = m_pMeshMngr->GetModelMatrix("Steve") * glm::translate(m_pSphere1->GetCenter());

	m_pSphere1->Render(m4Projection, m4View, m4Model);



	m4Projection = m_pCameraMngr->GetProjectionMatrix();
	m4View = m_pCameraMngr->GetViewMatrix();
	m4Model = m_pMeshMngr->GetModelMatrix("Creeper") * glm::translate(m_pSphere2->GetCenter());

	m_pSphere1->Render(m4Projection, m4View, m4Model);
	
	

	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
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