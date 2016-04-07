#include "AppClass.h"

#define DEPTH 5

void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("Instance Rendering");
}

void AppClass::InitVariables(void)
{
	m_pMesh = new MyMesh();

	//Creating the Mesh points
	m_pMesh->AddVertexPosition(vector3(-1.0f, 0.0f, 0.0f));
	m_pMesh->AddVertexColor(REGREEN);
	m_pMesh->AddVertexPosition(vector3(1.0f, 0.0f, 0.0f));
	m_pMesh->AddVertexColor(RERED);
	m_pMesh->AddVertexPosition(vector3(0.0f, sqrt(3), 0.0f));
	m_pMesh->AddVertexColor(REBLUE);

	//Compiling the mesh
	m_pMesh->CompileOpenGL3X();

	int numRows = pow(2, (DEPTH-1));

	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, -numRows/2, 10.0f * DEPTH), vector3(0.0f, -numRows/2, 0.0f), REAXISY);

	//Initializing the array
	m_nObjects = Sum(numRows);

	// We need 16 floats for each object (each matrix has 16 floats)
	m_fMatrixArray = new float[m_nObjects * 16];

	CalculatePascal(numRows);
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Is the arcball active?
	if (m_bArcBall == true)
		ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
										  //Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window
	
	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	m_pMesh->RenderList(m4Projection, m4View, m_fMatrixArray, m_nObjects);//Rendering nObjects

	m_pMeshMngr->Render();

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	super::Release();
}

void AppClass::CalculatePascal(int numRows)
{
	int count = 0;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j <= i; j++)
		{
			long long n = Factorial(i) / (Factorial(j) * Factorial(i - j));
			if (n % 2 == 1)
			{
				// If it's odd, we can draw it.
				const float* m4MVP = glm::value_ptr(glm::translate(vector3((j * 2) - i, -i*sqrt(3), 0)));
				memcpy(&m_fMatrixArray[count * 16], m4MVP, 16 * sizeof(float));
				count++;
			}
			std::cout << n << std::endl;
		}
	}
}

long long AppClass::Factorial(int n)
{
	if (n == 0)
	{
		return 1;
	}

	return n * Factorial(n - 1);
}

long long AppClass::Sum(int n)
{
	if (n == 0)
	{
		return 0;
	}
	
	return n + Sum(n - 1);
}