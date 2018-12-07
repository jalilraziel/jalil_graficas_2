
#include <windows.h>

//#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>
#include <d3dcompiler.h>
#include <ResourceIndexer.h>
//#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "d3dx11.lib")
//#pragma comment (lib, "d3dx10.lib")
//#include <xnamath.h>
struct simplevertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct ConstantBuffer
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

HINSTANCE g_hInst;
HWND g_hWnd;
D3D_DRIVER_TYPE         g_driverType = D3D_DRIVER_TYPE_NULL;
D3D_FEATURE_LEVEL       g_featureLevel = D3D_FEATURE_LEVEL_11_0;
ID3D11Device*           g_pd3dDevice;
ID3D11DeviceContext*    g_pImmediateContext;
IDXGISwapChain*         g_pSwapChain;
ID3D11RenderTargetView* g_pRenderTargetView;
ID3D11InputLayout*      g_pVertexLayout;
ID3D11Buffer*           g_pVertexBuffer;
ID3D11Buffer*           g_pIndexBuffer;
ID3D11PixelShader*      g_pPixelShader;
ID3D11VertexShader*     g_pVertexShader;
ID3D11Buffer*           g_pConstantBuffer;
XMMATRIX                g_World;
XMMATRIX                g_View;
XMMATRIX                g_Projection;

LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
HRESULT InitDev();
void Render();

int WINAPI wWinMain(HINSTANCE hInstan, HINSTANCE hPrevInstan, LPWSTR lpCmdLine, int nCmdShow)
{
	InitWindow(hInstan, nCmdShow);
	InitDev();
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Render();  // Do some rendering
		}
	}
}

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX Window_V;   // declarando la classe ventana
	Window_V.cbSize = sizeof(WNDCLASSEX);
	Window_V.style = CS_HREDRAW | CS_VREDRAW;
	Window_V.lpfnWndProc = WndProc;
	Window_V.cbClsExtra = 0;
	Window_V.cbWndExtra = 0;
	Window_V.hInstance = hInstance;
	Window_V.hIcon = LoadIcon(hInstance, NULL);
	Window_V.hCursor = LoadCursor(NULL, IDC_ARROW);
	Window_V.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	Window_V.lpszMenuName = NULL;
	Window_V.lpszClassName = L"WindowClass";
	Window_V.hIconSm = LoadIcon(Window_V.hInstance, NULL);

	if (!RegisterClassEx(&Window_V)) return FALSE;   // damos de alta la clase ventena

	g_hInst = hInstance;
	RECT Size_W = { 0, 0, 720, 480 };
	AdjustWindowRect(&Size_W, WS_OVERLAPPEDWINDOW, FALSE);
	g_hWnd = CreateWindow(L"WindowClass", L"Venta de jalil - graficas 2", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Size_W.right - Size_W.left, Size_W.bottom - Size_W.top, NULL, NULL, hInstance, NULL);

	if (!g_hWnd) return FALSE;

	ShowWindow(g_hWnd, nCmdShow); // mostramos la ventana
	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT Paint_S;
	HDC hdc;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &Paint_S);
		EndPaint(hWnd, &Paint_S);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HRESULT InitDev(){
	HRESULT hr = S_OK;

	D3D_FEATURE_LEVEL featureLevels[] =	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = 720;
	sd.BufferDesc.Height = 480;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, NULL, &g_pImmediateContext))) return false;

	ID3D11Texture2D *pBackBuffer;
	if (FAILED(g_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer))) return FALSE;
	hr = g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr)) return hr;

	g_pImmediateContext->OMSetRenderTargets(1, &g_pRenderTargetView, NULL);
	
	D3D11_VIEWPORT vp;
	vp.Width = 720;
	vp.Height = 480;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	g_pImmediateContext->RSSetViewports(1, &vp);

	D3D11_INPUT_ELEMENT_DESC layout[] = {	
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	ID3DBlob* pVSBlob;
	ID3DBlob* pPSBlob;

	hr = D3DX11CompileFromFile(L"shader.shader", 0, 0, "VS", "vs_4_0", 0, 0, 0, &pVSBlob, 0, 0);
	hr = D3DX11CompileFromFile(L"shader.shader", 0, 0, "PS", "ps_4_0", 0, 0, 0, &pPSBlob, 0, 0);

	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
	
	g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout);
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

	simplevertex vertices[] =
	{
	
		{ XMFLOAT3(-1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f,  1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f,  1.0f,  1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f,  1.0f,  1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f,  1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f,  1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },

	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(simplevertex) * 8;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;
	g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);

	UINT stride = sizeof(simplevertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);


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
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = indices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);

	g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstantBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);

	g_World = XMMatrixIdentity();

	XMVECTOR Eye = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	g_View = XMMatrixLookAtLH(Eye, At, Up);

	g_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, 720 / (FLOAT)480, 0.01f, 100.0f);
}



void Render()
{
	static float t = 0.0f;
	if (g_driverType == D3D_DRIVER_TYPE_REFERENCE){
		t += (float)XM_PI * 0.0125f;
	}
	else{
		static DWORD dwTimeStart = 0;
		DWORD dwTimeCur = GetTickCount();
		if (dwTimeStart == 0)
			dwTimeStart = dwTimeCur;
		t = (dwTimeCur - dwTimeStart) / 1000.0f;
	}
	g_World = XMMatrixRotationY(t);

	g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, D3DXCOLOR(.208f, .368f, .231f, 1.0));

	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);
	cb.mProjection = XMMatrixTranspose(g_Projection);
	g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
	g_pImmediateContext->DrawIndexed(36, 0, 0);


	g_pSwapChain->Present(0, 0);
}