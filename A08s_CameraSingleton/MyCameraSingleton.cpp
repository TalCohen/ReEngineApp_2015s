#include "MyCameraSingleton.h"
//  MyCameraSingleton
MyCameraSingleton* MyCameraSingleton::m_pInstance = nullptr;

MyCameraSingleton* MyCameraSingleton::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyCameraSingleton();
	}
	return m_pInstance;
}
void MyCameraSingleton::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

void MyCameraSingleton::Init(void)
{
	m_bFPS = true;

	m_nMode = CAMERAMODE::CAMPERSP;

	m_fFOV = 45.0f;

	m_v2NearFar = vector2(0.001f, 1000.0f);

	m_v3Position = vector3(0.0f, 0.0f, 5.0f);
	m_v3Target = vector3(0.0f, 0.0f, 0.0f);
	m_v3Top = vector3(0.0f, 1.0f, 0.0f);

	m_v3Forward = vector3(0.0f, 0.0f, -1.0f);
	m_v3Upward = vector3(0.0f, 1.0f, 0.0f);
	m_v3Rightward = vector3(1.0f, 0.0f, 0.0f);

	m_v3PitchYawRoll = vector3(0.0f);

	m_m4Projection = matrix4(1.0f);
	m_m4View = matrix4(1.0f);
}
void MyCameraSingleton::Swap(MyCameraSingleton& other)
{
	std::swap(m_bFPS, other.m_bFPS);

	std::swap(m_nMode, other.m_nMode);

	std::swap(m_fFOV, other.m_fFOV);

	std::swap(m_v2NearFar, other.m_v2NearFar);

	std::swap(m_v3Position, other.m_v3Position);
	std::swap(m_v3Target, other.m_v3Target);
	std::swap(m_v3Top, other.m_v3Top);

	std::swap(m_v3Forward, other.m_v3Forward);
	std::swap(m_v3Upward, other.m_v3Upward);
	std::swap(m_v3Rightward, other.m_v3Rightward);

	std::swap(m_v3PitchYawRoll, other.m_v3PitchYawRoll);

	std::swap(m_m4Projection, other.m_m4Projection);
	std::swap(m_m4View, other.m_m4View);
}
void MyCameraSingleton::Release(void){}
//The big 3
MyCameraSingleton::MyCameraSingleton()
{
	Init();
	CalculateProjection();
}
MyCameraSingleton::MyCameraSingleton(MyCameraSingleton const& other)
{
	m_bFPS = other.m_bFPS;

	m_nMode = other.m_nMode;

	m_fFOV = other.m_fFOV;

	m_v2NearFar = other.m_v2NearFar;

	m_v3Position = other.m_v3Position;
	m_v3Target = other.m_v3Target;
	m_v3Top = other.m_v3Top;

	m_v3Forward = other.m_v3Forward;
	m_v3Upward = other.m_v3Upward;
	m_v3Rightward = other.m_v3Rightward;

	m_v3PitchYawRoll = other.m_v3PitchYawRoll;

	m_m4Projection = other.m_m4Projection;
	m_m4View = other.m_m4View;
}
MyCameraSingleton& MyCameraSingleton::operator=(MyCameraSingleton const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyCameraSingleton temp(other);
		Swap(temp);
	}
	return *this;
}
MyCameraSingleton::~MyCameraSingleton(){ Release(); };
//Accessors
vector3 MyCameraSingleton::GetPosition(void){ return m_v3Position; }
matrix4 MyCameraSingleton::GetViewMatrix(void){ CalculateView(); return m_m4View; }
matrix4 MyCameraSingleton::GetProjectionMatrix(void){ CalculateProjection(); return m_m4Projection; }
void MyCameraSingleton::SetNearFarPlanes(float a_fNear, float a_fFar){ m_v2NearFar = vector2(a_fNear, a_fFar); }
void MyCameraSingleton::SetFOV(float a_fFOV){ m_fFOV = a_fFOV; }
void MyCameraSingleton::SetFPS(bool a_bFPS){ m_bFPS = a_bFPS; }
void MyCameraSingleton::SetCameraMode(CAMERAMODE a_nMode){ m_nMode = a_nMode; ResetCamera(); }
CAMERAMODE MyCameraSingleton::GetCameraMode(void){ return m_nMode; }
void MyCameraSingleton::SetPosition(vector3 a_v3Position)
{
	m_v3Position = a_v3Position;

	m_v3Forward = glm::normalize(m_v3Target - m_v3Position);
	m_v3Upward = glm::normalize(glm::cross(m_v3Rightward, m_v3Forward));
	m_v3Rightward = glm::normalize(glm::cross(m_v3Forward, m_v3Upward));

	m_v3Top = m_v3Position + m_v3Upward;
	m_v3Target = m_v3Position + glm::normalize(m_v3Forward);

	CalculateProjection();
}
void MyCameraSingleton::SetTarget(vector3 a_v3Target)
{
	m_v3Target = a_v3Target;

	m_v3Forward = glm::normalize(m_v3Target - m_v3Position);
	m_v3Upward = glm::normalize(glm::cross(m_v3Rightward, m_v3Forward));
	m_v3Rightward = glm::normalize(glm::cross(m_v3Forward, m_v3Upward));
	
	m_v3Top = m_v3Position + m_v3Upward;
	m_v3Target = m_v3Position + glm::normalize(m_v3Forward);

	CalculateProjection();
}
matrix4 MyCameraSingleton::GetMVP(matrix4 a_m4ModelToWorld)
{
	CalculateView();
	CalculateProjection();
	return m_m4Projection * m_m4View * a_m4ModelToWorld;
}
matrix4 MyCameraSingleton::GetVP(void)
{
	CalculateView();
	CalculateProjection();
	return m_m4Projection * m_m4View;
}
//--- Non Standard Singleton Methods
void MyCameraSingleton::CalculateProjection(void)
{
	SystemSingleton* pSystem = SystemSingleton::GetInstance();
	float fRatio = static_cast<float>(pSystem->GetWindowWidth()) / static_cast<float>(pSystem->GetWindowHeight());
	float fPos = 0;
	switch (m_nMode)
	{
	default:
		m_m4Projection = glm::perspective(m_fFOV, fRatio, m_v2NearFar.x, m_v2NearFar.y);
		break;

	case CAMERAMODE::CAMROTHOX:
		fPos = m_v3Position.x;
		m_m4Projection = glm::ortho(-fPos * fRatio, fPos * fRatio, -fPos, fPos, m_v2NearFar.x, m_v2NearFar.y);
		break;

	case CAMERAMODE::CAMROTHOY:
		fPos = m_v3Position.y;
		m_m4Projection = glm::ortho(-fPos * fRatio, fPos * fRatio, -fPos, fPos, m_v2NearFar.x, m_v2NearFar.y);
		break;

	case CAMERAMODE::CAMROTHOZ:
		fPos = m_v3Position.z;
		m_m4Projection = glm::ortho(-fPos * fRatio, fPos * fRatio, -fPos, fPos, m_v2NearFar.x, m_v2NearFar.y);
		break;
	}
}
void MyCameraSingleton::CalculateView(void)
{
	//Calculate the Forward again
	m_v3Forward = glm::normalize(m_v3Target - m_v3Position);
	//Determine axis for pitch rotation
	m_v3Rightward = glm::cross(m_v3Forward, m_v3Upward);
	//Compute quaternion for pitch based on the camera pitch angle
	quaternion qPitch = glm::angleAxis(m_v3PitchYawRoll.x, m_v3Rightward);
	//Compute quaternion for Yaw based on the camera pitch angle
	quaternion qYaw = glm::angleAxis(m_v3PitchYawRoll.y, m_v3Upward);
	//Compute quaternion for Roll based on the camera pitch angle
	quaternion qRoll = glm::angleAxis(m_v3PitchYawRoll.z, m_v3Forward);

	//Add the quaternions
	quaternion qTemp = glm::cross(qPitch, qYaw);
	//update the direction from the quaternion
	m_v3Forward = glm::rotate(qTemp, m_v3Forward);


	//This will affect all of the other directions, generally speaking you do not want that other than in flight games
	if (m_bFPS == false)
	{
		qTemp = glm::cross(qRoll, qPitch);
		m_v3Upward = glm::rotate(qTemp, m_v3Upward);

		qTemp = glm::cross(qYaw, qRoll);
		m_v3Rightward = glm::rotate(qTemp, m_v3Rightward);
	}
	
	//set the look at to be in front of the camera
	m_v3Target = m_v3Position + m_v3Forward;
	m_v3Top = m_v3Position + m_v3Upward;
	
	//Damping for a smooth camera
	m_v3PitchYawRoll *= 0.5f;
	
	//Calculate the look at
	m_m4View = glm::lookAt(m_v3Position, m_v3Target, m_v3Upward);
}
void MyCameraSingleton::MoveForward(float a_fDistance)
{
	m_v3Position += m_v3Forward * a_fDistance;
	m_v3Target += m_v3Forward * a_fDistance;
	m_v3Top += m_v3Forward * a_fDistance;

	m_v3Forward = glm::normalize(m_v3Target - m_v3Position);
	m_v3Upward = glm::normalize(m_v3Top - m_v3Position);
	m_v3Rightward = glm::normalize(glm::cross(m_v3Forward, m_v3Upward));
	if (m_nMode != CAMERAMODE::CAMPERSP)
	{
		CalculateProjection();
	}
}
void MyCameraSingleton::MoveVertical(float a_fDistance)
{
	m_v3Position += m_v3Upward * a_fDistance;
	m_v3Target += m_v3Upward * a_fDistance;
	m_v3Top += m_v3Upward * a_fDistance;

	m_v3Forward = glm::normalize(m_v3Target - m_v3Position);
	m_v3Upward = glm::normalize(m_v3Top - m_v3Position);
	m_v3Rightward = glm::normalize(glm::cross(m_v3Forward, m_v3Upward));
	if (m_nMode != CAMERAMODE::CAMPERSP)
	{
		CalculateProjection();
	}
}
void MyCameraSingleton::MoveSideways(float a_fDistance)
{
	m_v3Position += m_v3Rightward * a_fDistance;
	m_v3Target += m_v3Rightward * a_fDistance;
	m_v3Top += m_v3Rightward * a_fDistance;

	m_v3Forward = glm::normalize(m_v3Target - m_v3Position);
	m_v3Upward = glm::normalize(m_v3Top - m_v3Position);
	m_v3Rightward = glm::normalize(glm::cross(m_v3Forward, m_v3Upward));
	if (m_nMode != CAMERAMODE::CAMPERSP)
	{
		CalculateProjection();
	}
}
void MyCameraSingleton::ChangePitch(float a_fDegree)
{
	if (m_nMode == CAMERAMODE::CAMPERSP)
		m_v3PitchYawRoll.x += a_fDegree;
}
void MyCameraSingleton::ChangeYaw(float a_fDegree)
{
	if (m_nMode == CAMERAMODE::CAMPERSP)
		m_v3PitchYawRoll.y += a_fDegree;
}
void MyCameraSingleton::ChangeRoll(float a_fDegree)
{
	if (m_nMode == CAMERAMODE::CAMPERSP)
		m_v3PitchYawRoll.z += a_fDegree;
}
void MyCameraSingleton::SetPositionAndTarget(vector3 a_v3Position, vector3 a_v3Target)
{
	SetPosition(a_v3Position);
	SetTarget(a_v3Target);
}
void MyCameraSingleton::ResetCamera(void)
{
	m_v3PitchYawRoll = vector3(0.0f);

	switch (m_nMode)
	{
	default:
		m_v3Position = vector3(0.0f, 0.0f, 10.0f);
		m_v3Target = vector3(0.0f, 0.0f, 9.0f);
		m_v3Top = vector3(0.0f, 1.0f, 10.0f);

		m_v3Forward = vector3(0.0f, 0.0f, -1.0f);
		m_v3Upward = vector3(0.0f, 1.0f, 0.0f);
		m_v3Rightward = vector3(1.0f, 0.0f, 0.0f);
		break;
	case CAMERAMODE::CAMROTHOX:
		m_v3PitchYawRoll = vector3(0.0f);
		m_v3Position = vector3(10.0f, 0.0f, 0.0f);
		m_v3Target = vector3(9.0f, 0.0f, 0.0f);
		m_v3Top = vector3(10.0f, 1.0f, 0.0f);

		m_v3Forward = vector3(-1.0f, 0.0f, 0.0f);
		m_v3Upward = vector3(0.0f, 1.0f, 0.0f);
		m_v3Rightward = vector3(0.0f, 0.0f, -1.0f);
		break;
	case CAMERAMODE::CAMROTHOY:
		m_v3PitchYawRoll = vector3(0.0f);
		m_v3Position = vector3(0.0f, 10.0f, 0.0f);
		m_v3Target = vector3(0.0f, 9.0f, 0.0f);
		m_v3Top = vector3(0.0f, 10.0f, -1.0f);

		m_v3Forward = vector3(0.0f, -1.0f, 0.0f);
		m_v3Upward = vector3(0.0f, 0.0f, -1.0f);
		m_v3Rightward = vector3(1.0f, 0.0f, 0.0f);
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_v3PitchYawRoll = vector3(0.0f);
		m_v3Position = vector3(0.0f, 0.0f, 10.0f);
		m_v3Target = vector3(0.0f, 0.0f, 9.0f);
		m_v3Top = vector3(0.0f, 1.0f, 10.0f);

		m_v3Forward = vector3(0.0f, 0.0f, -1.0f);
		m_v3Upward = vector3(0.0f, 1.0f, 0.0f);
		m_v3Rightward = vector3(1.0f, 0.0f, 0.0f);
		break;
	}
}
matrix4 MyCameraSingleton::GetCameraSpaceAdjusted(void)
{
	CalculateView();
	return glm::inverse(m_m4Projection) * glm::translate(vector3(0.0f, 0.0f, -1.2085f));
}
matrix4 MyCameraSingleton::GetCameraPlane(void)
{
	SystemSingleton* pSystem = SystemSingleton::GetInstance();
	float fSize = pSystem->GetWindowWidth() / static_cast<float>(pSystem->GetWindowHeight());
	return GetCameraSpaceAdjusted() * glm::translate(vector3(0.0f, 0.0f, -1.2085f)) * glm::scale(vector3(fSize, 1.0f, 0.0f));
}