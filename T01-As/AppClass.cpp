#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("MIDTERM DEMO"); // Window Name
	m_pSystem->SetWindowWidth(720); //Set window dimensions
	m_pSystem->SetWindowHeight(720);
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);//Set clear color
}

void AppClass::InitVariables(void)
{
	//Set the camera position
	m_pCameraMngr->SetPosition(vector3(0.0f, 0.0f, 18.0f));
	//Load the model
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
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

	//Calculate delta and total times
	static double dTotalTime = 0.0; //Total time of the simulation
	double dDeltaTime = m_pSystem->LapClock(); //time difference between function calls
	dTotalTime += dDeltaTime; //Incrementing the time differences 

	float fStartTime = 0.0f;
	float fEndTime = 5.0f;
	float fAngle = MapValue(static_cast<float>(dTotalTime), fStartTime, fEndTime, 0.0f, 360.0f);
	m_m4Steve = glm::rotate(IDENTITY_M4, fAngle, REAXISZ);

	static float fStartHeight = 0.0f;
	static float fEndHeight = 5.0f;
	float fPercent = MapValue(static_cast<float>(dTotalTime), fStartTime, fEndTime, 0.0f, 1.0f);
	float fHeight = glm::lerp(fStartHeight, fEndHeight, fPercent);
	m_m4Steve = m_m4Steve * glm::translate(vector3(0.0f, fHeight, 0.0f));
	if (fPercent > 1.0f)
	{
		std::swap(fStartHeight, fEndHeight);
		dTotalTime = 0.0;
	}

	//Set the model matrix
	m_pMeshMngr->SetModelMatrix(m_m4Steve, "Steve");
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("Seconds:");
	m_pMeshMngr->PrintLine(std::to_string(dTotalTime), RERED);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ResetRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}