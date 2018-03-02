#include "../stdafx.h"
#include "Drawer.h"

Drawer::Drawer()
	: vertexData(NULL), vertexBuffer(NULL)
{

}

Drawer::~Drawer()
{
	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE(vertexBuffer);
}

void Drawer::SetData(Vertex * vertex, UINT dataSize, DWORD fvf)
{
	assert(vertexBuffer == NULL);
	assert(dataSize > 0);

	this->dataSize = dataSize;
	this->vertexData = vertex;
	this->fvf = fvf;

	vertexBuffer = new VertexBuffer();
	vertexBuffer->Create(dataSize, fvf);
	vertexBuffer->CopyData(vertexData);
}
