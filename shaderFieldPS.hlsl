

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
    float4 Parameter2;
    float4 ColorBuffer;
    float4 ColorBuffer2;
    float4 ColorBuffer3;
    
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
    
	//outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);
    //outDiffuse.rgb = ColorBuffer.xyz - fbmabs22(inTexCoord * Parameter.x, Parameter.y) * Parameter.z * 0.6;
    //outDiffuse.rgb *= ColorBuffer.xyz + fbmabs22(inTexCoord * Parameter.x, Parameter.y);
    //outDiffuse.rgb *= ColorBuffer2.xyz + fbmabs22(inTexCoord * Parameter.x, Parameter.y) * 6;

    //outDiffuse.rgb = float3(1.0, 0.95, 0.95);
    
    
    //outDiffuse.xyz = float3(0.21, 0.21, 0.21);
    
    float peak = -(inWorldPosition.y + 1.8);
    peak = saturate(peak);
                                     //float3(1.0, 0.95, 0.95)
    //outDiffuse.rgb = (1.0 - peak) + ColorBuffer.xyz * peak;
    outDiffuse.rgb = (1.0 - peak) + ColorBuffer.xyz * peak;
    
    float side = -(inWorldPosition.y + 2.0);
    side = saturate(side);
                                                  //float3(0.21, 0.21, 0.21)     
    outDiffuse.rgb = outDiffuse.rgb * (1.5 - side) + ColorBuffer2.xyz * side;


    float dx = fbmabs2((inTexCoord + float2(0.002, 0.0)) * Parameter2.z, Parameter2.x) * Parameter2.y
                 - fbmabs2((inTexCoord - float2(0.002, 0.0)) * Parameter2.z, Parameter2.x) * Parameter2.y;
    
    float3 vx = float3(0.004, dx, 0.0);
    
    float dz = fbmabs2((inTexCoord - float2(0.0, 0.002)) * Parameter2.z, Parameter2.x) * Parameter2.y
                 - fbmabs2((inTexCoord + float2(0.0, 0.002)) * Parameter2.z, Parameter2.x) * Parameter2.y;
    
    float3 vz = float3(0.0, dz, 0.004);
    
    float3 normal = normalize(cross(vz, vx));
    

    //ライティング

    //float3 lightDir = float3(1.0, -1.0, 1.0);
    //lightDir = normalize(lightDir);
    float3 lightDir = normalize(Light.Direction.xyz);
    
    float light = saturate(0.5 - dot(normal, lightDir) * 0.5);
    //float light = 0.5 - dot(normal, lightDir) * 0.5;
    outDiffuse.rgb *= light;
    
    
    
    
    float waterFog = -(inWorldPosition.y + 3.0) * Parameter.w;
    waterFog = saturate(waterFog);
    
                                                         //float3(0.0, 0.95, 1.0)
    outDiffuse.rgb = outDiffuse.rgb * (1.0 - waterFog) + ColorBuffer3.xyz * waterFog;
   
   
    
    outDiffuse.a = 1.0;

}
