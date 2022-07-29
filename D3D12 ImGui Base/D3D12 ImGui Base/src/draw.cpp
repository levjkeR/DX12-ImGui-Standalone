#include "draw.h"
#include "misc/fonts/FontAwesome6.h"
#include "misc/fonts/ForkAwesome.h"
#include "misc/fonts/MaterialDesign.h"
#include <shlobj.h>
#include <string>
#include <filesystem>
#include <map>
#include <iostream>
#define NORMAL_SIZE 18
#define MEDIUM_SIZE 21
#define BIG_SIZE 28
#define ICON_SIZE 32

struct FiraCodeFamily
{
	ImFont* pNormalFiraCodeMedium;
	ImFont* pMediumFiraCodeMedium;
	ImFont* pBigFiraCodeMedium;

	ImFont* pNormalFiraCodeBold;
	ImFont* pMediumFiraCodeBold;
	ImFont* pBigFiraCodeBold;
};

struct IconFamily
{
	ImFont* pNormalFontAwesome;
	
	ImFont* pNormalFontAwesomeSolid;
	
	ImFont* pNormalForkAwesome;

	ImFont* pNormalMaterialDesign;
};

static FiraCodeFamily FiraCode;
static IconFamily Icons;

// Our state
bool show_demo_window = false;
bool show_metrics_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

bool getLocalFontFolder(char* path)
{
	char out[MAX_PATH];

	if (!SHGetSpecialFolderPathA(NULL, out, CSIDL_LOCAL_APPDATA, 0))
		return 0;

	strcat_s(out, sizeof(out), "\\Microsoft\\Windows\\Fonts");

	std::filesystem::path local = out;
	if (std::filesystem::exists(local))
		strcpy_s(path, sizeof(out), out); return 1;
	return 0;
}


struct tab_anim
{
	int hovered_anim;
	int active_anim;
};

bool TabEx(const char* label, bool selected, const ImVec2& size_arg, ImGuiButtonFlags flags)
{
	using namespace ImGui;
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	/*static std::map <ImGuiID, tab_anim> anim;
	auto it_anim = anim.find(id);
	if (it_anim == anim.end())
	{
		anim.insert({ id, {0, 0} });
		it_anim = anim.find(id);
	}

	if (hovered)
		it_anim->second.hovered_anim += 8 * (1.f - ImGui::GetIO().DeltaTime);
	else
		it_anim->second.hovered_anim -= 8 * (1.f - ImGui::GetIO().DeltaTime);

	if (it_anim->second.hovered_anim > 50)
		it_anim->second.hovered_anim = 50;
	else if (it_anim->second.hovered_anim < 0)
		it_anim->second.hovered_anim = 0;

	if (selected)
		it_anim->second.active_anim += 16 * (1.f - ImGui::GetIO().DeltaTime);
	else
		it_anim->second.active_anim -= 16 * (1.f - ImGui::GetIO().DeltaTime);

	if (it_anim->second.active_anim > 255)
		it_anim->second.active_anim = 255;
	else if (it_anim->second.active_anim < 0)
		it_anim->second.active_anim = 0;*/


	// Render
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, ImColor(40, 40, 40, 255), false, 0.f);

	if (g.LogEnabled)
		LogSetNextTextDecoration("[", "]");

	RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x, bb.Min.y + style.FramePadding.y), ImVec2(bb.Max.x - style.FramePadding.x, bb.Max.y - style.FramePadding.y), label, NULL, &label_size, style.ButtonTextAlign, &bb);


	IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
	return pressed;
}

bool Tab(const char* label, bool selected, const ImVec2& size_arg) {
	return TabEx(label, selected, size_arg, ImGuiButtonFlags_None);
}







