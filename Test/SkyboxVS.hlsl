cbuffer TransformCBuf
{
    matrix viewProj;
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
    vso.pos = mul(float4(pos, 0.0f), viewProj);
    
    vso.pos.z = vso.pos.w;

    return vso;
} 