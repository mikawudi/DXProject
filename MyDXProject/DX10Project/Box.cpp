#include "Box.h"

const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXCOLOR YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);
Box::Box()
	: mNumVertices(0), mNumFaces(0), md3dDevice(0), mVB(0), mIB(0)
{
}

Box::~Box()
{

}

void Box::init(ID3D10Device* device, float scale)
{
	md3dDevice = device;

	mNumVertices = 8;
	mNumFaces = 12; // 2 per quad

	// Create vertex buffer
	Vertex1 vertices[] =
	{
		{ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), WHITE },
		{ D3DXVECTOR3(-1.0f, +1.0f, -1.0f), BLACK },
		{ D3DXVECTOR3(+1.0f, +1.0f, -1.0f), RED },
		{ D3DXVECTOR3(+1.0f, -1.0f, -1.0f), GREEN },
		{ D3DXVECTOR3(-1.0f, -1.0f, +1.0f), BLUE },
		{ D3DXVECTOR3(-1.0f, +1.0f, +1.0f), YELLOW },
		{ D3DXVECTOR3(+1.0f, +1.0f, +1.0f), CYAN },
		{ D3DXVECTOR3(+1.0f, -1.0f, +1.0f), MAGENTA },
	};

	// Scale the box.
	for (DWORD i = 0; i < mNumVertices; ++i)
		vertices[i].pos *= scale;


	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex1) * mNumVertices;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	md3dDevice->CreateBuffer(&vbd, &vinitData, &mVB);


	// Create the index buffer

	DWORD indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D10_BUFFER_DESC ibd;
	ibd.Usage = D3D10_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(DWORD) * mNumFaces * 3;
	ibd.BindFlags = D3D10_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	md3dDevice->CreateBuffer(&ibd, &iinitData, &mIB);
}

void Box::draw()
{
	UINT stride = sizeof(Vertex1);
	UINT offset = 0;
	md3dDevice->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);
	md3dDevice->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);
	md3dDevice->DrawIndexed(mNumFaces * 3, 0, 0);
}