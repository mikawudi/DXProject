#include<Windows.h>
#include <d3d11.h>
#include <xnamath.h>
#include <d3dcompiler.h>
#include <D3DX11async.h>
#include <string>
using namespace std;
ID3D11Device* d3dDevice_;
ID3D11DeviceContext* d3dContext_;
IDXGISwapChain* swapChain_;
D3D_FEATURE_LEVEL featureLevel_;
D3D_DRIVER_TYPE driverType_;
unsigned int creationFlags = D3D11_CREATE_DEVICE_DEBUG;
ID3D11RenderTargetView* backBufferTarget_;
ID3D11Buffer* vertexBuffer_;
ID3D11InputLayout *inputLayout_;
ID3D11VertexShader *solidColorVS_;
ID3D11PixelShader *solidColorPS_;
HRESULT result;
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void CreateShader();
void CreateBuff();
void Clear();
void Render();
bool initD3dContext(HWND hwnd);
class TestStr
{
public :
	TestStr(const string& a);
private :
	const string& ms;
};
TestStr::TestStr(const string& a)
	:ms(a){}
struct VertexPos
{
	XMFLOAT3 pos;
};
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR cmdLine, int nShow)
{
	unsigned int creationFlags = 0;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = hInstance;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "DX11BookWindowClass";
	if (!RegisterClassEx(&wndClass))
		return -1;
	RECT rc = { 0, 0, 640, 480 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	HWND hwnd = CreateWindow("Dx11BookWindowClass", "Blank Win32 Window",
		WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, hInstance, NULL);
	if (!hwnd) 
		return -1;
	if (!initD3dContext(hwnd))
		return -1;
	CreateShader();
	ShowWindow(hwnd, nShow);
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Update
			//Draw
		}
		Render();
	}
	//Demo Shutdown
	return static_cast<int>(msg.wParam);
}
void Render()
{
	Clear();
	CreateBuff();
	d3dContext_->IASetInputLayout(inputLayout_);
	unsigned int stride = sizeof(VertexPos);
	unsigned int offset = 0;
	d3dContext_->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	d3dContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	d3dContext_->VSSetShader(solidColorVS_, 0, 0);
	d3dContext_->PSSetShader(solidColorPS_, 0, 0);
	d3dContext_->Draw(3, 0);
	swapChain_->Present(0, 0);
}
void Clear()
{
	float color[] = { 0.0f, 0.0f, 0.25f, 1.0f };
	d3dContext_->ClearRenderTargetView(backBufferTarget_, color);
}
void CreateBuff()
{
	VertexPos vertices[] = {
		XMFLOAT3(0.5f, 0.5f, 0.5f),
		XMFLOAT3(0.5f, -0.5f, 0.5f),
		XMFLOAT3(-0.5f, -0.5f, 0.5f),
	};
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.ByteWidth = sizeof(VertexPos) * 3;
	D3D11_SUBRESOURCE_DATA resourceData;
	resourceData.pSysMem = vertices;
	HRESULT result = d3dDevice_->CreateBuffer(&vertexDesc, &resourceData, &vertexBuffer_);
	if (FAILED(result))
	{

	}
}
void CreateShader()
{
	ID3DBlob *vsBuffer = 0;
	ID3DBlob *errorBuffer = 0;
	ID3DBlob *psBuffer = 0;
	DWORD shaderFlags = (1 << 11) | (1 << 0);
	//顶点着色器部分
	result = D3DX11CompileFromFile("VertexShader.hlsl", 0, 0, "VS_Main", "vs_4_0", shaderFlags, 0, 0, &vsBuffer, &errorBuffer, 0);
	if (FAILED(result))
	{
		if (errorBuffer != 0)
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}
		return;
	}
	if (errorBuffer != 0)
		errorBuffer->Release();
	result = d3dDevice_->CreateVertexShader(vsBuffer->GetBufferPointer(), vsBuffer->GetBufferSize(), 0, &solidColorVS_);
	
	if (FAILED(result))
	{
		if (vsBuffer)
			vsBuffer->Release();
		return;
	}
	//设置输入样式
	D3D11_INPUT_ELEMENT_DESC solidColorLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	unsigned int totalLayoutElements = ARRAYSIZE(solidColorLayout);
	result = d3dDevice_->CreateInputLayout(solidColorLayout,
		totalLayoutElements, vsBuffer->GetBufferPointer(),
		vsBuffer->GetBufferSize(), &inputLayout_);
	//像素着色器部分
	result = D3DX11CompileFromFile("PixelShader.hlsl", 0, 0, "PS_Main", "ps_4_0", shaderFlags, 0, 0, &psBuffer, &errorBuffer, 0);
	if (FAILED(result))
	{
		if (errorBuffer != 0)
		{
			OutputDebugStringA((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}
		return;
	}
	if (errorBuffer != 0)
		errorBuffer->Release();
	result = d3dDevice_->CreatePixelShader(psBuffer->GetBufferPointer(), psBuffer->GetBufferSize(), 0, &solidColorPS_);
	if (FAILED(result))
	{
		if (psBuffer)
			psBuffer->Release();
		return;
	}
}
bool initD3dContext(HWND hwnd)
{
	//获取窗口区间
	RECT dimensions;
	if (!GetClientRect(hwnd, &dimensions))
		return -1;
	unsigned int width = dimensions.right - dimensions.left;
	unsigned int height = dimensions.bottom - dimensions.top;
	//设备枚举
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_SOFTWARE
	};
	unsigned int totalDriverTypes = ARRAYSIZE(driverTypes);
	unsigned int driver = 0;
	//支持API等级
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	unsigned int totalFeatureLevels = ARRAYSIZE(featureLevels);
	//交换链定义
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	for (driver = 0; driver < totalDriverTypes; ++driver)
	{
		result = D3D11CreateDeviceAndSwapChain(
			0,
			driverTypes[driver],
			0,
			creationFlags,
			featureLevels,
			totalFeatureLevels,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&swapChain_,
			&d3dDevice_,
			&featureLevel_,
			&d3dContext_);
		if (SUCCEEDED(result))
		{
			driverType_ = driverTypes[driver];
			break;
		}
	}
	if (FAILED(result))
	{
		return false;
	}
	ID3D11Texture2D* backBufferTexture;
	result = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	if (FAILED(result))
	{
		return false;
	}
	result = d3dDevice_->CreateRenderTargetView(backBufferTexture, 0, &backBufferTarget_);
	if (backBufferTexture)
		backBufferTexture->Release();
	if (FAILED(result))
	{
		return false;
	}
	d3dContext_->OMSetRenderTargets(1, &backBufferTarget_, 0);

	D3D11_VIEWPORT viewport;
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 0.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	d3dContext_->RSSetViewports(1, &viewport);
	
	return true;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;
	switch (message)
	{
		case WM_PAINT:
			hDC = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
