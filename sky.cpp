
#include "main.h"
#include "renderer.h"
#include "shader.h"

#include "myGui.h"
#include "game_object.h"
#include "texture.h"
#include "camera.h"
#include "input.h"
#include "sky.h"





void CSky::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3( 1000.0f, 0.0f, 1000.0f);
	vertex[0].Normal   = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	vertex[0].Diffuse  = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[0].TexCoord = XMFLOAT2( 100.0f, 0.0f );

	vertex[1].Position = XMFLOAT3( -1000.0f, 0.0f, 1000.0f);
	vertex[1].Normal   = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	vertex[1].Diffuse  = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[1].TexCoord = XMFLOAT2( 0.0f, 0.0f );

	vertex[2].Position = XMFLOAT3( 1000.0f, 0.0f, -1000.0f);
	vertex[2].Normal   = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	vertex[2].Diffuse  = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[2].TexCoord = XMFLOAT2( 100.0f, 100.0f );

	vertex[3].Position = XMFLOAT3( -1000.0f, 0.0f, -1000.0f);
	vertex[3].Normal   = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	vertex[3].Diffuse  = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	vertex[3].TexCoord = XMFLOAT2( 0.0f, 100.0f );


	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( VERTEX_3D ) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory( &sd, sizeof(sd) );
	sd.pSysMem = vertex;

	CRenderer::GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );


	//m_Texture = new CTexture();
	//m_Texture->Load("data/TEXTURE/sky.tga");
		


	m_Position = XMFLOAT3( 0.0f, 20.0f, 0.0f );
	m_Rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	m_Scale = XMFLOAT3( 1.0f, 1.0f, 1.0f );



	m_Shader = new CShader();
	m_Shader->Init("shaderSkyVS.cso", "shaderSkyPS.cso");



	m_Time = 0.0f;

}


void CSky::Uninit()
{

	m_VertexBuffer->Release();


	//m_Texture->Unload();
	//delete m_Texture;

}


void CSky::Update()
{
	m_Time += 1.0f / 60.0f;


}


void CSky::Draw()
{
	//if(MyGui::GetDy() > 0.7) return;

	// 頂点バッファ設定
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	CRenderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// テクスチャ設定
	//CRenderer::SetTexture( m_Texture );

	// マトリクス設定
	XMMATRIX world;
	world = XMMatrixScaling( m_Scale.x, m_Scale.y, m_Scale.z );
	world *= XMMatrixRotationRollPitchYaw( m_Rotation.x, m_Rotation.y, m_Rotation.z );
	world *= XMMatrixTranslation( m_Position.x, m_Position.y, m_Position.z );


	XMFLOAT4X4 worldf;
	DirectX::XMStoreFloat4x4(&worldf, world);

	m_Shader->SetWorldMatrix(&worldf);


	CCamera* camera = CCamera::GetInstance();
	m_Shader->SetViewMatrix(&camera->GetViewMatrix());
	m_Shader->SetProjectionMatrix(&camera->GetProjectionMatrix());
	m_Shader->SetCameraPosition(&camera->GetPosition());

	m_Shader->SetPrameter( XMFLOAT4(m_Time, 0.0f, 0.0f, 0.0f));

	m_Shader->Set();

	LIGHT light;
	light.Direction = XMFLOAT4(0.0f, -cosf(MyGui::LightRotation), sinf(MyGui::LightRotation), 0.0f);
	light.Diffuse = COLOR(0.9f, 0.9f, 0.9f, 1.0f);
	light.Ambient = COLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_Shader->SetLight(light);



	// プリミティブトポロジ設定
	CRenderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// ポリゴン描画
	CRenderer::GetDeviceContext()->Draw( 4, 0 );

}