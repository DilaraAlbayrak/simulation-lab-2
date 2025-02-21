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
float4 PS(VS_OUTPUT input) : SV_Target
{
    float size = 2; 
    
    float2 uv = input.WorldPos.xy / size + 5;
    
    float patternMask = fmod(floor(uv.x) + fmod(floor(uv.y), 2.0), 2.0);

    float4 lightColor = float4(0.6, 0.2, 0.2, 1.0); 
    float4 darkColor = float4(0.1, 0.1, 0.3, 1.0); 

    return lerp(lightColor, darkColor, patternMask);
    
    return input.Color;
}