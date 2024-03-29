
#include "main.h"
#include "renderer.h"
#include "myGui.h"
#include "game_object.h"
#include "field.h"
#include "texture.h"

#include "camera.h"

#include "input.h"



void CField::Init()
{
	for (int z = 0; z < FIELD_Z; z++)
	{
		for (int x = 0; x < FIELD_X; x++)
		{
			m_Vertex[z * FIELD_X + x].Position.x = x / 5.0f - FIELD_X / 5.0f / 2;
			m_Vertex[z * FIELD_X + x].Position.z = -z / 5.0f + FIELD_Z / 5.0f / 2;
			m_Vertex[z * FIELD_X + x].Position.y = 0.0f;
			m_Vertex[z * FIELD_X + x].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			m_Vertex[z * FIELD_X + x].TexCoord = XMFLOAT2(x / 5.0f, z / 5.0f);
			m_Vertex[z * FIELD_X + x].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		}
	}



	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * FIELD_X * FIELD_Z;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}

	//インデックスバッファ生成
	unsigned int index[(FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2];

	unsigned int i = 0;
	for (int z = 0; z < FIELD_Z - 1; z++)
	{
		for (int x = 0; x < FIELD_X; x++)
		{
			index[i] = (z + 1) * FIELD_X + x;
			i++;
			index[i] = z * FIELD_X + x;
			i++;
		}

		if (z == FIELD_Z - 2)
			break;

		index[i] = z * FIELD_X + FIELD_X - 1;
		i++;
		index[i] = (z + 2) * FIELD_X;
		i++;
	}


	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}


	m_Texture = new CTexture();
	m_Texture->Load("data/TEXTURE/field2.tga");




	m_Position = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Scale = XMFLOAT3( 1.0f, 1.0f, 1.0f );

	m_Time = 0.0f;


	m_Shader = new CShader();
	m_Shader->Init("shaderFieldVS.cso", "shaderFieldPS.cso");
	

}


void CField::Uninit()
{

	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	m_Texture->Unload();
	delete m_Texture;

}


void CField::Update()
{
	m_Time += 1.0f / 60.0f;
}


void CField::Draw()
{
	if (!MyGui::EnableField) return;

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// テクスチャ設定
	//CRenderer::SetTexture( m_Texture, 0 );

	// マトリクス設定
	XMMATRIX world;
	world = XMMatrixScaling( m_Scale.x, m_Scale.y, m_Scale.z );
	world *= XMMatrixRotationRollPitchYaw( m_Rotation.x, m_Rotation.y, m_Rotation.z );
	world *= XMMatrixTranslation( m_Position.x, m_Position.y, m_Position.z );
	//CRenderer::SetWorldMatrix( &world );

	XMFLOAT4X4 worldf;
	DirectX::XMStoreFloat4x4(&worldf, world);

	m_Shader->SetWorldMatrix(&worldf);


	CCamera* camera = CCamera::GetInstance();
	m_Shader->SetViewMatrix(&camera->GetViewMatrix());
	m_Shader->SetProjectionMatrix(&camera->GetProjectionMatrix());
	m_Shader->SetCameraPosition(&camera->GetPosition());
	
	m_Shader->SetPrameter(XMFLOAT4(MyGui::FieldMountain, MyGui::FieldOctave, MyGui::FieldAltitude, MyGui::Fog));
	m_Shader->SetPrameter2(XMFLOAT4(MyGui::NoiseOctave, MyGui::NoiseSize, MyGui::Bump, 0.0f));
	m_Shader->SetColorBuffer(MyGui::FieldColor);
	m_Shader->SetColorBuffer2(MyGui::FieldSideColor);
	m_Shader->SetColorBuffer3(MyGui::FogColor);
	
	LIGHT light;
	light.Direction = XMFLOAT4(1.3f, -cosf(MyGui::LightRotation), sinf(MyGui::LightRotation), 0.0f);
	light.Diffuse = COLOR(0.9f, 0.9f, 0.9f, 1.0f);
	light.Ambient = COLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_Shader->SetLight(light);

	m_Shader->Set();

	



	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );


	if (MyGui::WireframeField)
	{
		CRenderer::SetRasterizerStateMode(D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);

		// ポリゴン描画
		CRenderer::GetDeviceContext()->DrawIndexed(((FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2), 0, 0);

		CRenderer::SetRasterizerStateMode(D3D11_FILL_SOLID, D3D11_CULL_BACK);

	}
	else
	{
		// ポリゴン描画
		CRenderer::GetDeviceContext()->DrawIndexed(((FIELD_X * 2 + 2) * (FIELD_Z - 1) - 2), 0, 0);
	}
}



