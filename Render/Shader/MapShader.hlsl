//float4 _toneColor;
float _PrevWeight;

texture _map;
sampler sampler0
{
	Texture = _map;

MinFilter = Linear;
MagFilter = Linear;
};

struct Input
{
	float2 uv : TEXCOORD0;
};

float4 main(Input input) : COLOR0
{

	float4 diffuse = tex2D(sampler0 , input.uv);
	// ���÷��� (1,1,0,0) �����
	//colorWeight�� 0~1�ǰ�. colorWeight�� 0�̸� ������ ����̾�ߵ�.

  //  diffuse.a = _PrevWeight;

  

	return diffuse;
}

float4 main1(Input input) : COLOR0
{

    float4 diffuse = tex2D(sampler0, input.uv);
	// ���÷��� (1,1,0,0) �����
	//colorWeight�� 0~1�ǰ�. colorWeight�� 0�̸� ������ ����̾�ߵ�.

    diffuse.a = 0.5f;

  

    return diffuse;
}


technique Shader
{
	pass P0
	{
		Lighting = false;
		AlphaBlendEnable = true;

		DestBlend = InvSrcAlpha;
		SrcBlend = SrcAlpha;

		PixelShader = compile ps_2_0 main();
	}
    pass P0
    {
        Lighting = false;
        AlphaBlendEnable = true;

        DestBlend = InvSrcAlpha;
        SrcBlend = SrcAlpha;

        PixelShader = compile ps_2_0 main1();
    }
};
	
		
		//�����ռ�
		//C = Cd(1 - As) + CsAs
		/*DestBlend = InvSrcAlpha;
		SrcBlend = SrcAlpha;*/

		//�����ռ�
		//C = Cd + CsAs
		/*DestBlend = One;
		SrcBlend = SrcAlpha;*/

		//�����ռ�
		//C = Cd - CsAs
		/*BlendOp = RevSubtract;
		DestBlend = One;
		SrcBlend = SrcAlpha;*/

		//�����ռ�
		//C = Cd * Cs
		/*DestBlend = SrcColor;
		SrcBlend = Zero;*/

		//�����ռ�
		//C = Cd * Cd
		/*DestBlend = DestColor;
		SrcBlend = Zero;*/

		//�װ�Ƽ�� ����Ƽ�� ����
		//C  = (1 - Cd) * Cs
		/*DestBlend = Zero;
		SrcBlend = InvDestColor;*/

		//������
		//C = Cd
		/*DestBlend = Zero;
		SrcBlend = One;*/
