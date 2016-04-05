#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Sandbox"); // Window Name
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	//Set the camera at a position other than the default
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 2.5f, 12.0f), vector3(0.0f, 2.5f, 11.0f), REAXISY);

	//Load a model onto the Mesh manager
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
}

matrix4 CalculateMatrix(matrix4 m4Input)
{
	matrix4 m4Output = glm::inverse(m4Input);
	return m4Input * m4Output;
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

	//Call the arcball method
	ArcBall();

	//matrix4 m4Example = glm::translate(vector3(1.0, 0.0, 0.0));
	//m4Example = CalculateMatrix(m4Example);

	//Lets us know how much time has passed since the last call
	double fDeltaTime = m_pSystem->LapClock();

	//cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fDeltaTime;

	float fPercerntage;

	fPercerntage = MapValue(fRunTime, 0.0, 50.0, 0.0, 1.0);

	float fPosition = glm::lerp(-5.0f, 5.0f, fPercerntage);

	matrix4 m4Example = glm::translate(vector3(fPosition, 0.0, 0.0));

	m_pMeshMngr->SetModelMatrix(m4Example, "Steve");

	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
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
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}