#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("MyEntityClass"); // Window Name
	//m_pSystem->SetWindowResolution(RESOLUTIONS::HD_1280X720);
	//m_pSystem->SetWindowFullscreen(); //Sets the window to be fullscreen
	//m_pSystem->SetWindowBorderless(true); //Sets the window to not have borders
	m_v4ClearColor = vector4(REBLACK, 1.0f);
}

void AppClass::InitVariables(void)
{
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 2.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What Im looking at
		REAXISY);//What is up
	m_pCameraMngr->SetCameraMode(CAMERAMODE::CAMROTHOZ);

	//Instance objects to display the game elements
	m_pMeshMngr->InstanceSphere(0.3f, 5, RERED, "Ball");

	m_pMeshMngr->InstanceCuboid(vector3(20, 1, 1), REPURPLE, "BoxT");
	m_pMeshMngr->InstanceCuboid(vector3(20, 1, 1), REPURPLE, "BoxB");
	m_pMeshMngr->InstanceCuboid(vector3(1, 11, 1), REGREEN, "BoxL");
	m_pMeshMngr->InstanceCuboid(vector3(1, 11, 1), REGREEN, "BoxR");
	
	m_pMeshMngr->InstanceCuboid(vector3(0.5, 3, 0.5), REBLUE, "PalletL");
	m_pMeshMngr->InstanceCuboid(vector3(0.5, 3, 0.5), REBLUE, "PalletR");

	//Create game objects based on the loaded objects
	m_pBall = new MyEntityClass("Ball");
	
	m_pBoxT = new MyEntityClass("BoxT");
	m_pBoxB = new MyEntityClass("BoxB");
	m_pBoxL = new MyEntityClass("BoxL");
	m_pBoxR = new MyEntityClass("BoxR");

	m_pPalletL = new MyEntityClass("PalletL");
	m_pPalletR = new MyEntityClass("PalletR");

	//Set properties of the objects
	m_pBall->SetVelocity(vector3(0.11f, 0.11f, 0.0f));

	m_pBoxT->SetModelMatrix(glm::translate(vector3(0, 5, 0)));
	m_pBoxB->SetModelMatrix(glm::translate(vector3(0, -5, 0)));
	m_pBoxR->SetModelMatrix(glm::translate(vector3(10.5, 0, 0)));
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
	
	//Set the model matrix for the first model to be the arcball
	m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qArcBall), "All");

	m_pBall->Update();
	if (m_pBall->IsColliding(m_pBoxT))
	{
		vector3 v3Velocity = m_pBall->GetVelocity();
		v3Velocity.y *= -1;
		m_pBall->SetVelocity(v3Velocity);
	}
	if (m_pBall->IsColliding(m_pBoxR))
	{
		vector3 v3Velocity = m_pBall->GetVelocity();
		v3Velocity.x *= -1;
		m_pBall->SetVelocity(v3Velocity);
	}

	//Add objects to the render list
	m_pBall->AddToRenderList(true);
	m_pBoxT->AddToRenderList(true);
	m_pBoxB->AddToRenderList(true);
	m_pBoxL->AddToRenderList(true);
	m_pBoxR->AddToRenderList(true);
	m_pPalletL->AddToRenderList(true);
	m_pPalletR->AddToRenderList(true);

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);
	
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
	SafeDelete(m_pBall);
	SafeDelete(m_pBoxT);
	SafeDelete(m_pBoxB);
	SafeDelete(m_pBoxL);
	SafeDelete(m_pBoxR);
	SafeDelete(m_pPalletL);
	SafeDelete(m_pPalletR);

	super::Release(); //release the memory of the inherited fields
}