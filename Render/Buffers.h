#pragma once

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	void Create(UINT dataSize, DWORD fvf);
	void CopyData(void* data);

	LPDIRECT3DVERTEXBUFFER9 GetBuffer()
	{
		return vertexBuffer;
	}

	void* GetData() { return data; }
	UINT GetDataSize() { return dataSize; }

private:
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;

	void* data;
	UINT dataSize;
	DWORD fvf;
};

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	void Create(UINT dataSize, D3DFORMAT format = D3DFMT_INDEX16);
	void CopyData(void* data);

	LPDIRECT3DINDEXBUFFER9 GetBuffer()
	{
		return indexBuffer;
	}

	void* GetData() { return data; }
	UINT GetDataSize() { return dataSize; }

private:
	LPDIRECT3DINDEXBUFFER9 indexBuffer;

	void* data;
	UINT dataSize;
	D3DFORMAT format;
};