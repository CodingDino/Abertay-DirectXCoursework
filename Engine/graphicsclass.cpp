// Solar Exploration Sim
// Developed for DirectX Coursework for Abertay University
// Copyright Sarah Herzog, 2011, all rights reserved.
//
// GraphicsClass
//		Overall control of graphics and rendering, holds model objects and other graphics objects


// |----------------------------------------------------------------------------|
// |								Includes									|
// |----------------------------------------------------------------------------|
#include "graphicsclass.h"


// |----------------------------------------------------------------------------|
// |						   Default Constructor								|
// |----------------------------------------------------------------------------|
GraphicsClass::GraphicsClass() :
	m_D3D(0),
	m_Camera(0),
	m_LightShader(0),
	m_TextureShader(0),
	m_Light(0),
	m_Models(0),
	crosshairs(0),
	HUD(0),
	m_Text(0),
	m_sun(0)
{
}

	
// |----------------------------------------------------------------------------|
// |						    Copy Constructor								|
// |----------------------------------------------------------------------------|
GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


// |----------------------------------------------------------------------------|
// |						     Deconstructor									|
// |----------------------------------------------------------------------------|
GraphicsClass::~GraphicsClass()
{
}


// |----------------------------------------------------------------------------|
// |						      Initialize									|
// |----------------------------------------------------------------------------|
bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Record screen dimmensions
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

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

	// Initialize a base view matrix with the camera for 2D user interface rendering.
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the model objects.
	m_Models = new ModelClass*[NUM_MODELS];
	for (int i=0; i<NUM_MODELS; ++i)
	{
		m_Models[i] = 0;
	}
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
			result = m_Models[i]->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/earth02.jpg");
			if(!result)
			{
				MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
				return false;
			}
		}
	}

	// Create and initialize planets
	m_sun = new PlanetClass;
	if(!m_sun) return false;
	result = m_sun->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/star_sol.png",10.0f, 1.0f, Coord(0,0,0), 1.0f, 
		0.0f, Coord(0,0,0), Coord(0,0,0));

	// Create the shader objects.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader)
	{
		return false;
	}
	m_TextureShader = new TextureShaderClass;
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
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
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
	m_Light->SetDiffuseColor(1.0f, 1.0f, 0.75f, 1.0f);
	m_Light->SetDirection(1.0f, -1.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(16.0f);

	// Create the bitmap objects.
	crosshairs = new BitmapClass;
	if(!crosshairs)
	{
		return false;
	}
	HUD = new BitmapClass;
	if(!HUD)
	{
		return false;
	}

	// Initialize the bitmap objects.
	result = crosshairs->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/crosshair.png", 60, 50);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the crosshairs object.", L"Error", MB_OK);
		return false;
	}
	result = HUD->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"../Engine/data/hud02.png", screenWidth, screenHeight);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the HUD object.", L"Error", MB_OK);
		return false;
	}

	// Create the text object.
	m_Text = new TextClass;
	if(!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, baseViewMatrix);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}


	return true;
}


