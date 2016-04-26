/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/06
----------------------------------------------*/
#ifndef __MYENTITYCLASS_H_
#define __MYENTITYCLASS_H_

#include "RE\ReEngAppClass.h"
#include "MyBoundingBoxClass.h"

//System Class
class MyEntityClass
{
	uint m_uMeshIndex = 0; //Index of the object in the mesh manager
	String m_sMeshName = ""; //Index of the object in the mesh manager
	MyBoundingBoxClass* m_pBoundingBox = nullptr; //Bounding Box of the object
	MeshManagerSingleton* m_pMeshMngr = nullptr; //singleton pointer of the mesh manager
	matrix4 m_m4ToWorld; //model matrix that represents the position of the object in global space
	vector3 m_v3Velocity; //velocity of the object

public:
	/*
	Usage: Constructor
	Arguments:
		String m_sMeshName -> Name of the instance in the mesh manager to make an entity from
	Output: class object
	*/
	MyEntityClass(String m_sMeshName);

	/*
	Method: MyEntityClass
	Usage: Copy Constructor
	Arguments: class object to copy
	Output: class object instance
	*/
	MyEntityClass(MyEntityClass const& other);

	/*
	Method: operator=
	Usage: Copy Assignment Operator
	Arguments: class object to copy
	Output: ---
	*/
	MyEntityClass& operator=(MyEntityClass const& other);

	/*
	Method: ~MyEntityClass
	Usage: Destructor
	Arguments: ---
	Output: ---
	*/
	~MyEntityClass(void);

	/*
	Method: Swap
	Usage: Changes object contents for other object's
	Arguments:
		other -> object to swap content from
	Output: ---
	*/
	void Swap(MyEntityClass& other);

	/*
	Usage: Adds the object to the render list
	Arguments:
		bool a_bAddBox = false -> adds the box to the render list as well
	Output: ---
	*/
	void AddToRenderList(bool a_bAddBox = false);

	/*
	USAGE: Sets the Entity into world coordinates
	ARGUMENTS:
		matrix4 a_m4ToWorld -> Model to World matrix
	OUTPUT: ---
	*/
	void SetModelMatrix(matrix4 a_m4ToWorld);

	/*
	USAGE: Gets the Entity ModelToWorld matrix
	ARGUMENTS: ---
	OUTPUT: Model to World matrix
	*/
	matrix4 GetModelMatrix(void);

	/*
	USAGE: Sets the velocity of the object
	ARGUMENTS:
		vector3 a_v3Velocity -> velocity to be applied to the object
	OUTPUT: ---
	*/
	void SetVelocity(vector3 a_v3Velocity);

	/*
	USAGE: Gets the velocity of the object
	ARGUMENTS: ---
	OUTPUT: velocity of the object
	*/
	vector3 GetVelocity(void);

	/*
	USAGE: Updates the position of the object based on its velocity
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Update(void);

	/*
	USAGE: Asks if an entity is colliding with the specified one
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	bool IsColliding(MyEntityClass* a_pOther);

private:
	/*
	Method: Release
	Usage: Deallocates member fields
	Arguments: ---
	Output: ---
	*/
	void Release(void);

	/*
	Method: Init
	Usage: Allocates member fields
	Arguments: ---
	Output: ---
	*/
	void Init(String a_sMeshName);
};

#endif //__MYENTITYCLASS_H__