bool ButtonEx(const char* label, bool* selected,const ImVec2& size_arg, ImGuiButtonFlags flags, const char* icon = nullptr, ImFont* iconFont = nullptr)
{
	using namespace ImGui;
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = CalcTextSize(label, NULL, true);
	ImVec2 item_size = label_size;
	ImVec2 icon_size;

	if (icon && iconFont) {
		ImGui::PushFont(iconFont);
		icon_size = CalcTextSize(icon, NULL, true);
		if (icon_size.y > label_size.y)
			item_size.y = icon_size.y;
		item_size.x += icon_size.x + style.FramePadding.x;
		ImGui::PopFont();
	}
	else {
		icon_size = ImVec2(0, 0);
	}

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = CalcItemSize(size_arg, item_size.x + style.FramePadding.x * 2.0f, item_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
	ItemSize(size, style.FramePadding.y);
	if (!ItemAdd(bb, id))
		return false;

	if (g.LastItemData.InFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;

	bool hovered, held;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held, flags);

	// Render
	const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

	if (g.LogEnabled)
		LogSetNextTextDecoration("[", "]");
	if (icon && iconFont) {
		ImGui::PushFont(iconFont);
		RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x, bb.Min.y + style.FramePadding.y), ImVec2(icon_size.x, icon_size.y), icon, NULL, &icon_size, style.ButtonTextAlign, &bb);
		PopFont();
	}
	RenderTextClipped(ImVec2(bb.Min.x + style.FramePadding.x*2.0f + icon_size.x, bb.Min.y + style.FramePadding.y), ImVec2(bb.Max.x - style.FramePadding.x, bb.Max.y - style.FramePadding.y), label, NULL, &label_size, style.ButtonTextAlign, &bb);

	// Automatically close popups
	//if (pressed && !(flags & ImGuiButtonFlags_DontClosePopups) && (window->Flags & ImGuiWindowFlags_Popup))
	//    CloseCurrentPopup();

	IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
	return pressed;
}




bool SelectButton(const char* label, bool* selected,const ImVec2& size_arg = ImVec2(0, 0), const char* icon = nullptr, ImFont* iconFont = nullptr)
{
	return ButtonEx(label, selected,size_arg, ImGuiButtonFlags_None, icon, iconFont);
}





void Draw::ShowCustomWindow(bool* p_open)
{


	// 1. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	//    p_open for close application.
	{
		static float speed = 0.5f;
		static int i = 0;

		ImGui::SetWindowSize(ImVec2(800, 650));		// Set initial window size
		ImGui::Begin("DX12 Standalone1", p_open);		// Create a window called "DX12 Standalone" and append into it.

		ImGui::Text("This is ImGui Base app use D3D12 to create GUI");		// Display eng text
		ImGui::Text(u8"Можно использовать для создания приложений с графическим интерфейсом");		// Display rus text
		ImGui::Checkbox("Demo Window", &show_demo_window);		// Edit bools storing our window open/close state
		ImGui::Checkbox("Metrics Window", &show_metrics_window);
		ImGui::NewLine();
		ImGui::SliderFloat("Drag Speed", &speed, 0.1f, 25.0f); // Edit 1 float using a slider from 0.0f to 10.0f
		ImGui::DragInt("Drag to Change", &i, speed, INT16_MIN, INT16_MAX, "%d"); // Edit 1 int using a drag item 
		ImGui::ColorEdit4("Color", (float*)&clear_color); // Edit 4 floats representing a color
		SelectButton("Button /w icon", NULL, ImVec2(500, 0), ICON_FK_GIT, Icons.pNormalForkAwesome);

		ImGui::End();
	}



	// 1. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static float speed = 0.1f;
		static int i = 0;
		static int counter = 0;
		static char path[256];
		size_t req_sz;

		ImGui::SetNextWindowSize(ImVec2(800, 600));
		ImGui::Begin("DX12 Standalone", p_open);                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Text(u8"Это какой-то полезный текст");               // Display some russian text
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Metrics Window", &show_metrics_window);

		ImGui::SliderFloat("Slider float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::Separator();
		ImGui::SliderFloat("Drag speed", &speed, 0.0f, 10.0f); // Edit 1 float using a slider from 0.0f to 10.0f
		ImGui::DragInt("Drag int", &i, speed, INT16_MIN, INT16_MAX, "%d, int"); // Edit 1 int using a drag from  to 100.0f
		ImGui::ColorEdit4("clear color", (float*)&clear_color); // Edit 3 floats representing a color
		

		if (getLocalFontFolder(path))
			ImGui::Text("%s", path);

		ImGui::Text("Fail");

		if (ImGui::Button(ICON_FK_GITHUB_SQUARE " Boom!"))                          // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		static bool s = 0;

		ImGui::PushFont(Icons.pNormalFontAwesome);
		if (SelectButton("Button /w icon", &s, ImVec2(0, 0), ICON_FK_GIT, Icons.pNormalForkAwesome));
		ImGui::PopFont();


		ImGui::End();
	}

	// 2. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 3. Show metrics window
	if (show_metrics_window)
		ImGui::ShowMetricsWindow();
	
}
	
