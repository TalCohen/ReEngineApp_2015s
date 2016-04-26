#include "MyBoundingSphereClass.h"
//  MyBoundingSphereClass
void MyBoundingSphereClass::Init(void)
{
	m_fRadius = 0.0f;
	m_m4ToWorld = IDENTITY_M4;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);
}
void MyBoundingSphereClass::Swap(MyBoundingSphereClass& other)
{
	std::swap(m_fRadius, other.m_fRadius);
	std::swap(m_m4ToWorld, other.m_m4ToWorld);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
}
void MyBoundingSphereClass::Release(void)
{

}
//The big 3
MyBoundingSphereClass::MyBoundingSphereClass(std::vector<vector3> a_lVectorList)
{
	//Init the default values
	Init();

	//Count the points of the incoming list
	uint nVertexCount = a_lVectorList.size();

	//If there are none just return, we have no information to create the BS from
	if (nVertexCount == 0)
		return;
	
	//Max and min as the first vector of the list
	m_v3Max = m_v3Min = a_lVectorList[0];
	
	//Get the max and min out of the list
	for (uint nVertex = 1; nVertex < nVertexCount; nVertex++)
	{
		if (m_v3Min.x > a_lVectorList[nVertex].x) //If min is larger than current
			m_v3Min.x = a_lVectorList[nVertex].x;
		else if (m_v3Max.x < a_lVectorList[nVertex].x)//if max is smaller than current
			m_v3Max.x = a_lVectorList[nVertex].x;

		if (m_v3Min.y > a_lVectorList[nVertex].y) //If min is larger than current
			m_v3Min.y = a_lVectorList[nVertex].y;
		else if (m_v3Max.y < a_lVectorList[nVertex].y)//if max is smaller than current
			m_v3Max.y = a_lVectorList[nVertex].y;

		if (m_v3Min.z > a_lVectorList[nVertex].z) //If min is larger than current
			m_v3Min.z = a_lVectorList[nVertex].z;
		else if (m_v3Max.z < a_lVectorList[nVertex].z)//if max is smaller than current
			m_v3Max.z = a_lVectorList[nVertex].z;
	}

	//with the max and the min we calculate the center
	m_v3Center = (m_v3Min + m_v3Max) / 2.0f;

	//we calculate the distance between the center and all the points, the larger will be the radius
	for (uint nVertex = 0; nVertex < nVertexCount; nVertex++)
	{
		float fDistance = glm::distance(m_v3Center, a_lVectorList[nVertex]);
		if (m_fRadius < fDistance)
			m_fRadius = fDistance;
	}
}
MyBoundingSphereClass::MyBoundingSphereClass(MyBoundingSphereClass const& other)
{
	m_fRadius = other.m_fRadius;
	m_m4ToWorld = other.m_m4ToWorld;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
}
MyBoundingSphereClass& MyBoundingSphereClass::operator=(MyBoundingSphereClass const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyBoundingSphereClass temp(other);
		Swap(temp);
	}
	return *this;
}
MyBoundingSphereClass::~MyBoundingSphereClass(){Release();};
//Accessors
void MyBoundingSphereClass::SetModelMatrix(matrix4 a_m4ToWorld){ m_m4ToWorld = a_m4ToWorld; }
vector3 MyBoundingSphereClass::GetCenter(void){ return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
float MyBoundingSphereClass::GetRadius(void) { return m_fRadius; }
//--- Non Standard Singleton Methods
bool MyBoundingSphereClass::IsColliding(MyBoundingSphereClass* const a_pOther)
{
	//Get both centers in world coordinates
	vector3 v3Center = GetCenter();
	vector3 v3CenterOther = a_pOther->GetCenter();

	//Calculate the distance between both centers
	float fDistance = glm::distance(v3Center, v3CenterOther);

	//Is the distance between both centers smaller than the sum of their radius?
	return fDistance < (m_fRadius + a_pOther->m_fRadius);
}