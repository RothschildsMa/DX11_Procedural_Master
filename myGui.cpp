

#include "main.h"
#include"system_timer.h"

#include "myGui.h"

#include<string>

float MyGui::m_Parameter = 0.0f;
bool MyGui::m_Drawflag = false;
bool MyGui::ShadowDepthView = false;
bool MyGui::AStarMesh = false;
bool MyGui::EnableField = true;
bool MyGui::EnableWave = true;
bool MyGui::WireframeField = false;
bool MyGui::WireframeWave = false;

//atmosphere
float MyGui::LightRotation = -1.5f;
float MyGui::Distance = 0.003f;
int MyGui::MeScatter = 50;
float MyGui::Intensity = 0.5f;

//field
float MyGui::FieldMountain = 0.1f;
float MyGui::FieldAltitude = 10.0f;
int MyGui::FieldOctave = 5; 
int MyGui::NoiseOctave = 10;
float MyGui::NoiseSize = 10.0f;
float MyGui::Bump = 0.1f;
float MyGui::FieldColor[4] = { 0.21f, 0.21f, 0.21f, 0.0f };
float MyGui::FieldSideColor[4] = { 0.21f, 0.21f, 0.21f, 0.0f };
float MyGui::Fog = 0.3f;
float MyGui::FogColor[4] = { 0.0, 0.95, 1.0, 0.0f };

//wave
float MyGui::Alpha = 0.2f;
float MyGui::Phase = 6.0f;
float MyGui::Speed = 2.0f;
float MyGui::Height = 0.0f;
int MyGui::Iter = 5;


float MyGui::m_Dy = 0.0f;
float MyGui::m_Dz = 0.0f;

static char buf[256] = "";

static bool my_tool_active = true;

static double g_BaseTime = 0;
static double g_FPS = 0.0;
static int g_FrameCount = 0;
static int g_BaseFrame = 0;


void MyGui::Init()
{
	//セットアップ ImGui
	IMGUI_CHECKVERSION();
	
	ImGui::CreateContext();
	//ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(CRenderer::GetDevice(), CRenderer::GetDeviceContext());
	
	


	ImGui::StyleColorsClassic();

	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(1.0f, 0.4f, 1.0f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(1.0f, 0.4f, 1.0f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.0f, 10.0f, 10.0f, 1.0f));
	//ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
	m_Parameter = 0.0f;
	m_Drawflag = false;

	SystemTimer_Initialize();

	g_BaseTime = SystemTimer_GetTime();
	g_FrameCount = g_BaseFrame = 0;
	g_FPS = 0.0f;
}

void MyGui::Uninit()
{
}

void MyGui::Update()
{
	//フレームスタット
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//計測
	double time = SystemTimer_GetTime();
	if (time - g_BaseTime > 1.0)
	{
		g_FPS = (g_FrameCount - g_BaseFrame) / (time - g_BaseTime);
		g_BaseTime = time;
		g_BaseFrame = g_FrameCount;
	}

	g_FrameCount++;




}

void MyGui::Draw()
{
	//static int counter = 0;

	//ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);

	//if (ImGui::BeginMenuBar())
	//{
	//	if (ImGui::BeginMenu("File"))
	//	{
	//		if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
	//		if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
	//		if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
	//		ImGui::EndMenu();
	//	}
	//	ImGui::EndMenuBar();
	//}
	//

	//ImGui::Text("Hello, world %d", 123);
	//ImGui::Button(" + ");
	//ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
	//

	//ImGui::End();

	ImGui::Begin("Debug");

	ImGui::Text("FPS :");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 1, 0, 1), "%.1f", g_FPS);
	//ImGui::TextColored(ImVec4(0, 1, 0, 1), "%.1f", m_Dy);
	//ImGui::TextColored(ImVec4(0, 1, 0, 1), "%.1f", m_Dz);

	//ImGui::Checkbox("SphereCollision", &m_Drawflag);

	//ImGui::Checkbox("ShadowDepthView", &ShadowDepthView);
	//ImGui::Checkbox("PathFindingMesh", &AStarMesh);

	ImGui::End();


	ImGui::Begin("Option");

	if (ImGui::CollapsingHeader("Atmosphere"))
	{
		ImGui::SliderFloat("LightDirection", &LightRotation, -5.0f, 5.0f);
		ImGui::SliderFloat("Distance", &Distance, 0.001f, 0.05f);
		ImGui::SliderInt("Me Scattering", &MeScatter, 10, 100);
		ImGui::SliderFloat("Intensity", &Intensity, 0.1f, 0.5f);

	}

	if (ImGui::CollapsingHeader("Field"))
	{
		ImGui::Checkbox("EnableField", &EnableField);
		ImGui::Checkbox("WireframeField", &WireframeField);
		ImGui::SliderFloat("Mountain", &FieldMountain, 0.01f, 0.16f);
		ImGui::SliderFloat("Altitude", &FieldAltitude, 0.0f, 20.0f);
		ImGui::SliderInt("Iterations", &FieldOctave, 0, 10);
		ImGui::SliderInt("NoiseIterations", &NoiseOctave, 0, 10);
		ImGui::SliderFloat("NoiseIntensity", &NoiseSize, 0.0f, 20.0f);
		ImGui::SliderFloat("Bump", &Bump, 0.0f, 0.2f);

		ImGui::ColorEdit4("Side", FieldColor);
		//ImGui::ColorEdit4("Peak", FieldSideColor);
		ImGui::SliderFloat("Fog", &Fog, 0.0f, 3.0f);
		ImGui::ColorEdit4("FogColor", FogColor);
	}

	if (ImGui::CollapsingHeader("Wave"))
	{
		ImGui::Checkbox("EnableWave", &EnableWave);
		ImGui::Checkbox("WireframeWave", &WireframeWave);
		ImGui::SliderFloat("Alpha", &Alpha, 0.0f, 0.9f);
		ImGui::SliderFloat("Phase", &Phase, 0.0f, 20.0f);
		ImGui::SliderFloat("Speed", &Speed, 0.0f, 20.0f);
		ImGui::SliderFloat("Height", &Height, 0.0f, 1.0f);
		ImGui::SliderInt("WaveIterations", &Iter, 0, 10);

	}
	
	ImGui::End();

	


	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}
