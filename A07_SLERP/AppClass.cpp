#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - Tal Cohen"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
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

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	//Earth Orbit
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route

	matrix4 m4Sun = IDENTITY_M4;
	matrix4 m4Earth = IDENTITY_M4;
	matrix4 m4Moon = IDENTITY_M4;

	//These matrices will hold the relative transformation of the Moon and the Earth
	matrix4 distanceEarth = glm::translate(11.0f, 0.0f, 0.0f);
	matrix4 distanceMoon = glm::translate(2.0f, 0.0f, 0.0f);

	// Get the start and end quats
	glm::quat start = glm::quat(vector3(0.0f, 0.0f, 0.0f));
	glm::quat end = glm::quat(vector3(0.0f, PI, 0.0f));

	// Find the earth orbit and moon orbit quats
	glm::quat earthOrb = glm::mix(start, end, (float)(fRunTime / fEarthHalfOrbTime));
	glm::quat moonOrb = glm::mix(start, end, (float)(fRunTime / fMoonHalfOrbTime));

	// Rotate Earth
	m4Earth *= glm::mat4_cast(earthOrb) * distanceEarth;

	// Rotate Moon
	m4Moon *= m4Earth * glm::mat4_cast(moonOrb) * distanceMoon;
	

	// Find earth revolve quat
	glm::quat earthRev = glm::mix(start, glm::quat(vector3(0.0f, 365*(PI/360), 0.0f)), (float)(fRunTime / fEarthHalfRevTime));

	// Revolve earth
	m4Earth *= glm::mat4_cast(earthRev);

	// Scale all of them
	float sunScale = 5.936f;
	float earthScale = 0.524f;
	float moonScale = 0.27f;
	m4Sun *= glm::scale(sunScale, sunScale, sunScale);
	m4Earth *= glm::scale(earthScale, earthScale, earthScale);
	m4Moon *= glm::scale(earthScale * moonScale, earthScale * moonScale, earthScale * moonScale);

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m4Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m4Moon, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	static int nEarthOrbits = 0;
	nEarthOrbits = fRunTime / (fEarthHalfOrbTime * 2);

	static int nEarthRevolutions = 0;
	nEarthRevolutions = fRunTime / (fEarthHalfRevTime * 2);

	static int nMoonOrbits = 0;
	nMoonOrbits = fRunTime / (fMoonHalfOrbTime * 2);

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

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
	default:
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToRenderList(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}