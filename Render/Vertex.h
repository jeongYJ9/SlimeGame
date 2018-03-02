#pragma once

struct Vertex
{
	D3DXVECTOR3 position;

	Vertex()
	{
		position = D3DXVECTOR3(0, 0, 0);
	}

	Vertex(float x, float y, float depth)
	{
		position = D3DXVECTOR3(x, y, depth);
	}

	Vertex(const Point& point, float depth)
	{
		position = D3DXVECTOR3(point.x, point.y, depth);
	}
};

struct VertexPC : public Vertex
{
	DWORD color;

	static DWORD FVF;

	VertexPC() : Vertex()
	{
		color = 0xFF000000;
	}

	VertexPC(float x, float y, DWORD color, float depth = 0.0f)
		: Vertex(x, y, depth)
	{
		this->color = color;
	}

	VertexPC(Point location, DWORD color, float depth = 0.0f)
		: Vertex(location, depth)
	{
		this->color = color;
	}
};

struct VertexPT : public Vertex
{
	D3DXVECTOR2 coord;

	static DWORD FVF;

	VertexPT() : Vertex()
	{
		coord = D3DXVECTOR2(0, 0);
	}

	VertexPT(float x, float y, float u, float v, float depth = 0.0f)
		: Vertex(x, y, depth)
	{
		coord = D3DXVECTOR2(u, v);
	}

	VertexPT(Point location, Point coord, float depth = 0.0f)
		: Vertex(location, depth)
	{
		this->coord = coord.ToVector2();
	}
};