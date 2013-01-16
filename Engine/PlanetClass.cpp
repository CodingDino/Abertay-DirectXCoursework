// Solar Exploration Sim
// Developed for DirectX Coursework for Abertay University
// Copyright Sarah Herzog, 2011, all rights reserved.
//
// PlanetClass
//		Contains model and texture for the planet. Also contains data and methods
//		for radius, rotation speed, orbit speed, radius of orbit, center of orbit, and 
//		plane of orbit.


// |----------------------------------------------------------------------------|
// |								Includes									|
// |----------------------------------------------------------------------------|
#include "PlanetClass.h"


//|----------------------------------------------------------------------------|
// |						   Default Constructor								|
// |----------------------------------------------------------------------------|
PlanetClass::PlanetClass() :
	m_model(0),
	m_radius(0),
	m_rotation_speed(0),
	m_rotation_tilt(0,0,0),
	m_orbit_speed(0),
	m_orbit_radius(0),
	m_orbit_center(0,0,0),
	m_orbit_tilt(0,0,0),
	m_rotation(0),
	m_orbit(0)
{
}

	
// |----------------------------------------------------------------------------|
// |						    Copy Constructor								|
// |----------------------------------------------------------------------------|
PlanetClass::PlanetClass(const PlanetClass& other)
{
}


// |----------------------------------------------------------------------------|
// |						     Deconstructor									|
// |----------------------------------------------------------------------------|
PlanetClass::~PlanetClass()
{
}


// |----------------------------------------------------------------------------|
// |						      Initialize									|
// |----------------------------------------------------------------------------|
bool PlanetClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename,
		float radius, float rotation_speed, Coord rotation_tilt, float orbit_speed, 
		float orbit_radius, Coord orbit_center, Coord orbit_tilt)
{
	// Create model
	m_model = new ModelClass;
	if(!m_model)
	{
		return false;
	}	
	// Initialize the model object.
	if(!(m_model->Initialize(device, modelFilename, textureFilename)))
	{
		return false;
	}

	// Set planet constants
	m_radius = radius;
	m_rotation_speed = rotation_speed;
	m_rotation_tilt = rotation_tilt;
	m_orbit_speed = orbit_speed;
	m_orbit_radius = orbit_radius;
	m_orbit_center = orbit_center;
	m_orbit_tilt = orbit_tilt;

	return true;
}


// |----------------------------------------------------------------------------|
// |						      Shutdown										|
// |----------------------------------------------------------------------------|
void PlanetClass::Shutdown()
{

	// Shutdown model
	if(m_model)
	{
		m_model->Shutdown();
		delete m_model;
		m_model = 0;
	}

	return;
}


// |----------------------------------------------------------------------------|
// |						       Frame										|
// |----------------------------------------------------------------------------|
void PlanetClass::Frame(float frameTime)
{

	// Apply rotation change
	m_rotation += m_rotation_speed / frameTime;
	if(m_rotation < 0.0f)
	{
		m_rotation += 360.0f;
	}
	if(m_rotation > 360.0f)
	{
		m_rotation -= 360.0f;
	}

	// Apply orbit change
	m_orbit += m_orbit_speed / frameTime;
	if(m_orbit < 0.0f)
	{
		m_orbit += 360.0f;
	}
	if(m_orbit > 360.0f)
	{
		m_orbit -= 360.0f;
	}

	return;
}


// |----------------------------------------------------------------------------|
// |						       Render										|
// |----------------------------------------------------------------------------|
void PlanetClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	if(m_model)
	{
		m_model->Render(deviceContext);
	}

	return;
}


// |----------------------------------------------------------------------------|
// |						       GetModel										|
// |----------------------------------------------------------------------------|
void PlanetClass::GetModel(ModelClass* model)
{
	model = m_model;

	return;
}


// |----------------------------------------------------------------------------|
// |						       GetScale										|
// |----------------------------------------------------------------------------|
Coord PlanetClass::GetScale()
{
	return Coord(m_radius,m_radius,m_radius);
}


// |----------------------------------------------------------------------------|
// |						      GetTranslate									|
// |----------------------------------------------------------------------------|
Coord PlanetClass::GetTranslate()
{
	D3DXMATRIX worldMatrix, translationMatrix, rotationMatrix, tiltMatrix;

	// Translate out to orbit radius
	D3DXMatrixTranslation(&translationMatrix, m_orbit_radius, 0, 0);

	// Determine point in orbit
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, m_orbit, 0, 0);

	// Determine skew of orbit based on orbit rotation
	D3DXMatrixRotationYawPitchRoll(&tiltMatrix, 0, m_orbit_tilt.x, m_orbit_tilt.z);
	
	// Translate first, then rotate and skew
	worldMatrix = translationMatrix * rotationMatrix * tiltMatrix;

	return Coord(worldMatrix._41,worldMatrix._42,worldMatrix._43);
}


// |----------------------------------------------------------------------------|
// |						       GetRotate									|
// |----------------------------------------------------------------------------|
Coord PlanetClass::GetRotate()
{
	return Coord(m_rotation_tilt.x,m_rotation,m_rotation_tilt.z);
}
