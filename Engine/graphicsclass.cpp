////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_Model = 0;
	m_Model_2 = 0;
	m_LightShader = 0;
	m_Light = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the model objects.
	m_Model = new ModelClass;
	if(!m_Model)
	{
		return false;
	}
	m_Model_2 = new ModelClass;
	if(!m_Model_2)
	{
		return false;
	}

	// Initialize the model object.
	result = m_Model->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/seafloor.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	result = m_Model_2->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/seafloor.dds");
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.05f, 0.05f, 0.05f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(16.0f);

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	// Release the model object.
	if(m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}
	if(m_Model_2)
	{
		m_Model_2->Shutdown();
		delete m_Model_2;
		m_Model_2 = 0;
	}

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	return;
}


bool GraphicsClass::Frame()
{
	bool result;
	static float rotation = 0.0f;
	static float delta =0.0f;


	// Update the rotation variable each frame.
	rotation += (float)D3DX_PI * 0.01f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	
	// Render the graphics scene.
	result = Render(rotation, delta);
	if(!result)
	{
		return false;
	}

	return true;
}


bool GraphicsClass::Render(float rotation, float deltavalue)
{
	bool result;

	result = beginRender();

	result = result && modelRender(*m_Model, Coord(1,1,1), Coord(0,0,0), Coord(0,rotation,0));

	result = result && modelRender(*m_Model_2, Coord(0.5,0.5,0.5), Coord(2,0,0), Coord(rotation,-rotation,0));

	result = result && endRender();

	return result;

	//D3DXMATRIX rotationMatrix, translationMatrix, scaleMatrix, worldMatrix, viewMatrix, projectionMatrix;
	//bool result;


	//// Clear the buffers to begin the scene.
	//m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	//// Generate the view matrix based on the camera's position.
	//m_Camera->Render();

	//// Get the world, view, and projection matrices from the camera and d3d objects.
	//m_Camera->GetViewMatrix(viewMatrix);
	//m_D3D->GetWorldMatrix(worldMatrix);
	//translationMatrix = worldMatrix;
	//rotationMatrix = worldMatrix;
	//m_D3D->GetProjectionMatrix(projectionMatrix);

	//// Rotate the world matrix by the rotation value so that the model will spin.
	//D3DXMatrixIdentity(&worldMatrix);
	//D3DXMatrixRotationY(&worldMatrix, rotation);

	//// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_Model->Render(m_D3D->GetDeviceContext());

	//// Render the model using the light shader.
	//result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
	//							    m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), 
	//								m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), deltavalue, m_Model->GetTexture());
	//if(!result)
	//{
	//	return false;
	//}

	//// Rotate the world matrix by the rotation value so that the model will spin.
	//D3DXMatrixIdentity(&worldMatrix);
	//D3DXMatrixIdentity(&translationMatrix);
	//D3DXMatrixIdentity(&rotationMatrix);
	//D3DXMatrixIdentity(&scaleMatrix);
	//D3DXMatrixScaling(&scaleMatrix, 0.5, 0.5, 0.5);
	//D3DXMatrixTranslation(&translationMatrix, 2, 0, 0);
	//rotationMatrix = *D3DXMatrixRotationX(&rotationMatrix, rotation);
	//worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	//

	//// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_Model_2->Render(m_D3D->GetDeviceContext());
	//
	//// Render the model using the light shader.
	//result = result && m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model_2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
	//							    m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), 
	//								m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), deltavalue, m_Model_2->GetTexture());
	//if(!result)
	//{
	//	return false;
	//}

	//// Present the rendered scene to the screen.
	//m_D3D->EndScene();

	//return true;
}

bool GraphicsClass::beginRender()
{
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	return true;
}


bool GraphicsClass::endRender()
{
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::modelRender(ModelClass& to_render, Coord scale, Coord translate, Coord rotate)
{
	D3DXMATRIX scaleMatrix, translationMatrix, rotationMatrix;
	bool result;

	// Rotate the world matrix by the rotation value so that the model will spin.
	D3DXMatrixIdentity(&worldMatrix);
	D3DXMatrixScaling(&scaleMatrix, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&translationMatrix, translate.x, translate.y, translate.z);
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, rotate.y, rotate.x, rotate.z);
	worldMatrix = scaleMatrix * rotationMatrix * translationMatrix;
	
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	to_render.Render(m_D3D->GetDeviceContext());
	
	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), to_render.GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, 
								    m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), 
									m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), 0.0f, to_render.GetTexture());

	return result;
}