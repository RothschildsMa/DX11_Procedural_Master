
#include "shaderNoise.hlsl"


// 定数バッファ
cbuffer ConstatntBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;

    float4 CameraPosition;
    float4 Parameter;
    float4 Parameter2;
}

float waves(float2 position, int iterations, float time)
{
    float iter = 0.0;
    float phase = Parameter.z; //6
    float speed = Parameter.w; //2
    float weight = 1.0; //1
    float w = 0.0;
    float ws = 0.0;
    
    for (int i = 0; i < iterations; i++)
    {
        float2 p = float2(sin(iter), cos(iter));
        float x = dot(p, position) * phase + time * speed;
        float wave = exp(sin(x) - 1.0);
        float dx = (wave + Parameter2.y) * cos(x);
        
        float2 res = (wave, -dx);
        position += normalize(p) * res.y * weight * 0.048;
        w += res.x * weight;
        iter += 12.0; //12
        ws += weight;
        weight = lerp(weight, 0.0, 0.2);
        phase *= 1.18;
        speed *= 1.07;
    }
    return w / ws;
}


//=============================================================================
// 頂点シェーダ
//=============================================================================
void main(in float4 inPosition : POSITION0,
			in float4 inNormal : NORMAL0,
			in float4 inDiffuse : COLOR0,
			in float2 inTexCoord : TEXCOORD0,
			
			out float4 outPosition : SV_POSITION,
            out float4 outWorldPosition : POSITION0,
			out float4 outNormal : NORMAL0,
			out float4 outDiffuse : COLOR0,
			out float2 outTexCoord : TEXCOORD0)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    inPosition.y += waves(inTexCoord * 0.1, Parameter2.x, Parameter.x);
    
    outPosition = mul(inPosition, wvp);
    outWorldPosition = mul(inPosition, World);

    outDiffuse = inDiffuse;
    outNormal = inNormal;
    outTexCoord = inTexCoord;
}

