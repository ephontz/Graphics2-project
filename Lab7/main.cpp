// D3D LAB 1a "Line Land".
// Author: L.Norri CD DRX, FullSail University

// Introduction:
// Welcome to the first lab of the Direct3D Graphics Programming class.
// This is the ONLY guided lab in this course! 
// Future labs will demand the habits & foundation you develop right now!  
// It is CRITICAL that you follow each and every step. ESPECIALLY THE READING!!!

// TO BEGIN: Open the word document that acompanies this lab and start from the top.

//************************************************************
//************ INCLUDES & DEFINES ****************************
//************************************************************

#include <iostream>
#include <ctime>
#include "XTime.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include "DDSTextureLoader.h"
#include <thread>

using namespace DirectX;
using namespace std;




// BEGIN PART 1
// TODO: PART 1 STEP 1a
#include <d3d11.h>
// TODO: PART 1 STEP 1b
#include "MathHammer.h"
// TODO: PART 2 STEP 6
#include "Trivial_VS.csh"
#include "Trivial_PS.csh"
#include "PixelShader.csh"
#include "VertexShader.csh"
#include "VertexShader1.csh"
#include "GeometryShader.csh"
#define BACKBUFFER_WIDTH	1024
#define BACKBUFFER_HEIGHT	768
IDXGISwapChain * SwapChain;
ID3D11Device * Device;
ID3D11DeviceContext * Context;
ID3D11RenderTargetView *bBuffer;
ID3D11DepthStencilView * DSV;
D3D11_VIEWPORT ViewPort;
D3D11_VIEWPORT VP;

struct TO_SCENE
{
	Mat View = Ident();
	Mat Proj = Ident();
};


TO_SCENE scene;
//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************

class DEMO_APP
{	
public:
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	// TODO: PART 1 STEP 2

	
	
	ID3D11DeviceContext * defCon;
	ID3D11CommandList * list;


	

	ID3D11ShaderResourceView * skybox;
	ID3D11ShaderResourceView * texture;





	ID3D11Buffer * indexBuffer;
	D3D11_BUFFER_DESC ibDesc;

	ID3D11Buffer * indexBuffer2;
	D3D11_BUFFER_DESC ibDesc2;

	ID3D11Buffer * TO_SCENE_buffer;
	D3D11_BUFFER_DESC tsbDesc;

	ID3D11Buffer * TO_OBJECT_buffer;
	D3D11_BUFFER_DESC tobDesc;

	ID3D11Buffer * Dlight;
	D3D11_BUFFER_DESC dlDesc;

	ID3D11Buffer * Plight;
	D3D11_BUFFER_DESC plDesc;

	ID3D11Buffer * Slight;
	D3D11_BUFFER_DESC slDesc;


	// TODO: PART 2 STEP 2
	ID3D11Buffer * verts;
	unsigned int count = 360;
	D3D11_BUFFER_DESC bDesc;
	ID3D11InputLayout * iLay;
	ID3D11InputLayout * iLay2;
	ID3D11Buffer * p2q;
	D3D11_BUFFER_DESC p2qDesc;

	
	
	// BEGIN PART 5
	// TODO: PART 5 STEP 1
	
	// TODO: PART 2 STEP 4
	ID3D11VertexShader * VS;
	ID3D11VertexShader * VS2;
	ID3D11VertexShader * VS3;
	ID3D11GeometryShader * GS;
	ID3D11PixelShader * PS;
	ID3D11PixelShader * PS2;
	// BEGIN PART 3
	// TODO: PART 3 STEP 1
	ID3D11Buffer * buff2;
	D3D11_BUFFER_DESC buff2Desc;
	// TODO: PART 3 STEP 2b
	struct SEND_TO_VRAM
	{
		float constantColor[4];
		float constantOffset[2];
		float padding[2];
	};
	// TODO: PART 3 STEP 4a
	SEND_TO_VRAM toShader;


	struct TO_OBJECT
	{
		Mat world = Ident();
	};



	TO_OBJECT world;



	Mat cameraTransform = Ident();
public:
	// BEGIN PART 2
	// TODO: PART 2 STEP 1
	
	struct Simple_Vert
	{
		float x, y, z, w;
		float NormX, NormY, NormZ;
		float u, v;
	};

	struct StationLight
	{
		float x, y, z, w;
		float a, r, g, b;
	};
	

	struct LocalLight
	{
		float pos[4];
		float x, y, z, w;
		float a, r, g, b;
	};

	StationLight direct;
	LocalLight pointl;
	LocalLight spot;

	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();

