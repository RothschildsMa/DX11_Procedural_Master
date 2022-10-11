

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

// ライトバッファ
struct LIGHT
{
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
};

cbuffer LightBuffer : register(b1)
{
    LIGHT Light;
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
    //プロシージャル曇
    float offset = Parameter.x * 0.15;
    float noise = fbm2((inTexCoord + float2(0.002, 0.0)) * 0.5, 3, offset);
    
    //ドメインワーピング
    //float noise = fbm2(inTexCoord + fbm2(inTexCoord, 5, offset), 5, offset);
    
    noise = noise + 0.2;
    noise = saturate(noise);
    
    outDiffuse.a = noise;
    
    //if(noise < 0.5)
    //{
    //    noise = noise * 2.0;
    //    outDiffuse.rgb = float3(1.0, 1.0, 1.0) * (1.0 - noise)
    //                 + float3(1.0, 1.0, 1.0) * noise;
    //}
    //else
    //{
    //    noise = (noise - 0.5) * 2.0;
    //    outDiffuse.rgb = float3(1.0, 1.0, 1.0) * (1.0 - noise)
    //                 + float3(0.0, 0.0, 0.0) * noise;
    //}
    
    float dx = fbm2((inTexCoord + float2(0.002, 0.0)) * 0.5, 3) * 1.0
                 - fbm2((inTexCoord - float2(0.002, 0.0)) * 0.5, 3) * 1.0;
    
    float3 vx = float3(0.004, dx, 0.0);
    
    float dz = fbm2((inTexCoord - float2(0.0, 0.002)) * 0.5, 3) * 1.0
                 - fbm2((inTexCoord + float2(0.0, 0.002)) * 0.5, 3) * 1.0;
    
    float3 vz = float3(0.0, dz, 0.004);
    
    float3 normal = normalize(cross(vz, vx));
    normal.y *= -1.0;
    
    //フォグ
    float dist = distance(CameraPosition.xyz, inWorldPosition.xyz);
    dist *= 0.005;
    dist = saturate(dist);
    
    //outDiffuse.rgb = outDiffuse.rgb * (1.0 - dist) + float3(1.0, 1.0, 1.0) * dist;
    
    outDiffuse.a *= (1.0 - dist);
    
    float3 lightDir = normalize(Light.Direction.xyz);
    
    float light = saturate(0.5 - dot(normal, lightDir) * 0.5);
    outDiffuse.rgb = light;
    
    //float light = -dot(lightDir, -inNormal.xyz);
    //light = saturate(light);
    
    //outDiffuse.rgb *= light;
    
}
