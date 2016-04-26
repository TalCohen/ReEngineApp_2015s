#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Camera Singleton"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCamera = MyCameraSingleton::GetInstance();
	m_pCone = new PrimitiveClass();
	m_pCone->GenerateCone(1.0f, 1.0f, 20, RERED);
}

void AppClass::CameraRotation(float a_fSpeed)
{
	UINT	MouseX, MouseY;		// Coordinates for the mouse
	UINT	CenterX, CenterY;	// Coordinates for the center of the screen.

								//Initialize the position of the pointer to the middle of the screen
	CenterX = m_pSystem->GetWindowX() + m_pSystem->GetWindowWidth() / 2;
	CenterY = m_pSystem->GetWindowY() + m_pSystem->GetWindowHeight() / 2;

	//Calculate the position of the mouse and store it
	POINT pt;
	GetCursorPos(&pt);
	MouseX = pt.x;
	MouseY = pt.y;

	//Calculate the difference in view with the angle
	float fAngleX = 0.0f;
	float fAngleY = 0.0f;
	float fDeltaMouse = 0.0f;
	if (MouseX < CenterX)
	{
		fDeltaMouse = static_cast<float>(CenterX - MouseX);
		fAngleY += fDeltaMouse * a_fSpeed;
	}
	else if (MouseX > CenterX)
	{
		fDeltaMouse = static_cast<float>(MouseX - CenterX);
		fAngleY -= fDeltaMouse * a_fSpeed;
	}

	if (MouseY < CenterY)
	{
		fDeltaMouse = static_cast<float>(CenterY - MouseY);
		fAngleX -= fDeltaMouse * a_fSpeed;
	}
	else if (MouseY > CenterY)
	{
		fDeltaMouse = static_cast<float>(MouseY - CenterY);
		fAngleX += fDeltaMouse * a_fSpeed;
	}
	//Change the Yaw and the Pitch of the camera
	m_pCamera->ChangeYaw(fAngleY * 3.0f);
	m_pCamera->ChangePitch(-fAngleX * 3.0f);
	SetCursorPos(CenterX, CenterY);//Position the mouse in the center
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Call the arcball method
	ArcBall();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	
	m_pCone->Render(m_pCamera->GetProjectionMatrix(), m_pCamera->GetViewMatrix(), ToMatrix4(m_qArcBall));
	m_pMeshMngr->Render(); //Render the list of objects
	m_pMeshMngr->ResetRenderList(); //Reset the render list
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	m_pCamera->ReleaseInstance();
	SafeDelete(m_pCone);
	super::Release(); //release the memory of the inherited fields
}