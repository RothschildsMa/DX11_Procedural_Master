#pragma once


struct CONSTANT
{

	XMFLOAT4X4				WorldMatrix;
	XMFLOAT4X4				ViewMatrix;
	XMFLOAT4X4				ProjectionMatrix;

	XMFLOAT4				CameraPosition;
	XMFLOAT4				Parameter;
	XMFLOAT4				Parameter2;
	XMFLOAT4                Color;
	XMFLOAT4                Color2;
	XMFLOAT4                Color3;

};

class CShader
{
private:
	ID3D11VertexShader*     m_VertexShader;
	ID3D11PixelShader*      m_PixelShader;
	ID3D11InputLayout*      m_VertexLayout;

	ID3D11Buffer*			m_ConstantBuffer;
	CONSTANT				m_Constant;

	ID3D11Buffer*			m_LightBuffer;
	LIGHT					m_Light;

public:
	void Init( const char* VertexShader, const char* PixelShader );
	void Uninit();
	void Set();

	void SetWorldMatrix(XMFLOAT4X4* WorldMatrix) { m_Constant.WorldMatrix = Transpose( WorldMatrix ); }
	void SetViewMatrix(XMFLOAT4X4* ViewMatrix) { m_Constant.ViewMatrix = Transpose( ViewMatrix ); }
	void SetProjectionMatrix(XMFLOAT4X4* ProjectionMatrix) { m_Constant.ProjectionMatrix = Transpose( ProjectionMatrix ); }
	void SetPrameter(XMFLOAT4 Parameter) { m_Constant.Parameter = Parameter; }
	void SetPrameter2(XMFLOAT4 Parameter2) { m_Constant.Parameter2 = Parameter2; }
	void SetColorBuffer(XMFLOAT4 Color) { m_Constant.Color = Color; }
	void SetColorBuffer(float* Color) { m_Constant.Color = { Color[0],Color[1],Color[2],Color[3] }; }
	void SetColorBuffer2(XMFLOAT4 Color2) { m_Constant.Color2 = Color2; }
	void SetColorBuffer2(float* Color2) { m_Constant.Color2 = { Color2[0],Color2[1],Color2[2],Color2[3] }; }
	void SetColorBuffer3(float* Color3) { m_Constant.Color3 = { Color3[0],Color3[1],Color3[2],Color3[3] }; }

	void SetLight(LIGHT Light) { m_Light = Light; }

	void SetCameraPosition(XMFLOAT3* CameraPosition)
	{
		m_Constant.CameraPosition.x = CameraPosition->x;
		m_Constant.CameraPosition.y = CameraPosition->y;
		m_Constant.CameraPosition.z = CameraPosition->z;
		m_Constant.CameraPosition.w = 1.0f;
	}


	XMFLOAT4X4 Transpose(XMFLOAT4X4* Matrix)
	{
		XMFLOAT4X4 outMatrix;

		outMatrix._11 = Matrix->_11;
		outMatrix._12 = Matrix->_21;
		outMatrix._13 = Matrix->_31;
		outMatrix._14 = Matrix->_41;

		outMatrix._21 = Matrix->_12;
		outMatrix._22 = Matrix->_22;
		outMatrix._23 = Matrix->_32;
		outMatrix._24 = Matrix->_42;

		outMatrix._31 = Matrix->_13;
		outMatrix._32 = Matrix->_23;
		outMatrix._33 = Matrix->_33;
		outMatrix._34 = Matrix->_43;

		outMatrix._41 = Matrix->_14;
		outMatrix._42 = Matrix->_24;
		outMatrix._43 = Matrix->_34;
		outMatrix._44 = Matrix->_44;

		return outMatrix;
	}

};