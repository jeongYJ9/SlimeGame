#include "../stdafx.h"
#include "Buffers.h"

VertexBuffer::VertexBuffer()
	: vertexBuffer(NULL)
	, data(NULL), dataSize(0)
{

}

VertexBuffer::~VertexBuffer()
{
	SAFE_RELEASE(vertexBuffer);
}

void VertexBuffer::Create(UINT dataSize, DWORD fvf)
{
	assert(vertexBuffer == NULL);
	assert(dataSize > 0);

	this->dataSize = dataSize;
	this->fvf = fvf;

	HRESULT hr = Device::GetDevice()->CreateVertexBuffer
	(
		dataSize, 0, fvf, D3DPOOL_DEFAULT, &vertexBuffer, NULL
	);
	assert(SUCCEEDED(hr));
}

void VertexBuffer::CopyData(void * data)
{
	assert(data != NULL);
	assert(vertexBuffer != NULL);

	this->data = data;

	LPVOID vertex = NULL;
	HRESULT hr = vertexBuffer->Lock(0, 0, &vertex, 0);
	assert(SUCCEEDED(hr));

	memcpy(vertex, this->data, dataSize);

	hr = vertexBuffer->Unlock();
	assert(SUCCEEDED(hr));
}

IndexBuffer::IndexBuffer()
	: indexBuffer(NULL)
	, data(NULL), format(D3DFMT_INDEX16)
{

}

IndexBuffer::~IndexBuffer()
{
	SAFE_RELEASE(indexBuffer);
}

void IndexBuffer::Create(UINT dataSize, D3DFORMAT format)
{
	assert(indexBuffer == NULL);
	assert(dataSize > 0);

	this->dataSize = dataSize;
	this->format = format;

	HRESULT hr = DEVICE()->CreateIndexBuffer
	(
		dataSize, 0, format, D3DPOOL_DEFAULT, &indexBuffer, NULL
	);
	assert(SUCCEEDED(hr));
}

void IndexBuffer::CopyData(void * data)
{
	assert(data != NULL);
	assert(indexBuffer != NULL);

	this->data = data;

	LPVOID index = NULL;
	HRESULT hr = indexBuffer->Lock(0, 0, &index, 0);
	assert(SUCCEEDED(hr));

	memcpy(index, this->data, dataSize);

	hr = indexBuffer->Unlock();
	assert(SUCCEEDED(hr));
}