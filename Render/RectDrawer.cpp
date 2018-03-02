#include "../stdafx.h"
#include "RectDrawer.h"

RectDrawer::RectDrawer(Size size, float depth, DWORD color)
	: color(color)
{
	VertexPC* vertex = new VertexPC[6];
	vertex[0] = VertexPC(0, 0, color, depth); //0
	vertex[1] = VertexPC(size.width, 0, color, depth); //1
	vertex[2] = VertexPC(0, size.height, color, depth); //2
	vertex[3] = VertexPC(0, size.height, color, depth); //2
	vertex[4] = VertexPC(size.width, 0, color, depth); //1
	vertex[5] = VertexPC(size.width, size.height, color, depth); //3

	int dataSize = sizeof(VertexPC) * 6;
	SetData(vertex, dataSize, VertexPC::FVF);
}

RectDrawer::~RectDrawer()
{

}


void RectDrawer::Render()
{
	Device::GetDevice()->SetStreamSource(0, vertexBuffer->GetBuffer(), 0, sizeof(VertexPC));
	Device::GetDevice()->SetFVF(VertexPC::FVF);
	Device::GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
}