	ID3D11RasterizerState * RState;
	D3D11_RASTERIZER_DESC rDesc;

	Simple_Vert circle[8];
	Simple_Vert gq[4];
	unsigned short index_array[36];
	unsigned short index_array2[6];
	ID3D11DepthStencilState * DSS;
	D3D11_DEPTH_STENCIL_DESC dssDesc;

	float check = 0;
	float move = .5;

};

void ThreadLoader(const wchar_t * path, ID3D11ShaderResourceView ** SRV, DEMO_APP * str)
{
	CreateDDSTextureFromFile(Device, path, NULL, SRV);
}

void ThreadDraw(DEMO_APP * app)
{

	app->defCon->OMSetRenderTargets(1, &bBuffer, NULL);
	
	app->defCon->RSSetViewports(1, &ViewPort);
	app->defCon->RSSetState(app->RState);

	app->direct.x += app->move;
	app->spot.pos[2] += (app->move/100.0f);
	app->spot.pos[1] -= (app->move / 100.0f);
	app->pointl.pos[2] -= (app->move);
	app->check += app->move;
	if (app->check >= 1000 || app->check <= -1000)
	{
 		app->move *= -1;
	}
	
	float color[4];
	color[0] = 0;
	color[1] = 0;
	color[2] = 0;
	color[3] = 1;
	app->defCon->ClearRenderTargetView(bBuffer, color);
	app->defCon->ClearDepthStencilView(DSV, NULL, 1, NULL);


	TO_SCENE temp;
	temp.Proj = scene.Proj;
	temp.View = scene.View;

	temp.View.mat[3][0] = 0;
	temp.View.mat[3][1] = 0;
	temp.View.mat[3][2] = 0;
	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));


	D3D11_MAPPED_SUBRESOURCE lightData;

	



	app->defCon->Map(app->TO_OBJECT_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &data);
	memcpy(data.pData, &app->world, sizeof(app->world));
	app->defCon->Unmap(app->TO_OBJECT_buffer, NULL);

	app->defCon->Map(app->TO_SCENE_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &data);
	memcpy(data.pData, &temp, sizeof(scene));
	app->defCon->Unmap(app->TO_SCENE_buffer, NULL);

	app->defCon->Map(app->Dlight, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &lightData);
	memcpy(lightData.pData, &app->direct, sizeof(app->direct));
	app->defCon->Unmap(app->Dlight, NULL);

	app->defCon->Map(app->Plight, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &lightData);
	memcpy(lightData.pData, &app->pointl, sizeof(app->pointl));
	app->defCon->Unmap(app->Plight, NULL);

	app->defCon->Map(app->Slight, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &lightData);
	memcpy(lightData.pData, &app->spot, sizeof(app->spot));
	app->defCon->Unmap(app->Slight, NULL);

	app->defCon->IASetInputLayout(app->iLay);

	app->defCon->VSSetConstantBuffers(1, 1, &app->TO_OBJECT_buffer);
	app->defCon->VSSetConstantBuffers(2, 1, &app->TO_SCENE_buffer);
	app->defCon->PSSetConstantBuffers(0, 1, &app->Dlight);
	app->defCon->PSSetConstantBuffers(1, 1, &app->Plight);
	app->defCon->PSSetConstantBuffers(2, 1, &app->Slight);
	app->defCon->IASetInputLayout(app->iLay2);
	app->defCon->GSSetConstantBuffers(1, 1, &app->TO_OBJECT_buffer);
	app->defCon->GSSetConstantBuffers(2, 1, &app->TO_SCENE_buffer);
	unsigned int size = sizeof(DEMO_APP::Simple_Vert);
	unsigned int offset = 0;
	app->defCon->IASetVertexBuffers(0, 1, &app->verts, &size, &offset);
	app->defCon->IASetIndexBuffer(app->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	app->defCon->VSSetShader(app->VS, NULL, NULL);
	app->defCon->PSSetShader(app->PS, NULL, NULL);
	app->defCon->OMSetDepthStencilState(app->DSS, 1);
	app->defCon->PSSetShaderResources(0, 1, &app->skybox);

	app->defCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	app->defCon->DrawIndexed(36, 0, 0);

	app->defCon->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH, 1.0, NULL);

	app->defCon->IASetVertexBuffers(0, 1, &app->buff2, &size, &offset);

	app->defCon->Map(app->TO_SCENE_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &data);
	memcpy(data.pData, &scene, sizeof(scene));
	app->defCon->Unmap(app->TO_SCENE_buffer, NULL);



	app->defCon->PSSetShader(app->PS2, NULL, NULL);
	app->defCon->PSSetShaderResources(1,1,&app->texture);
	app->defCon->VSSetShader(app->VS2, NULL, NULL);
	app->defCon->DrawIndexed(6,0,0);
	app->defCon->IASetInputLayout(app->iLay2);
	app->defCon->VSSetShader(app->VS3, NULL, NULL);
	app->defCon->GSSetShader(app->GS, NULL, NULL);
	app->defCon->IASetVertexBuffers(0, 1, &app->p2q, &size, &offset);
	app->defCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	app->defCon->Draw(1, 0);
	app->defCon->GSSetShader(NULL, NULL, NULL);
	app->defCon->RSSetViewports(1, &VP);

	app->defCon->RSSetState(app->RState);





	//app->defCon->ClearRenderTargetView(bBuffer, color);
	//app->defCon->ClearDepthStencilView(DSV, NULL, 1, NULL);


	
	temp.Proj = scene.Proj;
	temp.View = scene.View;

	temp.View.mat[3][0] = 0;
	temp.View.mat[3][1] = 0;
	temp.View.mat[3][2] = 0;
	temp.View = RotateY(temp.View, Degree_to_rad(180));
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));


	app->defCon->Map(app->TO_OBJECT_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &data);
	memcpy(data.pData, &app->world, sizeof(app->world));
	app->defCon->Unmap(app->TO_OBJECT_buffer, NULL);

	app->defCon->Map(app->TO_SCENE_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &data);
	memcpy(data.pData, &temp, sizeof(scene));
	app->defCon->Unmap(app->TO_SCENE_buffer, NULL);

	
	app->defCon->IASetInputLayout(app->iLay);

	app->defCon->VSSetConstantBuffers(1, 1, &app->TO_OBJECT_buffer);
	app->defCon->VSSetConstantBuffers(2, 1, &app->TO_SCENE_buffer);

	size = sizeof(DEMO_APP::Simple_Vert);
	offset = 0;
	app->defCon->IASetVertexBuffers(0, 1, &app->verts, &size, &offset);
	app->defCon->IASetIndexBuffer(app->indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	app->defCon->VSSetShader(app->VS, NULL, NULL);
	app->defCon->PSSetShader(app->PS, NULL, NULL);
	app->defCon->OMSetDepthStencilState(app->DSS, 1);
	app->defCon->PSSetShaderResources(0, 1, &app->skybox);

	app->defCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	app->defCon->DrawIndexed(36, 0, 0);

	app->defCon->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH, 1.0, NULL);
	app->defCon->PSSetShader(app->PS2, NULL, NULL);
	app->defCon->IASetVertexBuffers(0, 1, &app->buff2, &size, &offset);

	app->defCon->Map(app->TO_SCENE_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &data);
	memcpy(data.pData, &scene, sizeof(scene));
	app->defCon->Unmap(app->TO_SCENE_buffer, NULL);



	
	app->defCon->DrawIndexed(6, 0, 0);







	app->defCon->FinishCommandList(false, &app->list);
}



