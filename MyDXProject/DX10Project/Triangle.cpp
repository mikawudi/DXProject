#include "Triangle.h"


Triangle::Triangle(ID3D10Device* device)
	: _devic(device)
	, _vertexBuffer(0)
	, _indexBuffer(0)
	, rollY(0.0f)
{
	D3DXMatrixRotationY(&this->World, 0.0f);
	//数据在一个槽里
	Vertex1 _triangleVertexArray[] =
	{
		{ D3DXVECTOR3(0, 2, 2), D3DXCOLOR(0.2, 0.5, 0.8, 1) },
		{ D3DXVECTOR3(1, 0, 2), D3DXCOLOR(0.8, 0.5, 0.2, 1) },
		{ D3DXVECTOR3(-1, 0, 2), D3DXCOLOR(0.5, 0.8, 0.2, 1) }
	};
	D3D10_BUFFER_DESC vbd;
	vbd.Usage = D3D10_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex1) * 3;
	vbd.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = _triangleVertexArray;
	this->_devic->CreateBuffer(&vbd, &vinitData, &this->_vertexBuffer);

	//数据在两个槽,这个是顶点槽
	/*Vertex _triangleVertexArray2[] =
	{
		D3DXVECTOR3(0, 2, 2),
		D3DXVECTOR3(1, 0, 2),
		D3DXVECTOR3(-1, 0, 2)
	};*/
	Vertex _triangleVertexArray2[] = 
	{
		{ D3DXVECTOR3(0, 2, 0) },
		{ D3DXVECTOR3(1, 0, 0) },
		{ D3DXVECTOR3(-1, 0, 0) }
	};
	D3D10_BUFFER_DESC vbd2;
	vbd2.Usage = D3D10_USAGE_IMMUTABLE;
	vbd2.ByteWidth = sizeof(Vertex) * 3;
	vbd2.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd2.CPUAccessFlags = 0;
	vbd2.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData2;
	vinitData2.pSysMem = _triangleVertexArray2;
	this->_devic->CreateBuffer(&vbd2, &vinitData2, &this->_vertextOnlyBuffer);
	//数据在两个槽,这个是颜色槽
	Color _triangleVertexArray3[] =
	{
		D3DXCOLOR(0.5, 0.5, 0.5, 1),
		D3DXCOLOR(0.5, 0.5, 0.5, 1),
		D3DXCOLOR(0.5, 0.5, 0.5, 1)
	};
	D3D10_BUFFER_DESC vbd3;
	vbd3.Usage = D3D10_USAGE_IMMUTABLE;
	vbd3.ByteWidth = sizeof(Color) * 3;
	vbd3.BindFlags = D3D10_BIND_VERTEX_BUFFER;
	vbd3.CPUAccessFlags = 0;
	vbd3.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA vinitData3;
	vinitData3.pSysMem = _triangleVertexArray3;
	this->_devic->CreateBuffer(&vbd3, &vinitData3, &this->_colorOnlyBuffer);


	DWORD _triangleIndexArray[] =
	{
		0, 1, 2
	};
	D3D10_BUFFER_DESC idv;
	idv.Usage = D3D10_USAGE_IMMUTABLE;
	idv.ByteWidth = sizeof(DWORD) * 3;
	idv.BindFlags = D3D10_BIND_INDEX_BUFFER;
	idv.CPUAccessFlags = 0;
	idv.MiscFlags = 0;
	D3D10_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = _triangleIndexArray;
	this->_devic->CreateBuffer(&idv, &iinitData, &this->_indexBuffer);
}

void Triangle::draw()
{
	UINT stride = sizeof(Vertex1);
	UINT offset = 0;
	//this->_devic->IASetVertexBuffers(0, 1, &this->_vertexBuffer, &stride, &offset);
	//this->_devic->IASetIndexBuffer(this->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	UINT stride2 = sizeof(Vertex);
	UINT offset2 = 0;
	this->_devic->IASetVertexBuffers(0, 1, &this->_vertextOnlyBuffer, &stride2, &offset2);
	UINT stride3 = sizeof(Color);
	UINT offset3 = 0;
	this->_devic->IASetVertexBuffers(1, 1, &this->_colorOnlyBuffer, &stride3, &offset3);
	this->_devic->Draw(3, 0);
}
D3DXMATRIX& Triangle::CaculateWorldMatrix()
{
	D3DXMATRIX temp;
	D3DXMatrixRotationY(&temp, D3DX_PI * this->rollY);
	D3DXMATRIX temp2;
	D3DXMatrixTranslation(&temp2, 0, 0, 3);
	this->World = temp * temp2;
	return this->World;
}
void Triangle::ChangeRolY()
{
	this->rollY = this->rollY + 0.01f;
}
Triangle::~Triangle()
{
	/*delete[] this->_triangleVertexArray;
	delete[] this->_triangleIndexArray;
	if (this->_vertexBuffer != nullptr)
		this->_vertexBuffer->Release();
	if (this->_indexBuffer != nullptr)
		this->_indexBuffer->Release();*/
}
