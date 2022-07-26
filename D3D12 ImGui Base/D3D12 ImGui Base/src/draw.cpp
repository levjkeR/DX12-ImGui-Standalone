#include "draw.h"
#include "misc/fonts/FiraCodeFamily.h"

static ImFont* pFiraCodeMedium;
static ImFont* pFiraCodeBold;
static ImFont* pRobotoRegular;
static ImFont* pRobotoMedium;


// Our state
bool show_demo_window = false;
bool show_metrics_window = false;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void Draw::ShowCustomWindow(bool* p_open)
{
	// 1. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	{
		static float f = 0.0f;
		static float speed = 0.1f;
		static int i = 0;
		static int counter = 0;

		ImGui::SetNextWindowSize(ImVec2(800, 600));
		ImGui::Begin("DX12 Standalone", p_open);                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Metrics Window", &show_metrics_window);

		ImGui::SliderFloat("Slider float", &f, 0.0f, 1.0f); // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::Separator();
		ImGui::SliderFloat("Drag speed", &speed, 0.0f, 10.0f); // Edit 1 float using a slider from 0.0f to 10.0f
		ImGui::DragInt("Drag int", &i, speed, INT16_MIN, INT16_MAX, "%d, int"); // Edit 1 int using a drag from  to 100.0f
		ImGui::ColorEdit4("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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
	auto io = ImGui::GetIO();

	ImFontConfig fontConfig;
	fontConfig.FontDataOwnedByAtlas = false;

	pFiraCodeMedium = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(FiraCodeMedium), sizeof(FiraCodeMedium), 18.0f, &fontConfig);
	pFiraCodeBold = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(FiraCodeBold), sizeof(FiraCodeBold), 18.0f, &fontConfig);

	io.Fonts->AddFontDefault(&fontConfig);
}