//************************************************************
//************ CREATION OF OBJECTS & RESOURCES ***************
//************************************************************

DEMO_APP::DEMO_APP(HINSTANCE hinst, WNDPROC proc)
{
	// ****************** BEGIN WARNING ***********************// 
	// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY! 
	application = hinst; 
	appWndProc = proc; 

	WNDCLASSEX  wndClass;
    ZeroMemory( &wndClass, sizeof( wndClass ) );
    wndClass.cbSize         = sizeof( WNDCLASSEX );             
    wndClass.lpfnWndProc    = appWndProc;						
    wndClass.lpszClassName  = L"DirectXApplication";            
	wndClass.hInstance      = application;		               
    wndClass.hCursor        = LoadCursor( NULL, IDC_ARROW );    
    wndClass.hbrBackground  = ( HBRUSH )( COLOR_WINDOWFRAME ); 
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
    RegisterClassEx( &wndClass );

	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	window = CreateWindow(	L"DirectXApplication", L"Lab 1a Line Land",	WS_OVERLAPPEDWINDOW, // & ~(WS_THICKFRAME|WS_MAXIMIZEBOX), 
							CW_USEDEFAULT, CW_USEDEFAULT, window_size.right-window_size.left, window_size.bottom-window_size.top,					
							NULL, NULL,	application, this );												

    ShowWindow( window, SW_SHOW );
	//********************* END WARNING ************************//



	// TODO: PART 1 STEP 3a
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.BufferCount = 1;
	desc.BufferDesc.Height = BACKBUFFER_HEIGHT;
	desc.BufferDesc.Width = BACKBUFFER_WIDTH;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.OutputWindow = window;
	desc.Windowed = true;
	desc.SampleDesc.Count = 4;
	// TODO: PART 1 STEP 3b
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,NULL, NULL, D3D11_SDK_VERSION, &desc,&SwapChain, &Device, NULL, &Context);
	Device->CreateDeferredContext(NULL, &defCon);
	// TODO: PART 1 STEP 4
	ID3D11Texture2D * Surface;
	SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&Surface));
	Device->CreateRenderTargetView(Surface, NULL, &bBuffer);
	Surface->Release();
	// TODO: PART 1 STEP 5
	ZeroMemory(&ViewPort, sizeof(D3D11_VIEWPORT));
	ViewPort.Height = BACKBUFFER_HEIGHT;
	ViewPort.Width = BACKBUFFER_WIDTH;
	ViewPort.MinDepth = 0;
	ViewPort.MaxDepth = 1;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;

	ZeroMemory(&VP, sizeof(D3D11_VIEWPORT));
	VP.Height = BACKBUFFER_HEIGHT/4;
	VP.Width = BACKBUFFER_WIDTH/4;
	VP.MinDepth = 0;
	VP.MaxDepth = 1;
	VP.TopLeftX = 0;
	VP.TopLeftY = 0;
	// TODO: PART 2 STEP 3a

	//cameraTransform.mat[3][2] = -10;
	//cameraTransform = RotateX(cameraTransform, Degree_to_rad(-10));
	scene.View = InverseDirty(cameraTransform);


	circle[0].x = cameraTransform.mat[3][0] - .5;
	circle[0].y = cameraTransform.mat[3][1] + .5;
	circle[0].z = cameraTransform.mat[3][2] - .5;

	circle[1].x = cameraTransform.mat[3][0] + .5;
	circle[1].y = cameraTransform.mat[3][1] + .5;
	circle[1].z = cameraTransform.mat[3][2] - .5;

	circle[2].x = cameraTransform.mat[3][0] - .5;
	circle[2].y = cameraTransform.mat[3][1] - .5;
	circle[2].z = cameraTransform.mat[3][2] - .5;

	circle[3].x = cameraTransform.mat[3][0] + .5;
	circle[3].y = cameraTransform.mat[3][1] - .5;
	circle[3].z = cameraTransform.mat[3][2] - .5;

	circle[4].x = cameraTransform.mat[3][0] - .5;
	circle[4].y = cameraTransform.mat[3][1] + .5;
	circle[4].z = cameraTransform.mat[3][2] + .5;

	circle[5].x = cameraTransform.mat[3][0] + .5;
	circle[5].y = cameraTransform.mat[3][1] + .5;
	circle[5].z = cameraTransform.mat[3][2] + .5;

	circle[6].x = cameraTransform.mat[3][0] - .5;
	circle[6].y = cameraTransform.mat[3][1] - .5;
	circle[6].z = cameraTransform.mat[3][2] + .5;

	circle[7].x = cameraTransform.mat[3][0] + .5;
	circle[7].y = cameraTransform.mat[3][1] - .5;
	circle[7].z = cameraTransform.mat[3][2] + .5;


	gq[0].x = -.5;
	gq[0].y = 0;
	gq[0].z = .5;

	gq[1].x = .5;
	gq[1].y = 0;
	gq[1].z = .5;

	gq[2].x = -.5;
	gq[2].y = 0;
	gq[2].z = -.5;

	gq[3].x = .5;
	gq[3].y = 0;
	gq[3].z = -.5;

	for (size_t i = 0; i < 4; i++)
	{
		gq[i].NormX = 0;
		gq[i].NormY = 1;
		gq[i].NormZ = 0;
	}

	gq[0].u = 0;
	gq[0].v = 0;

	gq[1].u = 1;
	gq[1].v = 0;

	gq[2].u = 0;
	gq[2].v = 1;

	gq[3].u = 1;
	gq[3].v = 1;

	index_array2[0] = 0;
	index_array2[0] = 1;
	index_array2[0] = 2;

	index_array2[0] = 1;
	index_array2[0] = 3;
	index_array2[0] = 2;

	index_array[0] = 0;
	index_array[1] = 3;
	index_array[2] = 1;

	index_array[3] = 0;
	index_array[4] = 2;
	index_array[5] = 3;

	index_array[6] = 1;
	index_array[7] = 7;
	index_array[8] = 5;

	index_array[9] = 1;
	index_array[10] = 3;
	index_array[11] = 7;

	index_array[12] = 4;
	index_array[13] = 5;
	index_array[14] = 7;

	index_array[15] = 4;
	index_array[16] = 7;
	index_array[17] = 6;

	index_array[18] = 0;
	index_array[19] = 4;
	index_array[20] = 6;

	index_array[21] = 0;
	index_array[22] = 6;
	index_array[23] = 2;

	index_array[24] = 2;
	index_array[25] = 6;
	index_array[26] = 7;

	index_array[27] = 2;
	index_array[28] = 7;
	index_array[29] = 3;

	index_array[30] = 0;
	index_array[31] = 1;
	index_array[32] = 5;

	index_array[33] = 0;
	index_array[34] = 5;
	index_array[35] = 4;
	
	direct.x = .5;
	direct.y = .5;
	direct.z = 0;

	direct.a = 1;
	direct.r = 0;
	direct.g = 0;
	direct.b = 1;



	pointl.a = 0;
	pointl.r = 1;
	pointl.g = 0;
	pointl.b = 1;

	pointl.pos[0] = .2;
	pointl.pos[1] = .2;
	pointl.pos[2] = 0;
	pointl.pos[3] = 0;


	spot.pos[0] = 0;
	spot.pos[1] = 1;
	spot.pos[2] = 0;
	
	
	
	spot.a = 1;
	spot.r = 1;
	spot.g = 1;
	spot.b = 1;
	
	spot.x = 0;
	spot.y = -1;
	spot.z = 0;



	// BEGIN PART 4
	// TODO: PART 4 STEP 1

	// TODO: PART 2 STEP 3b

	Simple_Vert pquad[1];
	pquad[0].x = 0;
	pquad[0].y = .7;
	pquad[0].z = 75;
		 
	pquad[0].NormX = 0;
	pquad[0].NormY = 0;
	pquad[0].NormZ = -1;


	float zNear = .1;
	float zFar = 10.0;

	


	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	
	Device->CreateDepthStencilState(&dssDesc, &DSS);
	//CreateDDSTextureFromFile(Device, L"skybox.dds", NULL, &skybox);
	thread thing(ThreadLoader, L"skybox.dds", &skybox, this);
	



	thread thing2(ThreadLoader, L"brownRoof_seamless.dds", &texture, this);



	scene.Proj.mat[1][1] = 1 / tan(Degree_to_rad(50));
	scene.Proj.mat[0][0] = scene.Proj.mat[1][1] * (BACKBUFFER_WIDTH / BACKBUFFER_HEIGHT);
	scene.Proj.mat[2][2] = (zFar - zNear) / zFar;
	scene.Proj.mat[2][3] = 1;
	scene.Proj.mat[3][3] = 0;
	scene.Proj.mat[3][2] = -(zFar * zNear) / (zFar - zNear);

	ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.CullMode = D3D11_CULL_NONE;
	rDesc.FrontCounterClockwise = false;
	rDesc.DepthClipEnable = false;
	rDesc.MultisampleEnable = true;
	rDesc.AntialiasedLineEnable = true;

	Device->CreateRasterizerState(&rDesc, &RState);
	
	ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));
	bDesc.ByteWidth = sizeof(Simple_Vert) * 8;
	bDesc.Usage = D3D11_USAGE_DYNAMIC;
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&p2qDesc, sizeof(D3D11_BUFFER_DESC));
	p2qDesc.ByteWidth = sizeof(Simple_Vert);
	p2qDesc.Usage = D3D11_USAGE_DYNAMIC;
	p2qDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	p2qDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;


	ZeroMemory(&tsbDesc, sizeof(D3D11_BUFFER_DESC));
	tsbDesc.ByteWidth = sizeof(TO_SCENE);
	tsbDesc.Usage = D3D11_USAGE_DYNAMIC;
	tsbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tsbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&tobDesc, sizeof(D3D11_BUFFER_DESC));
	tobDesc.ByteWidth = sizeof(TO_OBJECT);
	tobDesc.Usage = D3D11_USAGE_DYNAMIC;
	tobDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tobDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&ibDesc, sizeof(D3D11_BUFFER_DESC));
	ibDesc.ByteWidth = sizeof(unsigned int) * 36;
	ibDesc.Usage = D3D11_USAGE_IMMUTABLE;
	ibDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc.CPUAccessFlags = NULL;

	ZeroMemory(&ibDesc2, sizeof(D3D11_BUFFER_DESC));
	ibDesc2.ByteWidth = sizeof(unsigned int) * 6;
	ibDesc2.Usage = D3D11_USAGE_IMMUTABLE;
	ibDesc2.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibDesc2.CPUAccessFlags = NULL;

    // TODO: PART 2 STEP 3c
	D3D11_SUBRESOURCE_DATA data3;
	ZeroMemory(&data3, sizeof(D3D11_SUBRESOURCE_DATA));
	data3.pSysMem = circle;

	D3D11_SUBRESOURCE_DATA data_index;
	ZeroMemory(&data_index, sizeof(D3D11_SUBRESOURCE_DATA));
	data_index.pSysMem = index_array;

	D3D11_SUBRESOURCE_DATA data_index2;
	ZeroMemory(&data_index2, sizeof(D3D11_SUBRESOURCE_DATA));
	data_index2.pSysMem = index_array2;

	D3D11_SUBRESOURCE_DATA worldmat;
	ZeroMemory(&worldmat, sizeof(D3D11_SUBRESOURCE_DATA));
	//worldmat.pSysMem = world;
	D3D11_SUBRESOURCE_DATA scenemat;
	ZeroMemory(&scenemat, sizeof(D3D11_SUBRESOURCE_DATA));
	D3D11_SUBRESOURCE_DATA p2qdata;
	ZeroMemory(&p2qdata, sizeof(D3D11_SUBRESOURCE_DATA));
	p2qdata.pSysMem = pquad;
	//scenemat.pSysMem = scene;
	// TODO: PART 2 STEP 3d
	
	Device->CreateBuffer(&bDesc, &data3, &verts);
	Device->CreateBuffer(&ibDesc, &data_index, &indexBuffer);
	Device->CreateBuffer(&ibDesc2, &data_index2, &indexBuffer2);
	Device->CreateBuffer(&tsbDesc,NULL, &TO_SCENE_buffer);
	Device->CreateBuffer(&tobDesc,NULL, &TO_OBJECT_buffer);
	Device->CreateBuffer(&p2qDesc, &p2qdata, &p2q);
	// TODO: PART 5 STEP 2a
	
	// TODO: PART 5 STEP 2b
	
	// TODO: PART 5 STEP 3
		
	// TODO: PART 2 STEP 5
	// ADD SHADERS TO PROJECT, SET BUILD OPTIONS & COMPILE

	// TODO: PART 2 STEP 7


	Device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &VS);
	Device->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &PS);
	Device->CreatePixelShader(PixelShader, sizeof(PixelShader), NULL, &PS2);
	Device->CreateVertexShader(VertexShader, sizeof(VertexShader), NULL, &VS2);
	Device->CreateVertexShader(VertexShader1, sizeof(VertexShader1), NULL, &VS3);
	Device->CreateGeometryShader(GeometryShader, sizeof(GeometryShader), NULL, &GS);
	// TODO: PART 2 STEP 8a
	
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	
	

	// TODO: PART 2 STEP 8b
	Device->CreateInputLayout(layout, 3, Trivial_VS, sizeof(Trivial_VS), &iLay);
	Device->CreateInputLayout(layout, 3, VertexShader1, sizeof(VertexShader1), &iLay2);
	// TODO: PART 3 STEP 3
	ZeroMemory(&buff2Desc, sizeof(D3D11_BUFFER_DESC));
	buff2Desc.ByteWidth = sizeof(Simple_Vert) * 4;
	buff2Desc.Usage = D3D11_USAGE_IMMUTABLE;
	buff2Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff2Desc.CPUAccessFlags = NULL;

	D3D11_SUBRESOURCE_DATA buff2Data;
	ZeroMemory(&buff2Data, sizeof(D3D11_SUBRESOURCE_DATA));
	buff2Data.pSysMem = gq;



	Device->CreateBuffer(&buff2Desc, &buff2Data, &buff2);
	// TODO: PART 3 STEP 4b

