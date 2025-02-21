//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBufferCamera : register(b0)
{
    matrix View;
    matrix Projection;
    float4 eyePos;
}

cbuffer ConstantBuffer : register(b1)
{
    matrix World;
}

//--------------------------------------------------------------------------------------
// Vertex Shader Output Structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float4 Color : COLOR0;
    float3 WorldPos : TEXCOORD0; // Used for procedural texture
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(float3 Pos : POSITION, float3 Normal : NORMAL, float4 Color : COLOR)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    float4 worldPos = mul(float4(Pos, 1.0f), World);
    output.Pos = mul(worldPos, View);
    output.Pos = mul(output.Pos, Projection);
    
    output.WorldPos = Pos; // Preserve world position for procedural texture
    output.Normal = Normal;
    output.Color = Color;

    return output;
}

//--------------------------------------------------------------------------------------
// Procedural Checkerboard Pixel Shader
//--------------------------------------------------------------------------------------
// Hash based random number generator
float random(float2 uv)
{
    return frac(sin(dot(uv, float2(30, 60))));
}

// Lerp function
float randRange(float2 uv, float minVal, float maxVal)
{
    return lerp(minVal, maxVal, random(uv));
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float size = 1.5;
    
    float2 uv = input.WorldPos.xy / size + 4;

    float patternMask = fmod(floor(uv.x) + fmod(floor(uv.y), 2.0), 2.0);
    
    float3 lightColorRGB = float3(
        randRange(floor(uv) + 0.1, 0.5, 1.0),
        randRange(floor(uv) + 0.2, 0.5, 1.0),
        randRange(floor(uv) + 0.3, 0.5, 1.0)
    );
    
    float3 darkColorRGB = float3(
        randRange(floor(uv) + 0.4, 0.0, 0.5),
        randRange(floor(uv) + 0.5, 0.0, 0.5),
        randRange(floor(uv) + 0.6, 0.0, 0.5)
    );

    float4 lightColor = float4(lightColorRGB, 1.0);
    float4 darkColor = float4(darkColorRGB, 1.0);

    //return lerp(lightColor, darkColor, patternMask);
    
    return input.Color;
}