void Draw::CustomStyle()
{

	ImGuiStyle* style = &ImGui::GetStyle();

	// Borders
	style->WindowBorderSize = 1.0f;
	style->FrameBorderSize = 0.0f;
	style->PopupBorderSize = 0.0f;

	// Rounding
	style->WindowRounding = 0.0f;
	style->ChildRounding = 0.0f;
	style->FrameRounding = 0.0f;
	style->PopupRounding = 0.0f;
	style->ScrollbarRounding = 0.0f;
	style->GrabRounding = 0.0f;
	style->LogSliderDeadzone = 0.0f;
	style->TabRounding = 0.0f;

	// Aligment
	style->WindowTitleAlign = ImVec2(0.5f, 0.25f);

	auto colors = ImGui::GetStyle().Colors;

	/*static ImColor ActiveColor = ImColor(17, 95, 226, 255);
	static ImColor HoveredColor = ImColor(54, 123, 240, 255);*/
	
	static ImColor ActiveColor = ImColor(60, 60, 60, 255);
	static ImColor HoveredColor = ImColor(41, 41, 41, 255);

	colors[ImGuiCol_WindowBg] = ImColor(18, 18, 18, 255);
	colors[ImGuiCol_ChildBg] = ImColor(22, 22, 22, 255);

	colors[ImGuiCol_Border] = ImColor(255, 255, 255, 5);

	colors[ImGuiCol_FrameBg] = ImColor(26, 26, 26, 255);
	colors[ImGuiCol_FrameBgActive] = ImColor(40, 40, 40, 255);
	colors[ImGuiCol_FrameBgHovered] = ImColor(34, 34, 34, 255);

	colors[ImGuiCol_TitleBg] = ImColor(18, 18, 18, 255);
	colors[ImGuiCol_TitleBgCollapsed] = ImColor(26, 26, 26, 255);
	colors[ImGuiCol_TitleBgActive] = ImColor(26, 26, 26, 255);

	colors[ImGuiCol_Tab] = ImColor(34, 34, 34, 255);
	colors[ImGuiCol_TabActive] = ActiveColor;
	colors[ImGuiCol_TabHovered] = HoveredColor;

	colors[ImGuiCol_Button] = ImColor(26, 26, 26, 255);
	colors[ImGuiCol_ButtonHovered] = HoveredColor;
	colors[ImGuiCol_ButtonActive] = ActiveColor;

	colors[ImGuiCol_SliderGrab] = HoveredColor;
	colors[ImGuiCol_SliderGrabActive] = ActiveColor;

	//colors[ImGuiCol_Border] = ImColor(0, 0, 0, 0);
	colors[ImGuiCol_Separator] = ImColor(34, 34, 34, 255);

	colors[ImGuiCol_ResizeGrip] = ImColor(40, 40, 40, 255);
	colors[ImGuiCol_ResizeGripActive] = ActiveColor;
	colors[ImGuiCol_ResizeGripHovered] = HoveredColor;


	colors[ImGuiCol_ScrollbarBg] = ImColor(24, 24, 24, 255);
	colors[ImGuiCol_ScrollbarGrab] = ImColor(34, 34, 34, 255);
	colors[ImGuiCol_ScrollbarGrabActive] = ActiveColor;
	colors[ImGuiCol_ScrollbarGrabHovered] = HoveredColor;

	colors[ImGuiCol_Header] = ImColor(26, 26, 26, 255);
	colors[ImGuiCol_HeaderActive] = ImColor(40, 40, 40, 255);
	colors[ImGuiCol_HeaderHovered] = ImColor(34, 34, 34, 255);

	colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);
}

