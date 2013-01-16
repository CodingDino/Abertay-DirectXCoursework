// Solar Exploration Sim
// Developed for DirectX Coursework for Abertay University
// Copyright Sarah Herzog, 2011, all rights reserved.
//
// GraphicsClass
//		Overall control of graphics and rendering, holds model objects and other graphics objects
#pragma once


// |----------------------------------------------------------------------------|
// |								Includes									|
// |----------------------------------------------------------------------------|
#include "d3dclass.h"
#include "cameraclass.h"
#include "lightshaderclass.h"
#include "textureshaderclass.h"
#include "lightclass.h"
#include "Coord.h"
#include "Util.h"
#include "BitmapClass.h"
#include "textclass.h"
#include "PlanetClass.h"


// |----------------------------------------------------------------------------|
// |								 Globals									|
// |----------------------------------------------------------------------------|
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const int NUM_MODELS=20;


// |----------------------------------------------------------------------------|
// |					       Class: GraphicsClass								|
// |----------------------------------------------------------------------------|
class GraphicsClass
{
public:

	//|-------------------------------Public Functions--------------------------|

	// Constructors and Destructors
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();
	
	// Creates and initializes all data members, sets initial graphics state
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);

	// Performs shutdown, deallocation, and cleanup for data members
	void Shutdown();

	// Frame-by-frame graphics processing
	bool Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime, Coord camera_rotation, Coord camera_position);

private:

	//|-------------------------------Private Functions-------------------------|

	// Calls render functions in order
	// TODO: Merge this with Frame(), maybe?
	bool Render(int mouseX, int mouseY);

	// Rendering setup (starts scene, gets world, view, and projection matrix)
	bool BeginRender();

	// Rendering cleanup (ends the scene)
	bool EndRender();

	// Renders the passed in model using the given transforms
	bool ModelRender(ModelClass& to_render, Coord scale, Coord translate, Coord rotate);

	// Renders the passed in bitmap using the given transforms
	bool BitmapRender(BitmapClass& to_render, int x, int y);

private:

	//|-----------------------------Private Data Members------------------------|

	// DirectX object
	D3DClass* m_D3D;

	// Camera
	CameraClass* m_Camera;

	// Shaders
	LightShaderClass* m_LightShader;
	TextureShaderClass* m_TextureShader;

	// Light source 
	// TODO: make an array of light sources, lights should be placeable in the scene
	LightClass* m_Light;

	// Models
	ModelClass** m_Models;

	// Matrices
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, baseViewMatrix;

	// 2D Bitmaps
	BitmapClass* crosshairs;
	BitmapClass* HUD;

	// FPS output
	TextClass* m_Text;

	// Planets
	PlanetClass* m_sun;

	// Screen dimmensions
	int m_screenWidth;
	int m_screenHeight;
};