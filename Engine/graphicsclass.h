////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "coord.h"
#include "Util.h"


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
#define NUM_MODELS 20

////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render(float, float);
	bool beginRender();
	bool endRender();
	bool modelRender(ModelClass& to_render, Coord scale, Coord translate, Coord rotate);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	LightShaderClass* m_LightShader;
	LightClass* m_Light;

	// Models
	ModelClass** m_Models;

	// Matrices
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix;
};

#endif