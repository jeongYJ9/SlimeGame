int _selected;
float4 _color;

texture _map;
sampler samp0
{
	Texture = _map;
};

struct Input
{
	float2 uv : TEXCOORD0;
};

float3 Inverse(float3 color)
{
	float r = 1.0f - color.r;
	float g = 1.0f - color.g;
	float b = 1.0f - color.b;

	return float3(r, g, b);
}

float4 Monotone(float3 color)
{
	float3 weight = float3(0.299f, 0.587f, 0.114f);
	color = dot(color, weight);
	color *= _color;

	return float4(color, 1);
}

float4 main(Input input) : COLOR0
{
	
	return tex2D(samp0, input.uv);
}


float4 main2(Input input) : COLOR0
{
	float4 color = tex2D(samp0, input.uv);

	return float4(Inverse(color.rgb), 1);
}


float4 main3(Input input) : COLOR0
{
	float4 color = tex2D(samp0, input.uv);

	return Monotone(color.rgb);
}

technique Shader
{
	pass Pass0
	{
		PixelShader = compile ps_2_0 main();
	}

	pass Pass2
	{
		PixelShader = compile ps_2_0 main2();
	}

	pass Pass3
	{
		PixelShader = compile ps_2_0 main3();
	}
};