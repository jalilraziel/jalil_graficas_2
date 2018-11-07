
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#include <xnamath.h>
     
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600   

ID3D11Device *dev;                     // puntero al dispositico del direcx_3D
ID3D11DeviceContext *devcon;           // puntero al contexto del dispositivo Direct3D
ID3D11RenderTargetView *pbackbuffer;    // puntero a la direccion del backbuffer
IDXGISwapChain *p_cadena;              // puntero a la interfaz de swapchain
ID3D11VertexShader *pvertex_S;         // puntero a la direccion del vertexshader
ID3D11PixelShader *ppixel_S;           // puntero a la direccion del pixelshader
ID3D11Buffer *pVertex_B;               // puntero a la direccion del vertexbuffer
ID3D11InputLayout *pLayout;            // puntero al diseño de entrada
ID3D11Buffer* pIndex_B;
ID3D11Buffer* Constant_B;
XMMATRIX g_World;
XMMATRIX g_View;
XMMATRIX g_Projection;
D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void Inicio_DXD3(HWND hWnd);
void RenderFrame(void);
void Limpiar_DXD3(void);
void Creando_Graphics(void);
void Creando_Pipeline(void);

struct Vertices{
	FLOAT X, Y, Z;
	D3DXCOLOR Color;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX My_Ventana;
	ZeroMemory(&My_Ventana, sizeof(WNDCLASSEX));
	
	My_Ventana.cbSize = sizeof(WNDCLASSEX);
	My_Ventana.style = CS_HREDRAW | CS_VREDRAW;
	My_Ventana.lpfnWndProc = WindowProc;
	My_Ventana.hInstance = hInstance;
	My_Ventana.hCursor = LoadCursor(NULL, IDC_ARROW);
	My_Ventana.lpszClassName = L"ClaseVentana1";

	RegisterClassEx(&My_Ventana);
	RECT w_t = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&w_t, WS_OVERLAPPEDWINDOW, FALSE);
	hWnd = CreateWindowEx( NULL, L"ClaseVentana1", L"proyecto 1", WS_OVERLAPPEDWINDOW, 300, 300, w_t.right-w_t.left, w_t.bottom-w_t.top, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	
	Inicio_DXD3(hWnd);
	MSG msg;
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		RenderFrame();
	}
	Limpiar_DXD3();
	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Inicio_DXD3(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC cad_interc;
	ZeroMemory(&cad_interc, sizeof(DXGI_SWAP_CHAIN_DESC));

	cad_interc.BufferCount = 1;
	cad_interc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	cad_interc.BufferDesc.Width = SCREEN_WIDTH;
	cad_interc.BufferDesc.Height = SCREEN_HEIGHT;
	cad_interc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	cad_interc.OutputWindow = hWnd;
	cad_interc.SampleDesc.Count = 4;
	cad_interc.Windowed = TRUE;
	cad_interc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
											
	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &cad_interc, &p_cadena, &dev, NULL, &devcon);

	ID3D11Texture2D *back_B;
	p_cadena->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_B);
	dev->CreateRenderTargetView(back_B, NULL, &pbackbuffer);
	back_B->Release();
	devcon->OMSetRenderTargets(1, &pbackbuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	devcon->RSSetViewports(1, &viewport);

	Creando_Pipeline();
	Creando_Graphics();
}

void RenderFrame(void)
{
	static float t = 0.0f;
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
	{
		t += (float)XM_PI * 0.0125f;
	}
	else
	{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		t = (dwTimeCur - dwTimeStart) / 1000.0f;
	}

	g_World = XMMatrixRotationY(t);

	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	devcon->ClearRenderTargetView(pbackbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);
	cb.mProjection = XMMatrixTranspose(g_Projection);
	devcon->UpdateSubresource(Constant_B, 0, NULL, &cb, 0, 0);
	
	UINT size = sizeof(Vertices);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVertex_B, &size, &offset);
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devcon->Draw(3, 0);
	p_cadena->Present(0, 0);
}

void Limpiar_DXD3(void)
{
	p_cadena->SetFullscreenState(FALSE, NULL);

	pLayout->Release();
	pvertex_S->Release();
	ppixel_S->Release();
	pVertex_B->Release();
	pIndex_B->Release();
	p_cadena->Release();
	pbackbuffer->Release();
	dev->Release();
	devcon->Release();
}

void Creando_Graphics(void)
{
	Vertices traingulo[] =
	{
		//{ 0.5f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) },
		//{ 0.5f, -0.5, 0.0f,  D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f) },
		//{ -0.5f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f) },
		{-1.0f, 1.0f, -1.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)},
		{1.0f, 1.0f, -1.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
		{1.0f, 1.0f, 1.0f, D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f)},
		{-1.0f, 1.0f, 1.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{-1.0f, -1.0f, -1.0f, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f)},
		{1.0f, -1.0f, -1.0f, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)},
		{1.0f, -1.0f, 1.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)},
		{-1.0f, -1.0f, 1.0f, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)},
	};
	WORD indices[] =
	{
		3,1,0,
		2,1,3,

		0,5,4,
		1,5,0,

		3,4,7,
		0,4,3,

		1,6,5,
		2,6,1,

		2,7,6,
		3,7,2,

		6,4,5,
		7,4,6,
	};


	D3D11_BUFFER_DESC buffer_d;
	ZeroMemory(&buffer_d, sizeof(buffer_d));

	buffer_d.Usage = D3D11_USAGE_DEFAULT;
	buffer_d.ByteWidth = sizeof(Vertices) * 36;
	buffer_d.BindFlags = D3D11_USAGE_DEFAULT;
	buffer_d.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_d.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	dev->CreateBuffer(&buffer_d, &InitData, &pIndex_B);
	devcon->IASetIndexBuffer(pIndex_B, DXGI_FORMAT_R16_UINT, 0);


	//D3D11_MAPPED_SUBRESOURCE map_S;
	//devcon->Map(pVertex_B, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &map_S);
	//memcpy(map_S.pData, traingulo, sizeof(traingulo));
	//devcon->Unmap(pVertex_B, NULL);
}


void Creando_Pipeline(void)
{
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pvertex_S);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &ppixel_S);

	devcon->IASetIndexBuffer(pIndex_B, DXGI_FORMAT_R16_UINT, 0);
	devcon->VSSetShader(pvertex_S, 0, 0);
	devcon->PSSetShader(ppixel_S, 0, 0);


	D3D11_INPUT_ELEMENT_DESC input_E_D[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	dev->CreateInputLayout(input_E_D, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);
}
