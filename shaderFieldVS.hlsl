
#include "shaderNoise.hlsl"


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
                                         //0.1
    inPosition.y += fbmabs2(inTexCoord * Parameter.x, Parameter.y) * Parameter.z;
    //inPosition.y += noise_deriv(inTexCoord);
    
    //float dx = fbm2((inTexCoord + float2(0.2, 0.0)) * 0.1, 5) * 10.0
    //             - fbm2((inTexCoord - float2(0.2, 0.0)) * 0.1, 5) * 10.0;
    
    //float3 vx = float3(0.4, dx, 0.0);
    
    //float dz = fbm2((inTexCoord - float2(0.0, 0.2)) * 0.1, 5) * 10.0
    //             - fbm2((inTexCoord + float2(0.0, 0.2)) * 0.1, 5) * 10.0;
    
    //float3 vz = float3(0.0, dz, 0.4);
    
    //outNormal.xyz = normalize(cross(vz, vx));
    //outNormal.w = 0.0;
    
	outPosition = mul(inPosition, wvp);
    outWorldPosition = mul(inPosition, World);

	outDiffuse = inDiffuse;
    outNormal = inNormal;
	outTexCoord = inTexCoord;
}

