#include <d3d10.h>
#include <D3DX10.h>
#include <DXGI.h>
#include <string>
using namespace std;
#ifndef BASE_DX_
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXCOLOR color;
} Vertex1;
typedef struct
{
	D3DXVECTOR3 pos;
} Vertex;
typedef struct
{
	D3DXCOLOR color;
} Color;
#define BASE_DX_
class BaseDX
{
public:
	BaseDX::BaseDX();
	virtual void BaseDX::InitD3d(const HWND& hwnd);
	virtual BaseDX::~BaseDX();
	virtual void BaseDX::DrawStr(string& str, D3DXCOLOR& color, RECT& R);
	virtual bool BaseDX::CreatePen();
	virtual void BaseDX::Clear(const float color[]);
	virtual void BaseDX::FlushTargetView();
	ID3D10Buffer* BaseDX::CreateBuffer(Vertex1 vertexBuffer[], int vertextBufferLength);
	bool BaseDX::SetVertextBuffer(UINT startSlot, UINT numBuffer, ID3D10Buffer** ppVertexBuffer, const UINT *pStrides, const UINT *pOffset);
	void BaseDX::SetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY fillMode);
	void BaseDX::ApplyPass(const char* techniqueName, const char* passName);
	void BaseDX::SetConstFXgWVP(const char* constName, D3DXMATRIX& objWorld);
	D3DXMATRIX* gWVP;
	ID3D10Device* _device;
protected:
	virtual bool BaseDX::GetWindowsSize();
	virtual bool BaseDX::CreateSwapChainAndDevice();
	virtual bool BaseDX::CreateRenderTargetView();
	virtual bool BaseDX::CreateDepthStencilView();
	virtual void BaseDX::OMSetRenderTarget();
	virtual bool BaseDX::SetViewPort();
	virtual bool BaseDX::CreateInputLayOut(const char* name);
	virtual bool BaseDX::CreateEffect();
	virtual void BaseDX::CalculationgWVP(D3DXMATRIX& objWorld);
	virtual bool BaseDX::SetRasterizerState();
private:
	bool IsError;
	const HWND* hwnd;
	unsigned int width;
	unsigned int height;
	IDXGISwapChain* _swapChain;
	ID3D10RenderTargetView* _backBufferTarget;
	ID3D10DepthStencilView* _mDeptStencilView;
	ID3DX10Font* _mfont;
	ID3D10Effect* mFX;
	D3DXVECTOR3 _lookAt;
	D3DXVECTOR3 _position;
	D3DXVECTOR3 _wordUp;
	D3DXMATRIX* _FovLHMatrix;
};
#endif