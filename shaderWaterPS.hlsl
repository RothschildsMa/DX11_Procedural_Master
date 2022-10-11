

#include "shaderNoise.hlsl"

Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);

// 定数バッファ
cbuffer ConstatntBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;

    float4 CameraPosition;
    float4 Parameter;

}



//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( in  float4 inPosition		: SV_POSITION,
            in float4 inWorldPosition   : POSITION0,
			in  float4 inNormal			: NORMAL0,
			in  float4 inDiffuse		: COLOR0,
			in  float2 inTexCoord		: TEXCOORD0,

			out float4 outDiffuse		: SV_Target )
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
    
    outDiffuse.rgb = float3(0.0, 0.1, 0.1) * (1.0 - fresnel) + float3(0.9, 0.95, 1.0) * fresnel;
    
    float dist = distance(CameraPosition.xyz, inWorldPosition.xyz);
   
    outDiffuse.a = saturate(dist * 0.3 + 1.1 + dot(eyev, normal));
 
}
