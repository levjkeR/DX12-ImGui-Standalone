#include "render.h"

void HideConsole()
{
	HWND hConsole;
	AllocConsole();
	hConsole = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(hConsole, 0);
}

int main()
{
	HideConsole();
	DXUI::RenderLoop();
	return 0;
}