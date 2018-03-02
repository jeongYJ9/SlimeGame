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
	// 톤컬러는 (1,1,0,0) 노란색
	//colorWeight는 0~1의값. colorWeight가 0이면 무조건 흰색이어야됨.

  //  diffuse.a = _PrevWeight;

  

	return diffuse;
}

float4 main1(Input input) : COLOR0
{

    float4 diffuse = tex2D(sampler0, input.uv);
	// 톤컬러는 (1,1,0,0) 노란색
	//colorWeight는 0~1의값. colorWeight가 0이면 무조건 흰색이어야됨.

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
	
		
		//선형합성
		//C = Cd(1 - As) + CsAs
		/*DestBlend = InvSrcAlpha;
		SrcBlend = SrcAlpha;*/

		//덧셈합성
		//C = Cd + CsAs
		/*DestBlend = One;
		SrcBlend = SrcAlpha;*/

		//뺄셈합성
		//C = Cd - CsAs
		/*BlendOp = RevSubtract;
		DestBlend = One;
		SrcBlend = SrcAlpha;*/

		//곱셈합성
		//C = Cd * Cs
		/*DestBlend = SrcColor;
		SrcBlend = Zero;*/

		//제곱합성
		//C = Cd * Cd
		/*DestBlend = DestColor;
		SrcBlend = Zero;*/

		//네거티브 포지티브 반전
		//C  = (1 - Cd) * Cs
		/*DestBlend = Zero;
		SrcBlend = InvDestColor;*/

		//불투명
		//C = Cd
		/*DestBlend = Zero;
		SrcBlend = One;*/
