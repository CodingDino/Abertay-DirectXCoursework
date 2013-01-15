// Solar Exploration Sim
// Developed for DirectX Coursework for Abertay University
// Copyright Sarah Herzog, 2011, all rights reserved.
//
// PositionClass
//		Records and passes along camera position and angle
#pragma once


// |----------------------------------------------------------------------------|
// |								Includes									|
// |----------------------------------------------------------------------------|
#include <math.h>


// |----------------------------------------------------------------------------|
// |					       Class: PositionClass								|
// |----------------------------------------------------------------------------|
class PositionClass
{
public:

	//|-------------------------------Public Functions--------------------------|
	
	// Constructors and Destructors
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	// Setters
	void SetFrameTime(float);

	// Getters
	void GetRotation(float&);

	// Set rotation
	void TurnLeft(bool);
	void TurnRight(bool);

private:
	float m_frameTime;
	float m_rotationY;
	float m_rotationX;
	float m_rotationZ;
	float m_leftTurnSpeed, m_rightTurnSpeed;
};