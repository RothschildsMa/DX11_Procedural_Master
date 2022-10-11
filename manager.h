#pragma once


class CScene;
class MyGui;

class CManager
{
private:
	static CScene*	m_Scene;
	static MyGui* m_MyGui;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static CScene* GetScene() { return m_Scene; }

};