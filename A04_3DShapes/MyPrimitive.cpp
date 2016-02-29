#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	//3--2
	//|  |
	//0--1

	std::vector<vector3> points;
	for (int i = 0; i < a_nSubdivisions; i++) {
		double theta = ((double)i / (double)a_nSubdivisions) * 2 * PI;
		vector3 point(a_fRadius * cos(theta), -a_fHeight / 2, a_fRadius * sin(theta));
		points.push_back(point);
	}

	vector3 top(0, a_fHeight / 2, 0);
	vector3 bottom(0, -a_fHeight / 2, 0);

	for (unsigned int i = 0; i < points.size(); i++)
	{
		vector3 first = points.at(i);
		vector3 second;
		if (i != points.size() - 1) 
		{
			second = points.at(i + 1);
		}
		else 
		{
			second = points.at(0);
		}
		AddQuad(bottom, first, second, top);
	}

	points.clear();

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	//3--2
	//|  |
	//0--1
	
	std::vector<vector3> topPoints;
	std::vector<vector3> bottomPoints;
	for (int i = 0; i < a_nSubdivisions; i++) {
		double theta = ((double)i / (double)a_nSubdivisions) * 2 * PI;
		vector3 topPoint(a_fRadius * cos(theta), a_fHeight / 2, a_fRadius * sin(theta));
		vector3 bottomPoint(a_fRadius * cos(theta), -a_fHeight / 2, a_fRadius * sin(theta));
		
		topPoints.push_back(topPoint);
		bottomPoints.push_back(bottomPoint);
	}

	vector3 top(0, a_fHeight / 2, 0);
	vector3 bottom(0, -a_fHeight / 2, 0);

	for (unsigned int i = 0; i < topPoints.size(); i++)
	{
		vector3 topFirst = topPoints.at(i);
		vector3 bottomFirst = bottomPoints.at(i);

		vector3 topSecond;
		vector3 bottomSecond;
		if (i != topPoints.size() - 1)
		{
			topSecond = topPoints.at(i + 1);
			bottomSecond = bottomPoints.at(i + 1);
		}
		else
		{
			topSecond = topPoints.at(0);
			bottomSecond = bottomPoints.at(0);
		}
		
		// Upper triangle
		AddVertexPosition(top);
		AddVertexPosition(topSecond);
		AddVertexPosition(topFirst);

		// Lower triangle
		AddVertexPosition(bottomFirst);
		AddVertexPosition(bottomSecond);
		AddVertexPosition(bottom);

		// Connecting wall
		AddQuad(topFirst, topSecond, bottomFirst, bottomSecond);
	}

	topPoints.clear();
	bottomPoints.clear();

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	
	std::vector<vector3> topInnerPoints;
	std::vector<vector3> topOuterPoints;
	std::vector<vector3> bottomInnerPoints;
	std::vector<vector3> bottomOuterPoints;
	for (int i = 0; i < a_nSubdivisions; i++) {
		double theta = ((double)i / (double)a_nSubdivisions) * 2 * PI;
		vector3 topInnerPoint(a_fInnerRadius * cos(theta), a_fHeight / 2, a_fInnerRadius * sin(theta));
		vector3 topOuterPoint(a_fOuterRadius * cos(theta), a_fHeight / 2, a_fOuterRadius * sin(theta));
		vector3 bottomInnerPoint(a_fInnerRadius * cos(theta), -a_fHeight / 2, a_fInnerRadius * sin(theta));
		vector3 bottomOuterPoint(a_fOuterRadius * cos(theta), -a_fHeight / 2, a_fOuterRadius * sin(theta));

		topInnerPoints.push_back(topInnerPoint);
		topOuterPoints.push_back(topOuterPoint);
		bottomInnerPoints.push_back(bottomInnerPoint);
		bottomOuterPoints.push_back(bottomOuterPoint);
	}

	for (unsigned int i = 0; i < topInnerPoints.size(); i++)
	{
		vector3 topInnerFirst = topInnerPoints.at(i);
		vector3 topOuterFirst = topOuterPoints.at(i);
		vector3 bottomInnerFirst = bottomInnerPoints.at(i);
		vector3 bottomOuterFirst = bottomOuterPoints.at(i);

		vector3 topInnerSecond;		
		vector3 topOuterSecond;
		vector3 bottomInnerSecond;
		vector3 bottomOuterSecond;
		if (i != topInnerPoints.size() - 1)
		{
			topInnerSecond = topInnerPoints.at(i + 1);
			topOuterSecond = topOuterPoints.at(i + 1);
			bottomInnerSecond = bottomInnerPoints.at(i + 1);
			bottomOuterSecond = bottomOuterPoints.at(i + 1);
		}
		else
		{
			topInnerSecond = topInnerPoints.at(0);
			topOuterSecond = topOuterPoints.at(0);
			bottomInnerSecond = bottomInnerPoints.at(0);
			bottomOuterSecond = bottomOuterPoints.at(0);
		}

		// Upper quad
		AddQuad(topInnerFirst, topInnerSecond, topOuterFirst, topOuterSecond);

		// Lower quad
		AddQuad(bottomOuterFirst, bottomOuterSecond, bottomInnerFirst, bottomInnerSecond);

		// Inner wall
		AddQuad(bottomInnerFirst, bottomInnerSecond, topInnerFirst, topInnerSecond);

		// Outer wall
		AddQuad(topOuterFirst, topOuterSecond, bottomOuterFirst, bottomOuterSecond);
	}

	topInnerPoints.clear();
	topOuterPoints.clear();
	bottomInnerPoints.clear();
	bottomOuterPoints.clear();

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();
	
	//Your code starts here
	a_nSubdivisions += 2;
	//3--2
	//|  |
	//0--1

	//3 = 3 and 3*2
	//4 = 4 and 4*2
	
	std::vector<std::vector<vector3>> points;
	for (int i = 0; i < a_nSubdivisions * 2; i++) 
	{
		// Horizontally
		std::vector<vector3> innerPoints;
		double phi = ((double)i / (double)(a_nSubdivisions * 2)) * 2 * PI;
		for (int j = 0; j < a_nSubdivisions; j++)
		{
			// Vertically
			double theta = ((double)j / (double)a_nSubdivisions) * PI;
			vector3 point(a_fRadius * cos(theta) * sin(phi), a_fRadius * cos(phi), a_fRadius * sin(theta) * sin(phi));
			innerPoints.push_back(point);
		}
		points.push_back(innerPoints);
	}

	// Some problem with cosine being even and the way the triangles are drawn not being visible from the correct side.
	
	for (unsigned int i = 0; i < points.size(); i++)
	{
		for (unsigned int j = 1; j < points.at(i).size() - 1; j++)
		{
			vector3 point1 = points.at(i).at(j);
			vector3 point2 = points.at(i).at(j + 1);

			int index;
			if (i != points.size() - 1)
			{
				index = i + 1;
			}
			else
			{
				index = 0;
			}

			vector3 point3 = points.at(index).at(j);
			vector3 point4 = points.at(index).at(j + 1);

			AddQuad(point1, point2, point3, point4);
		}
	}

	for (unsigned int i = 0; i < points.size(); i++)
	{
		points.at(i).clear();
	}
	points.clear();

	//Your code ends here
	CompileObject(a_v3Color);
}