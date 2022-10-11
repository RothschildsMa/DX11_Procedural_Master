#include "shaderNoise.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);


// 定数バッファ
cbuffer ConstatntBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;

    float4 CameraPosition;
    float4 Parameter;

}

#define DRAG_MULT 0.048
#define ITERATIONS_RAYMARCH 13
#define ITERATIONS_NORMAL 48
#define WATER_DEPTH 2.1



//float H = 0.0;

//float3 normal(float2 pos, float e, float depth)
//{
//    float2 ex = float2(e, 0);
//    H = getwaves(pos.xy * 0.1, ITERATIONS_NORMAL) * depth;
//    float3 a = float3(pos.x, H, pos.y);
//    return normalize(cross(normalize(a - float3(pos.x - e, getwaves(pos.xy * 0.1 - ex.xy * 0.1, ITERATIONS_NORMAL) * depth, pos.y)),
//                           normalize(a - float3(pos.x, getwaves(pos.xy * 0.1 + ex.yx * 0.1, ITERATIONS_NORMAL) * depth, pos.y + e))));
//}






//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(in float4 inPosition : SV_POSITION,
            in float4 inWorldPosition : POSITION0,
			in float4 inNormal : NORMAL0,
			in float4 inDiffuse : COLOR0,
			in float2 inTexCoord : TEXCOORD0,

			out float4 outDiffuse : SV_Target)
{
 
    float offset = Parameter.x * 0.5;
    
    float dx = fbm2((inTexCoord + float2(0.002, 0.0)) * 1.0, 5, offset) * 0.1
                 - fbm2((inTexCoord - float2(0.002, 0.0)) * 1.0, 5, offset) * 0.1;
    
    float3 vx = float3(0.004, dx, 0.0);
    
    float dz = fbm2((inTexCoord - float2(0.0, 0.002)) * 1.0, 5, offset) * 0.1
                 - fbm2((inTexCoord + float2(0.0, 0.002)) * 1.0, 5, offset) * 0.1;
    
    float3 vz = float3(0.0, dz, 0.004);
    
    float3 normal = normalize(cross(vz, vx));
    
    float3 eyev = inWorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    //フレネル近似式
    float fresnel = saturate(1.0 + dot(eyev, normal));
    fresnel = 0.05 + (1.0 - 0.05) * pow(fresnel, 5);
    
    outDiffuse.rgb = float3(0.0, 0.31, 0.31) * (1.0 - fresnel) + float3(0.403, 0.756, 0.792) * fresnel;
    
    float dist = distance(CameraPosition.xyz, inWorldPosition.xyz);
    
    //outDiffuse.rgb = float3(0.403, 0.756, 0.792);
   
    //outDiffuse.a = saturate(dist * 0.3 + 1.1 + dot(eyev, normal));
    outDiffuse.a = fresnel + Parameter.y;

}