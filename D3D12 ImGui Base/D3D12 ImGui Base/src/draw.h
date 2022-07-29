#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"


class Draw
{
public:
	static void ShowCustomWindow(bool* p_open = (bool*)0);
	static void CustomStyle();
	static void LoadFonts();
};
