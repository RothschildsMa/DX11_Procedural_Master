
#include "main.h"
#include "renderer.h"
#include "myGui.h"
#include "game_object.h"
#include "wave.h"

#include "camera.h"

#include "input.h"



void CWave::Init()
{
	float size = 5.0f;
	for (int z = 0; z < WAVE_Z; z++)
	{
		for (int x = 0; x < WAVE_X; x++)
		{
			m_Vertex[z * WAVE_X + x].Position.x = x / size - WAVE_X / size / 2;
			m_Vertex[z * WAVE_X + x].Position.z = -z / size + WAVE_Z / size / 2;
			m_Vertex[z * WAVE_X + x].Position.y = 0.0f;
			m_Vertex[z * WAVE_X + x].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			m_Vertex[z * WAVE_X + x].TexCoord = XMFLOAT2(x / size, z / size);
			m_Vertex[z * WAVE_X + x].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
		}
	}



	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * WAVE_X * WAVE_Z;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}


	unsigned int index[(WAVE_X * 2 + 2) * (WAVE_Z - 1) - 2];

	unsigned int i = 0;
	for (int z = 0; z < WAVE_Z - 1; z++)
	{
		for (int x = 0; x < WAVE_X; x++)
		{
			index[i] = (z + 1) * WAVE_X + x;
			i++;
			index[i] = z * WAVE_X + x;
			i++;
		}

		if (z == WAVE_Z - 2)
			break;

		index[i] = z * WAVE_X + WAVE_X - 1;
		i++;
		index[i] = (z + 2) * WAVE_X;
		i++;
	}


	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((WAVE_X * 2 + 2) * (WAVE_Z - 1) - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		CRenderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}



	m_Position = XMFLOAT3(0.0f, -3.0f, 0.0f);
	m_Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	m_Time = 0.0f;


	m_Shader = new CShader();
	m_Shader->Init("waveVS.cso", "wavePS.cso");

}


void CWave::Uninit()
{

	m_VertexBuffer->Release();
	m_IndexBuffer->Release();


}


void CWave::Update()
{
	m_Time += 1.0f / 60.0f;
}


void CWave::Draw()
{
	if (!MyGui::EnableWave) return;

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	CRenderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);



	// マトリクス設定
	XMMATRIX world;
	world = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	world *= XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	//CRenderer::SetWorldMatrix( &world );

	XMFLOAT4X4 worldf;
	DirectX::XMStoreFloat4x4(&worldf, world);

	m_Shader->SetWorldMatrix(&worldf);


	CCamera* camera = CCamera::GetInstance();
	m_Shader->SetViewMatrix(&camera->GetViewMatrix());
	m_Shader->SetProjectionMatrix(&camera->GetProjectionMatrix());
	m_Shader->SetCameraPosition(&camera->GetPosition());

	
	m_Shader->SetPrameter(XMFLOAT4(m_Time, MyGui::Alpha, MyGui::Phase, MyGui::Speed));
	m_Shader->SetPrameter2(XMFLOAT4(MyGui::Iter, MyGui::Height, 0.0f, 0.0f));

	LIGHT light;
	light.Direction = XMFLOAT4(0.0f, -cosf(MyGui::LightRotation), sinf(MyGui::LightRotation), 0.0f);
	light.Diffuse = COLOR(0.9f, 0.9f, 0.9f, 1.0f);
	light.Ambient = COLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_Shader->SetLight(light);

	m_Shader->Set();





	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	if (MyGui::WireframeWave)
	{
		CRenderer::SetRasterizerStateMode(D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);

		// ポリゴン描画
		CRenderer::GetDeviceContext()->DrawIndexed(((WAVE_X * 2 + 2) * (WAVE_Z - 1) - 2), 0, 0);

		CRenderer::SetRasterizerStateMode(D3D11_FILL_SOLID, D3D11_CULL_BACK);
	}
	else
	{
		// ポリゴン描画
		CRenderer::GetDeviceContext()->DrawIndexed(((WAVE_X * 2 + 2) * (WAVE_Z - 1) - 2), 0, 0);
	}

	

}



