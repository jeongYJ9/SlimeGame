#pragma once
class Shader
{
public:
	static void CreateEffect(wstring fileName, LPD3DXEFFECT* effect);

	Shader
	(
		LPDIRECT3DDEVICE9 device
		, wstring fileName
	);
	~Shader();

	virtual void Update() = 0;
	virtual void Render() = 0;

	void PushPreShader(Shader* shader)
	{
		preShaders.push_back(shader);
	}

protected:

	vector<Shader *> preShaders;

	LPDIRECT3DDEVICE9 device;
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	LPD3DXEFFECT shader;

	D3DXMATRIX world;
};

