#pragma once

#include "main.h"
#include "renderer.h"


#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

class MyGui
{
private:
	static float m_Parameter;
	static bool m_Drawflag;
	static float m_Dy;
	static float m_Dz;

public:
	MyGui(){}

	static bool ShadowDepthView;
	static bool AStarMesh;
	static bool EnableField;
	static bool EnableWave;
	static bool WireframeWave;
	static bool WireframeField;

	//atmosphere
	static float LightRotation;
	static float Distance;
	static int MeScatter;
	static float Intensity;


	//field
	static float FieldMountain;
	static float FieldAltitude;
	static int FieldOctave;
	static int NoiseOctave;
	static float Bump;
	static float NoiseSize;
	static float FieldColor[4];
	static float FieldSideColor[4];
	static float Fog;
	static float FogColor[4];

	//wave
	static float Alpha;
	static float Phase;
	static float Speed; 
	static float Height;
	static int Iter;

	void Init();
	void Uninit();
	void Update();
	void Draw();

	static float GetParam() { return m_Parameter; }
	static bool GetDrawFlag() { return m_Drawflag; }
	static float GetDy() { return m_Dy; }
	static void SetDy(float y) { m_Dy = y; }
	static void SetDz(float z) { m_Dz = z; }

};
