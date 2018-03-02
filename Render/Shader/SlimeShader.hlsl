float4 _toneColor;
float _colorWeight; //_colorWeight
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

    //diffuse.rgb *= _toneColor.rgb*(1-_colorWeight); //�������� ������� �Ʒ����� �ٲ�
   

    //float3 white = float3(1, 1, 1);
    //float3 color = (white * (1 - _colorWeight)) + (_toneColor.rgb * _colorWeight);
    //diffuse.rgb *= color;
    //�Ʒ� �ڵ� ��������
    diffuse.rgb *= (1 - _colorWeight) + _toneColor.rgb * _colorWeight;
	//colorWeight�� 0~1�ǰ�. colorWeight�� 0�̸� ������ ����̾�ߵ�.

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
