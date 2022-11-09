cbuffer CBuf
{
    float4 color;
};

float4 main(float3 pos : POSITION) : SV_TARGET
{
    float4 col;
    col.r = abs(pos.x);
    col.g = abs(pos.y);
    col.b = abs(pos.z);
    col.a = 1.0f;
    return col;
}