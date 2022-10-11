


//2D疑似乱数
float random2(in float2 vec)
{
    //frac　整数部分を消す
    //dot   特に意味ない　float　2個を１個に変更 
    return frac(sin(dot(vec.xy, float2(12.9898, 78.233))) * 43.545);
}

//2D→2D疑似乱数
float2 hash(in float2 vec)
{
    vec = float2(dot(vec, float2(127.1, 311.7)),
                 dot(vec, float2(269.5, 183.3)));
    
    return frac(sin(vec) * (4378.545));
}


//ボロノイ図2D
float voronoi2(in float2 vec)
{
    float2 ivec = floor(vec);
    float2 fvec = frac(vec);
    
    float value = 1.0;
    
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            float2 offset = float2(x, y);
            float2 position;
            position = hash(ivec + offset);
            
            float dist = distance(position + offset, fvec);
            
            value = min(value, dist);
        }

    }
    
    return value;
    
}


float2 voronoi22(in float2 vec, in float parameter)
{
    float2 ivec = floor(vec);
    float2 fvec = frac(vec);
    
    float3 m = 8.0;
    
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            float2 offset = float2(x, y);
            float2 position;
            position = hash(ivec + offset);
            
            float2 r = offset - fvec + (0.5 + 0.5 * sin(parameter + 6.2831 * position));
            float d = dot(r, r);
            
            if (d < m.x)
                m = float3(d, position);
        }

    }
    
    return float2(sqrt(m.x), m.y + m.z);
    
}


//バリューノイズ2D
float valueNoise2(in float2 vec)
{
    float2 ivec = floor(vec);
    float2 fvec = frac(vec);
    
    float a = random2(ivec + float2(0.0, 0.0));
    float b = random2(ivec + float2(1.0, 0.0));
    float c = random2(ivec + float2(0.0, 1.0));
    float d = random2(ivec + float2(1.0, 1.0));
    
    fvec = smoothstep(0.0, 1.0, fvec);
    
    return lerp(lerp(a, b, fvec.x), lerp(c, d, fvec.x), fvec.y);
    
}

//パーリンノイズ
float perlinNoise2(in float2 vec)
{
    float2 ivec = floor(vec);
    float2 fvec = frac(vec);
    
    float a = dot(hash(ivec + float2(0.0, 0.0)) * 2.0 - 1.0, fvec - float2(0.0, 0.0));
    float b = dot(hash(ivec + float2(1.0, 0.0)) * 2.0 - 1.0, fvec - float2(1.0, 0.0));
    float c = dot(hash(ivec + float2(0.0, 1.0)) * 2.0 - 1.0, fvec - float2(0.0, 1.0));
    float d = dot(hash(ivec + float2(1.0, 1.0)) * 2.0 - 1.0, fvec - float2(1.0, 1.0));
    
    fvec = smoothstep(0.0, 1.0, fvec);
    
    return lerp(lerp(a, b, fvec.x), lerp(c, d, fvec.x), fvec.y);
    
    
}


//非整数ブラウン運動2D（フラクタルパーリンノイズ）
float fbm2(float2 vec, int octave, float offset = 0.0f)
{
    float value = 0.0;
    float amplitude = 1.0;
    
    for (int i = 0; i < octave; i++)
    {
        value += amplitude * perlinNoise2(vec + offset);
        vec *= 2.0;
        amplitude *= 0.5;
        
    }
    
    return value;

}

//非整数ブラウン運動2D（フラクタルパーリンノイズ）
float fbmabs2(in float2 vec, int octave)
{
    float value = 0.0;
    float amplitude = 1.0;
    
    for (int i = 0; i < octave; i++)
    {
        value += amplitude * -abs(perlinNoise2(vec));
        vec *= 2.0;
        amplitude *= 0.5;
        
    }
    
    return value;

}

float fbmabs22(in float2 vec, int octave)
{
    float value = 0.0;
    float amplitude = 1.0;
    
    for (int i = 0; i < octave; i++)
    {
        value += amplitude * abs(perlinNoise2(vec));
        vec *= 2.0;
        amplitude *= 0.5;
        
    }
    
    return value;

}


//=====================================================================


//3D疑似乱数
float random3(in float3 vec)
{
    //frac　整数部分を消す
    //dot   特に意味ない　float　2個を１個に変更 
    return frac(sin(dot(vec.xy, float2(12.9898, 78.233))) * 43.545);
}

//3D→3D疑似乱数
float3 random33(in float3 vec)
{
    vec = float3(dot(vec, float3(127.1, 311.7, 245.4)),
                 dot(vec, float3(269.5, 183.3, 131.2)),
                 dot(vec, float3(522.3, 243.1, 523.4)));
    
    return frac(sin(vec) * (4378.545));
}

