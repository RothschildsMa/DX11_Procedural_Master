
#include "shaderNoise.hlsl"


// 定数バッファ
cbuffer ConstatntBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;

	float4 CameraPosition;
}


//=============================================================================
// 頂点シェーダ
//=============================================================================
void main( in  float4 inPosition		: POSITION0,
			in  float4 inNormal			: NORMAL0,
			in  float4 inDiffuse		: COLOR0,
			in  float2 inTexCoord		: TEXCOORD0,
			
			out float4 outPosition		: SV_POSITION,
            out float4 outWorldPosition : POSITION0,
			out float4 outNormal		: NORMAL0,
			out float4 outDiffuse		: COLOR0,
			out float2 outTexCoord		: TEXCOORD0 )
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
    
    //float dx = fbmabs2((inTexCoord + float2(0.002, 0.0)) * 0.5, 5) * 1.0
    //             - fbmabs2((inTexCoord - float2(0.002, 0.0)) * 0.5, 5) * 1.0;
    
    //float3 vx = float3(0.004, dx, 0.0);
    
    //float dz = fbmabs2((inTexCoord - float2(0.0, 0.002)) * 0.5, 5) * 1.0
    //             - fbmabs2((inTexCoord + float2(0.0, 0.002)) * 0.5, 5) * 1.0;
    
    //float3 vz = float3(0.0, dz, 0.004);
    
    //float3 normal = normalize(cross(vz, vx));
    //normal.y *= -1.0;
	
	outPosition = mul(inPosition, wvp);
    outWorldPosition = mul(inPosition, World);

    //outNormal.xyz = normal.xyz;
	outDiffuse = inDiffuse;

	outTexCoord = inTexCoord;
}

