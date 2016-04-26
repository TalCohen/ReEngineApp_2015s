#include "MyEntityClass.h"
//  MyEntityClass
void MyEntityClass::Init(String a_sMeshName)
{
	m_v3Velocity = vector3(0.0f, 0.0f, 0.0f);
	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_sMeshName = a_sMeshName;
	m_uMeshIndex = m_pMeshMngr->IdentifyInstance(m_sMeshName);
	if (m_uMeshIndex >= 0)
	{
		InstanceClass* pInstance = m_pMeshMngr->GetInstanceByName(m_sMeshName);
		m_pBoundingBox = new MyBoundingBoxClass(pInstance->GetVertexList());
	}
}
void MyEntityClass::Swap(MyEntityClass& other)
{
}
void MyEntityClass::Release(void)
{
	SafeDelete(m_pBoundingBox);
}
void MyEntityClass::AddToRenderList(bool a_bAddBox)
{
	if (a_bAddBox)
	{
		if (m_pBoundingBox)
			m_pBoundingBox->AddToRenderList();
		m_pMeshMngr->AddInstanceToRenderList(m_sMeshName);
	}
}
//The big 3
MyEntityClass::MyEntityClass(String a_sMeshName)
{
	Init(a_sMeshName);
}
MyEntityClass::MyEntityClass(MyEntityClass const& other)
{
}
MyEntityClass& MyEntityClass::operator=(MyEntityClass const& other)
{
	if (this != &other)
	{
		Release();
		Init(other.m_sMeshName);
		MyEntityClass temp(other);
		Swap(temp);
	}
	return *this;
}
MyEntityClass::~MyEntityClass(void) { Release(); };
//Accessors
void MyEntityClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
	m_m4ToWorld = a_m4ToWorld;
	m_pBoundingBox->SetModelMatrix(m_m4ToWorld);
	m_pMeshMngr->SetModelMatrix(m_m4ToWorld, m_sMeshName);
}
matrix4 MyEntityClass::GetModelMatrix(void) { return m_m4ToWorld; }
void MyEntityClass::SetVelocity(vector3 a_v3Velocity){m_v3Velocity = a_v3Velocity;}
vector3 MyEntityClass::GetVelocity(void) { return m_v3Velocity; };
//--- Non Standard Singleton Methods
void MyEntityClass::Update(void)
{
	m_m4ToWorld = m_m4ToWorld * glm::translate(m_v3Velocity);
	SetModelMatrix(m_m4ToWorld);
}
bool MyEntityClass::IsColliding(MyEntityClass * a_pOther)
{
	return m_pBoundingBox->IsColliding(a_pOther->m_pBoundingBox);
}