//パーリンノイズ3D
float perlinNoise3(in float3 vec)
{
    float3 ivec = floor(vec);
    float3 fvec = frac(vec);
    
    float a = dot(random33(ivec + float3(0.0, 0.0, 0.0)) * 2.0 - 1.0, fvec - float3(0.0, 0.0, 0.0));
    float b = dot(random33(ivec + float3(1.0, 0.0, 0.0)) * 2.0 - 1.0, fvec - float3(1.0, 0.0, 0.0));
    float c = dot(random33(ivec + float3(0.0, 1.0, 0.0)) * 2.0 - 1.0, fvec - float3(0.0, 1.0, 0.0));
    float d = dot(random33(ivec + float3(1.0, 1.0, 0.0)) * 2.0 - 1.0, fvec - float3(1.0, 1.0, 0.0));
    
    float e = dot(random33(ivec + float3(0.0, 0.0, 1.0)) * 2.0 - 1.0, fvec - float3(0.0, 0.0, 1.0));
    float f = dot(random33(ivec + float3(1.0, 0.0, 1.0)) * 2.0 - 1.0, fvec - float3(1.0, 0.0, 1.0));
    float g = dot(random33(ivec + float3(0.0, 1.0, 1.0)) * 2.0 - 1.0, fvec - float3(0.0, 1.0, 1.0));
    float h = dot(random33(ivec + float3(1.0, 1.0, 1.0)) * 2.0 - 1.0, fvec - float3(1.0, 1.0, 1.0));
    
    
    fvec = smoothstep(0.0, 1.0, fvec);
    float v1 = lerp(lerp(a, b, fvec.x), lerp(c, d, fvec.x), fvec.y);
    float v2 = lerp(lerp(e, f, fvec.x), lerp(g, h, fvec.x), fvec.y);
    float v3 = lerp(v1, v2, fvec.z);
    
    return v3;
    
    
}

//非整数ブラウン運動3D（フラクタルパーリンノイズ）
float fbm3(in float3 vec, int octave)
{
    float value = 0.0;
    float amplitude = 1.0;
    
    for (int i = 0; i < octave; i++)
    {
        value += amplitude * perlinNoise3(vec);
        vec *= 2.0;
        amplitude *= 0.5;
        
    }
    
    return value;

}




float voronoi3(in float3 vec)
{
    float3 ivec = floor(vec);
    float3 fvec = frac(vec);
    
    float value = 1;
    
    
    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            for (int z = -1; z <= 1; z++)
            {
                float3 offset = float3(x, y, z);
                float3 position;
                position = random33(ivec + offset);
            
                float dist = distance(position + offset, fvec);
            
                value = min(value, dist);
            }

        }
    }
    
    return value;
    
}



//float3 noise_deriv(in float2 p)
//{
//    float2 i = floor(p);
//    float2 f = frac(p);
//    float2 u = f * f * (3.0 - 2.0 * f);
    
//    float a = hash(i + float2(0.0, 0.0));
//    float b = hash(i + float2(1.0, 0.0));
//    float c = hash(i + float2(0.0, 1.0));
//    float d = hash(i + float2(1.0, 1.0));
//    float h1 = lerp(a, b, u.x);
//    float h2 = lerp(c, d, u.x);
                                  
//    return float3(abs(lerp(h1, h2, u.y)),
//               6.0 * f * (1.0 - f) * (float2(b - a, c - a) + (a - b - c + d) * u.yx));
//}

float mandelbrot(in float2 c)
{
    float2 z = 0.0;
    
    for (int i = 0; i < 1000; i++)
    {
        z = float2(z.x * z.x - z.y * z.y + c.x, 2.0 * z.x * z.y + c.y);
        
        if (dot(z, z) > 1000.0)
        {
            //発散
            return i;
        }

    }
    
    //発散しない
    return 0.0;
    
}


float3 hsv2rgb(float3 hsv)
{
    float h = hsv.x;
    float s = hsv.y;
    float v = hsv.z;
    
    float r = v;
    float g = v;
    float b = v;
    if (s > 0.0f)
    {
        h *= 6.0f;
        
        int i = (int) h;
        float f = h - (float) i;
        
        switch (i)
        {
            default:
            case 0:
                g *= 1 - s * (1 - f);
                b *= 1 - s;
                break;
            case 1:
                r *= 1 - s * f;
                b *= 1 - s;
                break;
            case 2:
                r *= 1 - s;
                b *= 1 - s * (1 - f);
                break;
            case 3:
                r *= 1 - s;
                g *= 1 - s * f;
                break;
            case 4:
                r *= 1 - s * (1 - f);
                g *= 1 - s;
                break;
            case 5:
                g *= 1 - s;
                b *= 1 - s * f;
                break;
        }
    }
    
    return float3(r, g, b);
}