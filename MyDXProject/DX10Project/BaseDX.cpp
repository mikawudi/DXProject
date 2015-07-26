#include <d3d10.h>
#include <D3DX10.h>
#include <DXGI.h>
#include "BaseDX.h"
#define MYERROR(n) if(FAILED(n)) { this->IsError = true; return false;}

BaseDX::BaseDX()
	:IsError(false)
	, _swapChain(nullptr)
	, _device(nullptr)
	, _backBufferTarget(nullptr)
	, _mDeptStencilView(nullptr)
	, _mfont(nullptr)
	, hwnd(nullptr)
	, width(0)
	, height(0)
	, gWVP(new D3DXMATRIX())
	, _position(0, 0, -3)
	, _lookAt(0, 0, 0)
	, _wordUp(0, 1, 0)
	, _FovLHMatrix(new D3DXMATRIX())
{

}
BaseDX::~BaseDX()
{
	_swapChain->Release();
	_device->Release();
	_backBufferTarget->Release();
	delete this->gWVP;
}
void BaseDX::InitD3d(const HWND& hwnd)
{
	this->hwnd = &hwnd;
	if (!this->GetWindowsSize())
		return;
	D3DXMatrixPerspectiveFovLH(_FovLHMatrix, D3DX_PI * 0.25f, (float)this->width / this->height, 1.3, 1000);
	if (!this->CreateSwapChainAndDevice())
		return;
	if (!this->CreateRenderTargetView())
		return;
	if (!this->CreateDepthStencilView())
		return;
	if (!this->SetViewPort())
		return;
	this->OMSetRenderTarget();
	if (!this->CreateEffect())
		return;
	if (!this->CreateInputLayOut("ColorTech"))
		return;
	if (this->SetRasterizerState())
		return;
}
bool BaseDX::GetWindowsSize()
{
	RECT dimensions;
	if (!GetClientRect(*hwnd, &dimensions))
		return false;
	width = dimensions.right - dimensions.left;
	height = dimensions.bottom - dimensions.top;
	return true;
}
bool BaseDX::CreateSwapChainAndDevice()
{
	if (!this->GetWindowsSize())
		return false;
	DXGI_SWAP_CHAIN_DESC swapDesc;
	ZeroMemory(&swapDesc, sizeof(swapDesc));
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = *hwnd;
	swapDesc.Windowed = true;
	UINT createDeviceFlags = D3D10_CREATE_DEVICE_DEBUG;
	HRESULT result = D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, D3D10_SDK_VERSION, &swapDesc, &this->_swapChain, &this->_device);
	MYERROR(result);
	return true;
}
bool BaseDX::CreateRenderTargetView()
{
	ID3D10Texture2D* backBuffer;
	HRESULT result = _swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), reinterpret_cast<void**>(&backBuffer));
	MYERROR(result);
	result = _device->CreateRenderTargetView(backBuffer, 0, &_backBufferTarget);
	MYERROR(result);
	backBuffer->Release();
	return true;
}
bool BaseDX::CreateDepthStencilView()
{
	D3D10_TEXTURE2D_DESC textDesc;
	ZeroMemory(&textDesc, sizeof(textDesc));
	textDesc.Width = width;
	textDesc.Height = height;
	textDesc.MipLevels = 1;
	textDesc.ArraySize = 1;
	textDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textDesc.SampleDesc.Count = 1;
	textDesc.SampleDesc.Quality = 0;
	textDesc.Usage = D3D10_USAGE_DEFAULT;
	textDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	textDesc.CPUAccessFlags = 0;
	textDesc.MiscFlags = 0;

	ID3D10Texture2D* mDepthStencilBuffer;
	HRESULT result = _device->CreateTexture2D(&textDesc, 0, &mDepthStencilBuffer);
	MYERROR(result);
	result = _device->CreateDepthStencilView(mDepthStencilBuffer, 0, &_mDeptStencilView);
	MYERROR(result);
	mDepthStencilBuffer->Release();
	return true;
}
void BaseDX::OMSetRenderTarget()
{
	_device->OMSetRenderTargets(1, &_backBufferTarget, _mDeptStencilView);
}
bool BaseDX::SetViewPort()
{
	D3D10_VIEWPORT bp;
	bp.TopLeftX = 0;
	bp.TopLeftY = 0;
	bp.Width = width;
	bp.Height = height;
	bp.MinDepth = 0.0f;
	bp.MaxDepth = 1.0f;
	_device->RSSetViewports(1, &bp);
	return true;
}
void BaseDX::DrawStr(string& str, D3DXCOLOR& color, RECT& R)
{
	if (this->_mfont == nullptr && !this->CreatePen())
	{
		this->_mfont = nullptr;
		return;
	}
	int result = _mfont->DrawText(0, str.c_str(), -1, &R, DT_TOP | DT_LEFT, color);
}
void BaseDX::Clear(const float color[])
{
	_device->ClearRenderTargetView(this->_backBufferTarget, color);
	_device->ClearDepthStencilView(this->_mDeptStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0);
}
void BaseDX::FlushTargetView()
{
	this->_device->Draw(3, 0);
	this->_swapChain->Present(0, 0);
}
bool BaseDX::CreatePen()
{
	D3DX10_FONT_DESC font_desc;
	font_desc.Height = 124;
	font_desc.Width = 0;
	font_desc.Weight = 0;
	font_desc.MipLevels = 1;
	font_desc.Italic = false;
	font_desc.CharSet = DEFAULT_CHARSET;
	font_desc.OutputPrecision = OUT_DEFAULT_PRECIS;
	font_desc.Quality = DEFAULT_QUALITY;
	font_desc.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(font_desc.FaceName, "ËÎÌו");
	font_desc.Italic = false;
	HRESULT result = D3DX10CreateFontIndirect(this->_device, &font_desc, &this->_mfont);
	if (FAILED(result))
	{
		this->_mfont = nullptr;
		return false;
	}
	return true;
}

