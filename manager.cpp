
#include "main.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "shader.h"
#include "myGui.h"
#include "scene.h"
#include "field.h"
#include "polygon.h"
#include "model.h"



CScene*	CManager::m_Scene;
MyGui*	CManager::m_MyGui;

void CManager::Init()
{

	CRenderer::Init();
	CInput::Init();

	m_MyGui = new MyGui();
	m_MyGui->Init();

	m_Scene = new CScene();
	m_Scene->Init();



}

void CManager::Uninit()
{

	m_Scene->Uninit();
	delete m_Scene;

	m_MyGui->Uninit();
	delete m_MyGui;

	CInput::Uninit();
	CRenderer::Uninit();

}

void CManager::Update()
{

	CInput::Update();

	m_MyGui->Update();

	m_Scene->Update();

}

void CManager::Draw()
{

	CRenderer::BeginShadow();

	m_Scene->DrawShadow();


	CRenderer::Begin();

	m_Scene->Draw();

	m_MyGui->Draw();

	CRenderer::End();

}