//Depth Stencile stuff

	

	ID3D11Texture2D* DepthStencil = NULL;
	D3D11_TEXTURE2D_DESC DepthDesc;
	ZeroMemory(&DepthDesc, sizeof(D3D11_TEXTURE2D_DESC));


	DepthDesc.Width = BACKBUFFER_WIDTH;
	DepthDesc.Height = BACKBUFFER_HEIGHT;

	DepthDesc.MipLevels = 1;
	DepthDesc.ArraySize = 1;

	DepthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthDesc.SampleDesc.Count = 4;
	DepthDesc.SampleDesc.Quality = 0;

	DepthDesc.Usage = D3D11_USAGE_DEFAULT;
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	Device->CreateTexture2D(&DepthDesc, NULL, &DepthStencil);
	D3D11_DEPTH_STENCIL_DESC stencil;

	ZeroMemory(&stencil, sizeof(D3D11_DEPTH_STENCIL_DESC));
	/*
	stencil.DepthEnable = true;
	stencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	stencil.DepthFunc = D3D11_COMPARISON_LESS;

	stencil.StencilEnable = true;
	stencil.StencilReadMask = 0xFF;
	stencil.StencilWriteMask = 0xFF;

	stencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	stencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencil.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	stencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	stencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	stencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	stencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    

	ID3D11DepthStencilState * depthState;

	Device->CreateDepthStencilState(&stencil, &depthState);
	Context->OMSetDepthStencilState(depthState, 1);
	*/
	D3D11_DEPTH_STENCIL_VIEW_DESC DSVdesc;
	ZeroMemory(&DSVdesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	DSVdesc.Format = DXGI_FORMAT_D32_FLOAT;
	DSVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	DSVdesc.Texture2D.MipSlice = 0;

	Device->CreateDepthStencilView(DepthStencil, &DSVdesc, &DSV);
//	Context->OMSetRenderTargets(1, Surface, DSV);
	DepthStencil->Release();

	// lighting constant buffer
#pragma region

	ZeroMemory(&dlDesc, sizeof(D3D11_BUFFER_DESC));
	dlDesc.ByteWidth = sizeof(StationLight);
	dlDesc.Usage = D3D11_USAGE_DYNAMIC;
	dlDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dlDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&plDesc, sizeof(D3D11_BUFFER_DESC));
	plDesc.ByteWidth = sizeof(LocalLight);
	plDesc.Usage = D3D11_USAGE_DYNAMIC;
	plDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	plDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ZeroMemory(&slDesc, sizeof(D3D11_BUFFER_DESC));
	slDesc.ByteWidth = sizeof(LocalLight);
	slDesc.Usage = D3D11_USAGE_DYNAMIC;
	slDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	slDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA DirLight;
	ZeroMemory(&DirLight, sizeof(D3D11_SUBRESOURCE_DATA));

	D3D11_SUBRESOURCE_DATA PointLight;
	ZeroMemory(&PointLight, sizeof(D3D11_SUBRESOURCE_DATA));

	D3D11_SUBRESOURCE_DATA SpotLight;
	ZeroMemory(&SpotLight, sizeof(D3D11_SUBRESOURCE_DATA));

	Device->CreateBuffer(&dlDesc, NULL, &Dlight);
	Device->CreateBuffer(&plDesc, NULL, &Plight);
	Device->CreateBuffer(&slDesc, NULL, &Slight);
