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
#define BACKBUFFER_WIDTH	500
#define BACKBUFFER_HEIGHT	500

//************************************************************
//************ SIMPLE WINDOWS APP CLASS **********************
//************************************************************

class DEMO_APP
{	
	HINSTANCE						application;
	WNDPROC							appWndProc;
	HWND							window;
	// TODO: PART 1 STEP 2
	ID3D11Device * Device;
	IDXGISwapChain * SwapChain;
	ID3D11DeviceContext * Context;
	ID3D11RenderTargetView *bBuffer;
	D3D11_VIEWPORT ViewPort;


	// TODO: PART 2 STEP 2
	ID3D11Buffer * verts;
	unsigned int count = 360;
	D3D11_BUFFER_DESC bDesc;
	ID3D11InputLayout * iLay;
	
	
	// BEGIN PART 5
	// TODO: PART 5 STEP 1
	
	// TODO: PART 2 STEP 4
	ID3D11VertexShader * VS;
	ID3D11PixelShader * PS;
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
	ID3D11DepthStencilView * DSV;
public:
	// BEGIN PART 2
	// TODO: PART 2 STEP 1
	
	struct Simple_Vert
	{
		float x, y, z, w;
	};
	
	DEMO_APP(HINSTANCE hinst, WNDPROC proc);
	bool Run();
	bool ShutDown();
};

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

	window = CreateWindow(	L"DirectXApplication", L"Lab 1a Line Land",	WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME|WS_MAXIMIZEBOX), 
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
	desc.SampleDesc.Count = 1;
	// TODO: PART 1 STEP 3b
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,NULL, NULL, D3D11_SDK_VERSION, &desc,&SwapChain, &Device, NULL, &Context);
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
	// TODO: PART 2 STEP 3a
	Simple_Vert circle[3];
	
	circle[0].x = -.5;
	circle[0].y = 0;
	circle[0].z = 0;
	circle[0].w = 0;

	circle[1].x = .5;
	circle[1].y = 0;
	circle[1].z = 0;
	circle[1].w = 0;

	circle[2].x = 0;
	circle[2].y = -.5;
	circle[2].z = 0;
	circle[2].w = 0;

	

	// BEGIN PART 4
	// TODO: PART 4 STEP 1

	// TODO: PART 2 STEP 3b
	
	ZeroMemory(&bDesc, sizeof(D3D11_BUFFER_DESC));
	bDesc.ByteWidth = sizeof(Simple_Vert) * 3;
	bDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bDesc.CPUAccessFlags = NULL;

    // TODO: PART 2 STEP 3c
	D3D11_SUBRESOURCE_DATA data3;
	ZeroMemory(&data3, sizeof(D3D11_SUBRESOURCE_DATA));
	data3.pSysMem = circle;
	// TODO: PART 2 STEP 3d
	Device->CreateBuffer(&bDesc, &data3, &verts);
	// TODO: PART 5 STEP 2a
	
	// TODO: PART 5 STEP 2b
	
	// TODO: PART 5 STEP 3
		
	// TODO: PART 2 STEP 5
	// ADD SHADERS TO PROJECT, SET BUILD OPTIONS & COMPILE

	// TODO: PART 2 STEP 7


	Device->CreateVertexShader(Trivial_VS, sizeof(Trivial_VS), NULL, &VS);
	Device->CreatePixelShader(Trivial_PS, sizeof(Trivial_PS), NULL, &PS);
	// TODO: PART 2 STEP 8a
	
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	

	// TODO: PART 2 STEP 8b
	Device->CreateInputLayout(layout, 1, Trivial_VS, sizeof(Trivial_VS), &iLay);
	// TODO: PART 3 STEP 3
	ZeroMemory(&buff2Desc, sizeof(D3D11_BUFFER_DESC));
	buff2Desc.ByteWidth = sizeof(toShader);
	buff2Desc.Usage = D3D11_USAGE_DYNAMIC;
	buff2Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buff2Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	Device->CreateBuffer(&buff2Desc, NULL, &buff2);
	// TODO: PART 3 STEP 4b
	toShader.constantColor[0] = 1.0f;
	toShader.constantColor[1] = 1.0f;
	toShader.constantColor[2] = 0.0f;
	toShader.constantColor[3] = 1.0f;

	toShader.constantOffset[0] = 0.0f;
	toShader.constantOffset[1] = 0.0f;




	ID3D11Texture2D* DepthStencil = NULL;
	D3D11_TEXTURE2D_DESC DepthDesc;
	ZeroMemory(&DepthDesc, sizeof(D3D11_TEXTURE2D_DESC));


	DepthDesc.Width = BACKBUFFER_WIDTH;
	DepthDesc.Height = BACKBUFFER_HEIGHT;

	DepthDesc.MipLevels = 1;
	DepthDesc.ArraySize = 1;

	DepthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	DepthDesc.SampleDesc.Count = 1;
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
	DSVdesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVdesc.Texture2D.MipSlice = 0;

	Device->CreateDepthStencilView(DepthStencil, &DSVdesc, &DSV);
//	Context->OMSetRenderTargets(1, Surface, DSV);
















}

//************************************************************
//************ EXECUTION *************************************
//************************************************************

bool DEMO_APP::Run()
{
	// TODO: PART 4 STEP 2	
	
	// TODO: PART 4 STEP 3
	
	// TODO: PART 4 STEP 5
	
	// END PART 4

	// TODO: PART 1 STEP 7a
	Context->OMSetRenderTargets(1, &bBuffer, NULL);
	// TODO: PART 1 STEP 7b
	Context->RSSetViewports(1, &ViewPort);
	// TODO: PART 1 STEP 7c
	float color[4];
	color[0] = 0;
	color[1] = 0;
	color[2] = 1;
	color[3] = 1;
	Context->ClearRenderTargetView(bBuffer, color);
	Context->ClearDepthStencilView(DSV, NULL, 1, NULL);
	// TODO: PART 5 STEP 4
	
	// TODO: PART 5 STEP 5
	
	// TODO: PART 5 STEP 6
	
	// TODO: PART 5 STEP 7
	
	// END PART 5
	
	// TODO: PART 3 STEP 5
	D3D11_MAPPED_SUBRESOURCE data;
	ZeroMemory(&data, sizeof(D3D11_MAPPED_SUBRESOURCE));
	Context->Map(buff2, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &data);
	memcpy(data.pData, &toShader, sizeof(toShader));
	Context->Unmap(buff2, NULL);
	
	// TODO: PART 3 STEP 6
	Context->VSSetConstantBuffers(0, 1, &buff2);
	// TODO: PART 2 STEP 9a
	unsigned int size = sizeof(Simple_Vert);
	unsigned int offset = 0;
	Context->IASetVertexBuffers(0, 1, &verts,&size , &offset);
	// TODO: PART 2 STEP 9b
	Context->VSSetShader(VS, NULL, NULL);
	Context->PSSetShader(PS, NULL, NULL);
	// TODO: PART 2 STEP 9c
	Context->IASetInputLayout(iLay);
	// TODO: PART 2 STEP 9d
	Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// TODO: PART 2 STEP 10
	Context->Draw(3, 0);
	// END PART 2

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
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}
//********************* END WARNING ************************//