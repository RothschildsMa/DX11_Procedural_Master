#pragma once

#include "shader.h"



class CWave : public CGameObject
{
private:


	ID3D11Buffer*	m_VertexBuffer = NULL;
	ID3D11Buffer*	m_IndexBuffer = NULL;


	static const int WAVE_X = 256;
	static const int WAVE_Z = 256;

	VERTEX_3D m_Vertex[WAVE_X * WAVE_Z];

	CShader*		m_Shader;
	float m_Time;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();


};