#include "BaseDX.h"
#ifndef TRIANGLE_H
#define TRIANGLE_H
class Triangle
{
public:
	Triangle(ID3D10Device* device);
	void draw();
	~Triangle();
	D3DXMATRIX& Triangle::CaculateWorldMatrix();
	void Triangle::ChangeRolY();
private:
	ID3D10Device* _devic;
	ID3D10Buffer* _vertexBuffer;
	ID3D10Buffer* _indexBuffer;

	ID3D10Buffer* _vertextOnlyBuffer;
	ID3D10Buffer* _colorOnlyBuffer;
	D3DXMATRIX World;
	float rollY;
};
#endif // !"Triangle_H"