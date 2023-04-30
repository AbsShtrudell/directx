cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};

struct VSOut
{
    float3 worldPos : POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float3 n : NORMAL, float3 color : COLOR)
{
    VSOut vso;
    vso.worldPos = (float3) mul(float4(pos, 1.0f), modelView);
    vso.normal = mul(n, (float3x3) modelView);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    vso.color = color;
    return vso;
}