bool BaseDX::CreateInputLayOut(const char* name)
{
	ID3D10InputLayout *inputLayOut;
	D3D10_PASS_DESC passDesc;
	ID3D10EffectTechnique* mTech = this->mFX->GetTechniqueByName(name);
	if (FAILED(mTech->GetPassByName("Pass1")->GetDesc(&passDesc)))
	{
		return false;
	}
	/*D3D10_INPUT_ELEMENT_DESC layOutDesc[]
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};*/
	D3D10_INPUT_ELEMENT_DESC layOutDesc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};
	auto tesult = this->_device->CreateInputLayout(layOutDesc, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &inputLayOut);
	if (FAILED(tesult))
		return false;
	this->_device->IASetInputLayout(inputLayOut);
	return true;
}
bool BaseDX::CreateEffect()
{
	DWORD shaderFlags = D3D10_SHADER_ENABLE_STRICTNESS | D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
	ID3D10Blob* compliationErrors = 0;
	auto result = D3DX10CreateEffectFromFile("color.fx", 0, 0, "fx_4_0", shaderFlags, 0, this->_device, 0, 0, &this->mFX, &compliationErrors, 0);
	if (FAILED(result))
	{
		if (compliationErrors != 0)
		{
			char* errMesage = (char*)compliationErrors->GetBufferPointer();
			MessageBoxA(0, errMesage, 0, 0);
		}
		return false;
	}
	return true;
}
ID3D10Buffer* BaseDX::CreateBuffer(Vertex1 vertexBuffer[], int vertextBufferLength)
{
	D3D10_BUFFER_DESC desc;
	desc.Usage = D3D10_USAGE_IMMUTABLE;
	desc.ByteWidth = sizeof(Vertex1)*vertextBufferLength;
	desc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA initDesc;
	initDesc.pSysMem = (void*)vertexBuffer;
	initDesc.SysMemPitch = 0;
	initDesc.SysMemSlicePitch = 0;
	ID3D10Buffer* pIndexBuffer = nullptr;
	HRESULT res = this->_device->CreateBuffer(&desc, &initDesc, &pIndexBuffer);
	if (FAILED(res))
		return nullptr;
	return pIndexBuffer;
}
bool BaseDX::SetVertextBuffer(UINT startSlot, UINT numBuffer, ID3D10Buffer** ppVertexBuffer, const UINT *pStrides, const UINT *pOffset)
{
	if (startSlot > 15)
		return false;
	this->_device->IASetVertexBuffers(startSlot, numBuffer, ppVertexBuffer, pStrides, pOffset);
	return true;
}
void BaseDX::SetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY fillMode)
{
	this->_device->IASetPrimitiveTopology(fillMode);
}
void BaseDX::ApplyPass(const char* techniqueName, const char* passName)
{
	this->mFX->GetTechniqueByName(techniqueName)->GetPassByName(passName)->Apply(0);
}
void BaseDX::SetConstFXgWVP(const char* constName, D3DXMATRIX& objWorld)
{
	this->CalculationgWVP(objWorld);
	auto pConstBuff = this->mFX->GetVariableByName(constName);
	auto pVarMatrix = pConstBuff->AsMatrix();
	auto result = pVarMatrix->SetMatrix((float*)this->gWVP);
	if (FAILED(result))
		throw exception();
}
void BaseDX::CalculationgWVP(D3DXMATRIX& objWorld)
{
	D3DXMATRIX lookAtMatrix;
	D3DXMatrixLookAtLH(&lookAtMatrix, &this->_position, &this->_lookAt, &this->_wordUp);
	*this->gWVP = objWorld*lookAtMatrix*(*(this->_FovLHMatrix));
}
bool BaseDX::SetRasterizerState()
{
	D3D10_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D10_RASTERIZER_DESC));
	desc.FillMode = D3D10_FILL_WIREFRAME;
	desc.CullMode = D3D10_CULL_NONE;
	desc.FrontCounterClockwise = true;
	ID3D10RasterizerState* state;
	auto result = this->_device->CreateRasterizerState(&desc, &state); 
	if (FAILED(result))
		return false;
	this->_device->RSSetState(state);
	return true;
}