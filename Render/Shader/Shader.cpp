#include "../../stdafx.h"
#include "Shader.h"

void Shader::CreateEffect(wstring fileName, LPD3DXEFFECT * effect)
{
	HRESULT hr;

	LPD3DXBUFFER error;
	hr = D3DXCreateEffectFromFile
	(
		Device::GetDevice(), 
		fileName.c_str(), NULL, NULL
		, D3DXSHADER_DEBUG, NULL
		, effect, &error
	);

	if (*effect == NULL && error != NULL)
	{
		void* pointer = error->GetBufferPointer();

		if (pointer != NULL)
		{
			string str = (const char *)pointer;

			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
			assert(false);
		}
	}
}

Shader::Shader(LPDIRECT3DDEVICE9 device, wstring fileName)
	: device(device)
	, vertexBuffer(NULL)
	, shader(NULL)
{
	CreateEffect(fileName, &shader);

	D3DXMatrixIdentity(&world);
}


Shader::~Shader()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(shader);
}
