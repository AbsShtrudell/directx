cbuffer CBuf
{
    float4 color;
    float time;
};

float4 main(float3 pos : POSITION) : SV_TARGET
{
    float4 col;
    col.r = abs(pos.x * cos(time));
    col.g = abs(pos.y * sin(time));
    col.b = abs(pos.z);
    col.a = 1.0f;
    return col;
}