void Draw::LoadFonts()
{
	auto io = ImGui::GetIO(); (void)io;
	char fontDir[MAX_PATH];

	if (!getLocalFontFolder(fontDir))
		return;

	ImFontConfig fontConfig;
	fontConfig.FontDataOwnedByAtlas = false;
	fontConfig.OversampleH = 3;
	fontConfig.OversampleV = 3;


	// Load FiraCode Family with different size + Cyrillic
	std::filesystem::path fonts = fontDir;

	//if (std::filesystem::exists(fonts / "FiraCode-Medium.ttf")) {
	FiraCode.pNormalFiraCodeMedium = io.Fonts->AddFontFromFileTTF((fonts / "FiraCode-Medium.ttf").u8string().c_str(), NORMAL_SIZE, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

	
	
	static ImWchar icon_ranges[] = { 0xe005, 0xf8ff, 0 };
	//if (std::filesystem::exists(fonts / "fa-regular-400.ttf") && std::filesystem::exists(fonts / "fa-solid-900.ttf")) {
	Icons.pNormalFontAwesome = io.Fonts->AddFontFromFileTTF((fonts / "fa-regular-400.ttf").u8string().c_str(), ICON_SIZE, &fontConfig, icon_ranges);
	fontConfig.MergeMode = true;
	Icons.pNormalFontAwesome = io.Fonts->AddFontFromFileTTF((fonts / "fa-solid-900.ttf").u8string().c_str(), ICON_SIZE, &fontConfig, icon_ranges);
	fontConfig.MergeMode = false;

	icon_ranges[0] = ICON_MIN_MD; icon_ranges[1] = ICON_MAX_MD;
	Icons.pNormalForkAwesome = io.Fonts->AddFontFromFileTTF((fonts / FONT_ICON_FILE_NAME_MD).u8string().c_str(), ICON_SIZE, &fontConfig, icon_ranges);

	icon_ranges[0] = ICON_MIN_FK; icon_ranges[1] = ICON_MAX_FK;
	Icons.pNormalForkAwesome = io.Fonts->AddFontFromFileTTF((fonts / FONT_ICON_FILE_NAME_FK).u8string().c_str(), ICON_SIZE, &fontConfig, icon_ranges);
	
	/*if (std::filesystem::exists(fonts)) {
		Icons.pNormalFontAwesome = io.Fonts->AddFontFromFileTTF(fonts.u8string().c_str(), NORMAL_SIZE, &fontConfig, icon_ranges);
	}*/
	//FiraCode.pNormalFiraCodeMedium = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(FiraCodeMedium), sizeof(FiraCodeMedium), NORMAL_SIZE, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());
	//FiraCode.pMediumFiraCodeMedium = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(FiraCodeMedium), sizeof(FiraCodeMedium), MEDIUM_SIZE, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());
	//FiraCode.pBigFiraCodeMedium = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(FiraCodeMedium), sizeof(FiraCodeMedium), BIG_SIZE, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

	//FiraCode.pNormalFiraCodeBold = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(FiraCodeBold), sizeof(FiraCodeBold), NORMAL_SIZE, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());
	//FiraCode.pMediumFiraCodeBold = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(FiraCodeBold), sizeof(FiraCodeBold), MEDIUM_SIZE, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());
	//FiraCode.pBigFiraCodeBold = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(FiraCodeBold), sizeof(FiraCodeBold), BIG_SIZE, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());
	
	// Load iconic fonts
	/*ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	Icons.pNormalFontAwesome = io.Fonts->AddFontFromMemoryTTF((void*)(FontAwesomeRegular), sizeof(FontAwesomeRegular), NORMAL_SIZE, &fontConfig, icon_ranges);
	Icons.pNormalFontAwesomeSolid = io.Fonts->AddFontFromMemoryTTF((void*)(FontAwesomeSolid), sizeof(FontAwesomeSolid), NORMAL_SIZE, &fontConfig, icon_ranges);*/



}

