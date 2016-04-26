/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@gmail.com)
Date: 2015/10
----------------------------------------------*/
#ifndef __MyCameraSingleton_H_
#define __MyCameraSingleton_H_

#include "RE\ReEng.h"

//Camera Class
class MyCameraSingleton
{
	bool m_bFPS = true; //If set to false will behave like a flight simulator camera

	CAMERAMODE m_nMode = CAMERAMODE::CAMPERSP; //Sets the mode the camera will display

	float m_fFOV = 45.0f;

	vector2 m_v2NearFar = vector2(0.001f, 1000.0f);

	vector3 m_v3Position = vector3(0.0f, 0.0f, 5.0f);
	vector3 m_v3Target = vector3(0.0f, 0.0f, 0.0f);
	vector3 m_v3Top = vector3(0.0f, 1.0f, 0.0f);

	vector3 m_v3Forward = vector3(0.0f, 0.0f, -1.0f);
	vector3 m_v3Upward = vector3(0.0f, 1.0f, 0.0f);
	vector3 m_v3Rightward = vector3(1.0f, 0.0f, 0.0f);

	vector3 m_v3PitchYawRoll = vector3(0.0f);

	matrix4 m_m4Projection = matrix4(1.0f);
	matrix4 m_m4View = matrix4(1.0f);

	static MyCameraSingleton* m_pInstance; // Singleton pointer

private:
	/*
	 CameraClass
	USAGE: Constructor
	ARGUMENTS: ---
	OUTPUT: class object
	*/
	MyCameraSingleton(void);
	/* Copy Constructor */
	/*
	 
	USAGE:
	ARGUMENTS:
	OUTPUT:
	*/
	MyCameraSingleton(MyCameraSingleton const& other);
	/* Copy Assignment Operator*/
	/*
	 
	USAGE:
	ARGUMENTS:
	OUTPUT:
	*/
	MyCameraSingleton& operator=(MyCameraSingleton const& other);
	/* Destructor */
	/*
	 
	USAGE:
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	~MyCameraSingleton(void);

public:
	/*
	
	USAGE:
	ARGUMENTS:
	OUTPUT:
	*/
	static MyCameraSingleton* GetInstance();
	/*
	
	USAGE:
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	static void ReleaseInstance(void);
	/* Gets the View matrix from the camera */
	/*
	 
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	matrix4 GetViewMatrix(void);
	/* Gets the Projection matrix from the camera */
	/*
	 
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	matrix4 GetProjectionMatrix(void);
	/* Gets the ModelViewProjection matrix from the camera */
	/*
	 
	USAGE:
	ARGUMENTS:
	OUTPUT:
	*/
	matrix4 GetMVP(matrix4 a_m4ModelToWorld);
	/* Gets the ViewProjection matrix from the camera */
	/*
	 
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	matrix4 GetVP(void);

	/* Gets the position of the camera */
	/*
	 
	USAGE:
	ARGUMENTS: ---
	OUTPUT:
	*/
	vector3 GetPosition(void);

	/* Changes the near and far planes */
	/*
	 
	USAGE:
	ARGUMENTS:
	OUTPUT:
	*/
	void SetNearFarPlanes(float a_fNear, float a_fFar);
	/* Calculates the projection based on the camera values */
	/*
	 
	USAGE:
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void CalculateProjection(void);
	/* Calculates the view based on the camera values */
	/*
	 
	USAGE:
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void CalculateView(void);
	/* Set Field of View */
	/*
	 
	USAGE:
	ARGUMENTS:
	OUTPUT:
	*/
	void SetFOV(float a_fFOV);

	/*
	 
	USAGE: Sets the camera at the specified position
	ARGUMENTS:
	OUTPUT:
	*/
	void SetPosition(vector3 a_v3Position);
	/*
	
	USAGE: Sets the target of the camera to the specified position
	ARGUMENTS:
		vector3 a_vTarget -> point to look at
	OUTPUT:
	*/
	void SetTarget(vector3 a_v3Target);
	/*
	 
	USAGE: Sets the camera at the specified position and target
	ARGUMENTS:
	OUTPUT:
	*/
	void SetPositionAndTarget(vector3 a_v3Position, vector3 a_v3Target);

	/*
	 
	USAGE: Gets the camera space just in front of the camera
	ARGUMENTS: ---
	OUTPUT:
	*/
	matrix4 GetCameraSpaceAdjusted(void);
		
	/*
	 
	USAGE: Gets the camera plane of the camera
	ARGUMENTS: ---
	OUTPUT:
	*/
	matrix4 GetCameraPlane(void);

	/*
	 
	USAGE: Translates the camera forward or backward
	ARGUMENTS: 
	OUTPUT: ---
	*/
	void MoveForward(float a_fDistance = 0.1f);
	/*
	 
	USAGE: Translates the camera Upward or downward
	ARGUMENTS: 
	OUTPUT: ---
	*/
	void MoveVertical(float a_fDistance = 0.1f);
	/*
	 
	USAGE: Translates the camera right or left
	ARGUMENTS:
	OUTPUT: ---
	*/
	void MoveSideways(float a_fDistance = 0.1f);

	/*
	 
	USAGE: Rotates the camera Pitch
	ARGUMENTS: 
	OUTPUT: ---
	*/
	void ChangePitch(float a_fDegree = 0.01f);
		
	/*
	 
	USAGE: Rotates the camera Yaw
	ARGUMENTS:
	OUTPUT: ---
	*/
	void ChangeYaw(float a_fDegree = 0.01f);
	/*
	 
	USAGE: Rotates the camera Roll
	ARGUMENTS:
	OUTPUT: ---
	*/
	void ChangeRoll(float a_fDegree = 0.01f);

		
	/*
	 
	USAGE: Sets the camera in First Person Shooter mode, if false it will work like an aircraft
	ARGUMENTS:
	OUTPUT: ---
	*/
	void SetFPS(bool a_bFPS = true);
		
	/*
	 
	USAGE: Sets the camera in Perspective mode, if false it will work as an orthographic camera
	ARGUMENTS:
	OUTPUT:
	*/
	void SetCameraMode(CAMERAMODE a_nMode = CAMERAMODE::CAMPERSP);
		
	/*
	 
	USAGE: Returns the current value for the camera mode
	ARGUMENTS: ---
	OUTPUT:
	*/
	CAMERAMODE GetCameraMode(void);

	/*
	 
	USAGE: Resets the value of the camera
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void ResetCamera(void);

private:
	/*
	 
	USAGE: Releases the object from memory
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Release(void);
	/*
	 
	USAGE: Initialize the object's fields
	ARGUMENTS: ---
	OUTPUT: ---
	*/
	void Init(void);
		
	/*
	 
	USAGE: Swaps the contents of the object with another object's content
	ARGUMENTS:
	OUTPUT: ---
	*/
	void Swap(MyCameraSingleton& other);
};

#endif //__MyCameraSingleton_H_