// |----------------------------------------------------------------------------|
// |						      Shutdown										|
// |----------------------------------------------------------------------------|
void GraphicsClass::Shutdown()
{
	// Release the planets.
	if(m_sun)
	{
		m_sun->Shutdown();
		delete m_sun;
		m_sun = 0;
	}

	// Release the text object.
	if(m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = 0;
	}

	// Release the bitmap objects.
	if(crosshairs)
	{
		crosshairs->Shutdown();
		delete crosshairs;
		crosshairs = 0;
	}
	if(HUD)
	{
		HUD->Shutdown();
		delete HUD;
		HUD = 0;
	}

	// Release the light object.
	if(m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the shader objects.
	if(m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
	if(m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
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


// |----------------------------------------------------------------------------|
// |						       Frame										|
// |----------------------------------------------------------------------------|
bool GraphicsClass::Frame(int mouseX, int mouseY, int fps, int cpu, float frameTime,
	Coord camera_rotation, Coord camera_position)
{
	bool result;

	// Set the position of the camera.
	m_Camera->SetPosition(camera_position.x, camera_position.y, camera_position.z-10.0f);

	// Set the rotation of the camera.
	m_Camera->SetRotation(camera_rotation.x, camera_rotation.y, camera_rotation.z);

	// Update planets
	m_sun->Frame(frameTime);
	
	// Render the graphics scene.
	result = Render(mouseX, mouseY);
	if(!result)
	{
		return false;
	}

	// Set the frames per second.
	result = m_Text->SetFps(fps, m_D3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}

	// Set the cpu usage.
	result = m_Text->SetCpu(cpu, m_D3D->GetDeviceContext());
	if(!result)
	{
		return false;
	}


	return true;
}


// |----------------------------------------------------------------------------|
// |						      Render										|
// |----------------------------------------------------------------------------|
bool GraphicsClass::Render(int mouseX, int mouseY)
{
	bool result;
	static float rotation = 0.0f;

	// Update the rotation variable each call.
	// TODO: Remove (this is just for the rotation to start out)
	rotation += (float)D3DX_PI * 0.01f;
	if(rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	result = BeginRender();

	// MODEL rendering
	float scale(1), translate(0);
	if(m_Models)
	{
		for (int i=0; i<NUM_MODELS; ++i)
		{
			if(m_Models[i])
			{
				result = result && ModelRender(*m_Models[i], Coord(scale,scale,scale), Coord(-6 + translate,0,0), Coord(0,rotation,0));
				translate += 3;
			}
		}
	}
	translate = 0;
	if(m_Models)
	{
		for (int i=0; i<NUM_MODELS; ++i)
		{
			if(m_Models[i])
			{
				result = result && ModelRender(*m_Models[i], Coord(scale,scale,scale), Coord(-6 + translate,3,0), Coord(0,rotation,0));
				translate += 3;
			}
		}
	}
	translate = 0;
	if(m_Models)
	{
		for (int i=0; i<NUM_MODELS; ++i)
		{
			if(m_Models[i])
			{
				result = result && ModelRender(*m_Models[i], Coord(scale,scale,scale), Coord(-6 + translate,-3,0), Coord(0,rotation,0));
				translate += 3;
			}
		}
	}

	// Render planets
	ModelClass* planet_model(0);
	m_sun->GetModel(planet_model);
	result = result && ModelRender(*planet_model, m_sun->GetScale(), m_sun->GetTranslate(), m_sun->GetRotate());

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();

	// BITMAP rendering
	result = result && BitmapRender(*crosshairs, mouseX-30, mouseY-25);
	result = result && BitmapRender(*HUD, 0, 0);

	// TEXT rendering
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if(!result)
	{
		return false;
	}

	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Perform closing actions
	result = result && EndRender();

	return result;
}


// |----------------------------------------------------------------------------|
// |						    BeginRender										|
// |----------------------------------------------------------------------------|
bool GraphicsClass::BeginRender()
{
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	return true;
}


// |----------------------------------------------------------------------------|
// |						    EndRender										|
// |----------------------------------------------------------------------------|
bool GraphicsClass::EndRender()
{
	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}


// |----------------------------------------------------------------------------|
// |						    ModelRender										|
// |----------------------------------------------------------------------------|
bool GraphicsClass::ModelRender(ModelClass& to_render, Coord scale, Coord translate, Coord rotate)
{
	D3DXMATRIX scaleMatrix, translationMatrix, rotationMatrix;
	bool result = true;

	// Modify the world matrix as needed.
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
									m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), to_render.GetTexture());

	return result;
}



// |----------------------------------------------------------------------------|
// |						    BitmapRender									|
// |----------------------------------------------------------------------------|
bool GraphicsClass::BitmapRender(BitmapClass& to_render, int x, int y)
{
	bool result;

	// Change world matrix to identity before rendering.
	D3DXMatrixIdentity(&worldMatrix);
	
	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = to_render.Render(m_D3D->GetDeviceContext(), x, y);
	if(!result)
	{
		return false;
	}
	// Render the bitmap with the texture shader.
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), to_render.GetIndexCount(), 
		worldMatrix, baseViewMatrix, orthoMatrix, to_render.GetTexture());
	if(!result)
	{
		return false;
	}

	return result;
}