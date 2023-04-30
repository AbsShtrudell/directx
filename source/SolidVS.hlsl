cbuffer CBuf
{
    matrix model;
    matrix modelViewProj;
};

struct VSOut
{
    float3 worldPos : POSITION;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION)
{
    VSOut vso;
    vso.worldPos = pos;
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    return vso;
}