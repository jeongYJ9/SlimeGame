#pragma once

class Drawer
{
public:
	Drawer();
	~Drawer();

	virtual void Render() = 0;

protected:
	void SetData(Vertex* vertex, UINT dataSize, DWORD fvf);

	DWORD fvf;
	UINT dataSize;
	Vertex* vertexData;
	VertexBuffer* vertexBuffer;
};