#include "gui.h"
#include <thread>

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int commandShow)
{
	// Create gui
	gui::createHWindow("My Menu", "My Menu Class");
	gui::createDevice();
	gui::createImGui();

	while (gui::exit)
	{
		gui::beginRender();
		gui::render();
		gui::endRender();

		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	// Destroy gui
	gui::destroyImGui();
	gui::destroyDevice();
	gui::destroyHWindow();

	return EXIT_SUCCESS;
}