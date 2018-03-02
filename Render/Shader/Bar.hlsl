float _barWidth;

texture _map;

sampler samp
{
    Texture = _map;
};

struct Input
{
    float2 uv : TEXCOORD0;
};

float4 main(Input input) : COLOR0
{
    float4 diffuse = tex2D(samp, input.uv);

	//diffuse.rgb = diffuse.bgr;
    diffuse.r = saturate((1 - _barWidth) * 2);
    diffuse.g = saturate(_barWidth * 2);
    diffuse.b = 0;
    return diffuse;
}


technique Shader
{
    pass Pass0
    {
        PixelShader = compile ps_2_0 main();
    }
};