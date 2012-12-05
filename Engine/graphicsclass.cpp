////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_Light = 0;

	m_Models = new ModelClass*[NUM_MODELS];
	for (int i=0; i<NUM_MODELS; ++i)
	{
		m_Models[i] = 0;
	}
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
	if(m_Models)
	{
		for (int i=0; i<NUM_MODELS; ++i)
		{
			m_Models[i] = new ModelClass;
			if(!m_Models[i])
			{
				return false;
			}	
		}
	}

	// Initialize the model object.
	debug("GraphicsClass: Initializing dynamic models...");
	if(m_Models)
	{
		for (int i=0; i<NUM_MODELS; ++i)
		{
			result = m_Models[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/seafloor.dds");
			if(!result)
			{
				MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
				return false;
			}
		}
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

	// Release the model objects.
	if(m_Models)
	{
		for (int i=0; i<NUM_MODELS; ++i)
		{
			if(m_Models[i])
			{
				m_Models[i]->Shutdown();
				delete m_Models[i];
				m_Models[i] = 0;
			}
		}
		delete[] m_Models;
		m_Models=0;
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

	// render the models
	float scale(1), translate(0);
	if(m_Models)
	{
		for (int i=0; i<NUM_MODELS; ++i)
		{
			if(m_Models[i])
			{
				result = result && modelRender(*m_Models[i], Coord(scale,scale,scale), Coord(-5 + translate,0,0), Coord(0,rotation,0));
				scale *= 0.75;
				translate += 2;
			}
		}
	}

	result = result && endRender();

	return result;
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