#pragma endregion



	thing2.join();
	thing.join();

}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{


 //input code
#pragma region
	if (GetAsyncKeyState(VK_UP))
	{
		scene.View = RotateX(scene.View, Deg2Rad(-.00001));
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		scene.View = RotateX(scene.View, Deg2Rad(.00001));
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		scene.View = RotateY(scene.View, Deg2Rad(-.00001));
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		scene.View = RotateY(scene.View, Deg2Rad(.00001));
	}
	if (GetAsyncKeyState('Q'))
	{
		scene.View = RotateZ(scene.View, Deg2Rad(-.00001));
	}
	if (GetAsyncKeyState('E'))
	{
		scene.View = RotateZ(scene.View, Deg2Rad(.00001));
	}
	if (GetAsyncKeyState('A'))
	{
		scene.View.mat[3][0] += .0005;
	}
	if (GetAsyncKeyState('D'))
	{
		scene.View.mat[3][0] -= .0005;
	}
	if (GetAsyncKeyState('W'))
	{
		scene.View.mat[3][2] -= .0005;
	}
	if (GetAsyncKeyState('S'))
	{
		scene.View.mat[3][2] += .0005;
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		scene.View.mat[3][1] -= .0005;
	}
	if (GetAsyncKeyState(VK_LCONTROL))
	{
		scene.View.mat[3][1] += .0005;
	}

#pragma endregion

	thread stuff(ThreadDraw, this);
	stuff.join();
	if (list)
	{
		Context->ExecuteCommandList(list, false);
		list->Release();
		list = nullptr;
	}
	
	// TODO: PART 1 STEP 8
	SwapChain->Present(NULL, NULL);
	// END OF PART 1
	return true; 
}

