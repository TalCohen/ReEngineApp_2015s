#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;

	m_numPositions = 11;
	m_currentPositionIndex = 0;
	m_positions = new vector3[m_numPositions]
	{
		vector3(1.0f,-2.0f, 5.0f),
		vector3(-3.0f,-1.0f, 3.0f),
		vector3(2.0f,-1.0f, 3.0f),
		vector3(-2.0f, 0.0f, 0.0f),
		vector3(3.0f, 0.0f, 0.0f),
		vector3(-1.0f, 1.0f,-3.0f),
		vector3(4.0f, 1.0f,-3.0f),
		vector3(0.0f, 2.0f,-5.0f),
		vector3(5.0f, 2.0f,-5.0f),
		vector3(1.0f, 3.0f,-5.0f),
		vector3(-4.0f,-2.0f, 5.0f)
	};
	m_startPosition = m_positions[m_numPositions - 1];
	m_endPosition = m_positions[m_currentPositionIndex];

	m_spheres = new PrimitiveClass[m_numPositions];
	for (unsigned int i = 0; i < m_numPositions; i++)
	{
		m_spheres[i].GenerateSphere(0.1f, 5, RERED);
	}
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	int secondsPassed = static_cast<int>(fRunTime / fDuration);
	int durationsPassed = secondsPassed % m_numPositions;
	if (durationsPassed != m_currentPositionIndex)
	{
		// A new second has passed!
		m_currentPositionIndex = durationsPassed;
		m_startPosition = m_endPosition;
		m_endPosition = m_positions[m_currentPositionIndex];
		std::cout << m_currentPositionIndex << std::endl;
		std::cout << fRunTime - durationsPassed << std::endl;
	}

	vector3 pos = glm::lerp(m_startPosition, m_endPosition, static_cast<float>(fRunTime - secondsPassed));

	m_pMeshMngr->SetModelMatrix(glm::translate(pos), "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
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

	matrix4 projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 view = m_pCameraMngr->GetViewMatrix();

	for (unsigned int i = 0; i < m_numPositions; i++)
	{
		m_spheres[i].Render(projection, view, glm::translate(m_positions[i]));
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	if (m_positions != nullptr)
	{
		delete[] m_positions;
		m_positions = nullptr;
	}

	if (m_spheres != nullptr)
	{
		delete[] m_spheres;
		m_spheres = nullptr;

	}

	super::Release(); //release the memory of the inherited fields
}