//************************************************************
//************ DESTRUCTION ***********************************
//************************************************************

bool DEMO_APP::ShutDown()
{
	// TODO: PART 1 STEP 6
	SwapChain->Release();
	bBuffer->Release();
	Context->Release();
	Device->Release();
	indexBuffer->Release();
	indexBuffer2->Release();
	TO_SCENE_buffer->Release();
	TO_OBJECT_buffer->Release();
	iLay->Release();
	VS->Release();
	PS->Release();
	buff2->Release();
	DSV->Release();
	RState->Release();
	verts->Release();
	DSS->Release();
	skybox->Release();
	PS2->Release();
	defCon->Release();
	Dlight->Release();
	Plight->Release();
	Slight->Release();
	texture->Release();
	VS2->Release();
	p2q->Release();
	VS3->Release();
	GS->Release();
	iLay2->Release();
	UnregisterClass( L"DirectXApplication", application ); 
	return true;
}

//************************************************************
//************ WINDOWS RELATED *******************************
//************************************************************

// ****************** BEGIN WARNING ***********************// 
// WINDOWS CODE, I DON'T TEACH THIS YOU MUST KNOW IT ALREADY!
	
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine,	int nCmdShow );						   
LRESULT CALLBACK WndProc(HWND hWnd,	UINT message, WPARAM wparam, LPARAM lparam );		
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE, LPTSTR, int )
{
	srand(unsigned int(time(0)));
	DEMO_APP myApp(hInstance,(WNDPROC)WndProc);	
    MSG msg; ZeroMemory( &msg, sizeof( msg ) );
    while ( msg.message != WM_QUIT && myApp.Run() )
    {	
	    if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        { 
            TranslateMessage( &msg );
            DispatchMessage( &msg ); 
        }
    }
	myApp.ShutDown(); 
	return 0; 
}
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    if(GetAsyncKeyState(VK_ESCAPE))
		message = WM_DESTROY;
    switch ( message )
    {
        case ( WM_DESTROY ): { PostQuitMessage( 0 ); }
        break;
		case(WM_SIZE) :
		{
			if (SwapChain)
			{
				Context->OMSetRenderTargets(0, 0, 0);
				Context->ClearState();
				bBuffer->Release();
				float width = LOWORD(lParam);
				float height = HIWORD(lParam);
				SwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, NULL);

				DXGI_SWAP_CHAIN_DESC newDesc;
				ZeroMemory(&newDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
				newDesc.SampleDesc.Count = 4;

				SwapChain->GetDesc(&newDesc);
				ID3D11Texture2D * text;

				SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&text));
				Device->CreateRenderTargetView(text, NULL, &bBuffer);
				text->Release();

				ID3D11Texture2D * dtext;
				DSV->Release();

				D3D11_TEXTURE2D_DESC DepthDesc;
				ZeroMemory(&DepthDesc, sizeof(D3D11_TEXTURE2D_DESC));


				DepthDesc.Width = width;
				DepthDesc.Height = height;

				DepthDesc.MipLevels = 1;
				DepthDesc.ArraySize = 1;

				DepthDesc.Format = DXGI_FORMAT_D32_FLOAT;
				DepthDesc.SampleDesc.Count = 4;
				DepthDesc.SampleDesc.Quality = 0;

				DepthDesc.Usage = D3D11_USAGE_DEFAULT;
				DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				Device->CreateTexture2D(&DepthDesc, NULL, &dtext);

				D3D11_DEPTH_STENCIL_VIEW_DESC DSVdesc;
				ZeroMemory(&DSVdesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
				DSVdesc.Format = DXGI_FORMAT_D32_FLOAT;
				DSVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				DSVdesc.Texture2D.MipSlice = 0;

				Device->CreateDepthStencilView(dtext, &DSVdesc, &DSV);

				dtext->Release();

				Context->OMSetRenderTargets(1, &bBuffer, NULL);

				ViewPort.Height = height;
				ViewPort.Width = width;
				ViewPort.MinDepth = 0;
				ViewPort.MaxDepth = 1;
				ViewPort.TopLeftX = 0;
				ViewPort.TopLeftY = 0;

				Context->RSSetViewports(1, &ViewPort);

				VP.Height = height/4;
				VP.Width = width/4;
				VP.MinDepth = 0;
				VP.MaxDepth = 1;
				VP.TopLeftX = 0;
				VP.TopLeftY = 0;

				Context->RSSetViewports(1, &VP);

				float zFar = 10.0;
				float zNear = .1;
				scene.Proj.mat[1][1] = 1 / tan(Degree_to_rad(50));
				scene.Proj.mat[0][0] = scene.Proj.mat[1][1] / (width / height);
				scene.Proj.mat[2][2] = (zFar - zNear) / zFar;
				scene.Proj.mat[2][3] = 1;
				scene.Proj.mat[3][3] = 0;
				scene.Proj.mat[3][2] = -(zFar * zNear) / (zFar - zNear);

				scene.View = InverseDirty(scene.View);

			}
		}
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}
//********************* END WARNING ************************//