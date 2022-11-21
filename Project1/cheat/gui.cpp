#include "gui.h"

#include <string>
#include <vector>
using namespace std;

#include "settings.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter);

long __stdcall WindowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
	{
		return true;
	}

	switch (message)
	{

		case WM_SIZE: 
		{
			if (gui::device && wideParameter != SIZE_MINIMIZED)
			{
				gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
				gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
				gui::resetDevice();
			}
		}return 0;

		case WM_SYSCOMMAND:
		{
			if ((wideParameter & 0xfff0) == SC_KEYMENU)	// Disable ALT application menu
			{
				return 0;
			}
		}break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}return 0;

		// Left button down
		case WM_LBUTTONDOWN:
		{
			gui::position = MAKEPOINTS(longParameter); // Set click points
		}return 0;

		// Mouse moving window
		case WM_MOUSEMOVE:
		{
			if (wideParameter == MK_LBUTTON)
			{
				const auto points = MAKEPOINTS(longParameter);
				auto rect = ::RECT{};
				GetWindowRect(gui::window, &rect);

				rect.left += points.x - gui::position.x;
				rect.top += points.y - gui::position.y;

				if (gui::position.x >= 0 && gui::position.x <= gui::WIDTH && gui::position.y >= 0 && gui::position.y <= 19)
				{
					SetWindowPos(gui::window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
				}
			}
		}return 0;

	}

	return DefWindowProcW(window, message, wideParameter, longParameter);
}

// handling the creation and destruction of the window
// In here is the window spawn location
void gui::createHWindow(const char* windowName, const char* className) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEXA);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = 0;

	RegisterClassExA(&windowClass);

	window = CreateWindowA(
		className,
		windowName,
		WS_POPUP,
		500,
		400,
		WIDTH,
		HEIGHT,
		0,
		0,
		windowClass.hInstance,
		0
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::destroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

// handling device creation and destruction
bool gui::createDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
	{
		return false;
	}

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	presentParameters.EnableAutoDepthStencil = TRUE;
	presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &device) < 0)
	{
		return false;
	}
	return true;
}

void gui::resetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
	{
		IM_ASSERT(0);
	}

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::destroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

// Colors
void Colors()
{
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowRounding = 11.0f;
	style.FrameRounding = 6.0f;
	style.ScrollbarRounding = 5;

	style.ChildRounding = 6.0f;

	style.Colors[ImGuiCol_WindowBg] = ImColor (28, 28, 28);
	//style.Colors[ImGuiCol_ChildBg] = ImColor (39, 39, 39);
	//style.Colors[ImGuiCol_Border] = ImColor(53,53,53);

	//style.Colors[ImGuiCol_WindowBg] = ImColor(Main::menuBackgroundColor[0], Main::menuBackgroundColor[1], Main::menuBackgroundColor[2], Main::menuBackgroundColor[3]);
	style.Colors[ImGuiCol_ChildBg] = ImColor(39, 39, 39);
	style.Colors[ImGuiCol_Border] = ImColor(53,53,53);

	style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);

	style.Colors[ImGuiCol_Header] = ImColor(24, 24, 24);
	style.Colors[ImGuiCol_HeaderActive] = ImColor(30,30,30);
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(30, 30, 30);

	style.Colors[ImGuiCol_Button] = ImColor(30, 30, 30);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(24,24,24);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(24, 24, 24);

	style.Colors[ImGuiCol_CheckMark] = ImColor(16, 16, 16);
	style.Colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30);
	style.Colors[ImGuiCol_FrameBgActive] = ImColor(16,16,16);
	style.Colors[ImGuiCol_FrameBgHovered] = ImColor(16, 16, 16);
}

// handling the ImGui creation and destruction
void gui::createImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;
	// Custom font
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma.ttf", 16.0f);

	// Color theme
	Colors();
	//ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);
}

void gui::destroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::beginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	// Start the Dear ImGui fram
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::endRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		resetDevice();
	}
}

// The Window is designed here
void gui::render() noexcept
{
	ImGui::SetNextWindowPos({ 0,0 });	// ImGui window will take up the full space of the DirectX window
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });	// Sets the background DirectX window to the width and height to 'hide' it
	{
		struct aimbot_data
		{
			const char* name;
		};
		struct visuals_data
		{
			const char* name;
		};
		struct misc_data
		{
			const char* name;
		};

		static vector<aimbot_data> aimbot = {
			{"Ragebot"},
			{"Anti Aim"},
			{"Legitbot"}
		};
		static vector<visuals_data> visuals = {
			{"Players"},
			{"Weapon"},
			{"Grenades"},
			{"World"},
			{"View"},
			{"Skins"}
		};
		static vector<misc_data> misc = {
			{"Main"},
			{"Scripts"},
			{"Config"}
		};

		static int focusAim = 0;
		static int focusVis = -1;
		static int focusMisc = -1;
		bool selected;
		int selection = 0;

		ImGui::Begin(
			"ImGui Window",
			&exit,
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoSavedSettings |
			ImGuiWindowFlags_NoCollapse |
			//ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoTitleBar
		);

		// Left side
		ImGui::BeginChild("##LeftSide", ImVec2(125, HEIGHT - 16), true);
		{
			// Aimbot
			{
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
				ImGui::Text("Aimbot");
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

				for (unsigned int i = 0; i < aimbot.size(); i++)
				{
					selected = (focusAim == i);
					ImGui::Dummy(ImVec2(1, 1));

					if (ImGui::Selectable(aimbot[i].name, &selected))
					{
						focusAim = i;
						focusVis = -1;
						focusMisc = -1;
						Settings::Tab = i + 1;
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::Dummy(ImVec2(1, 20));
			// Visuals
			{
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
				ImGui::Text("Visuals");
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

				for (unsigned int i = 0; i < visuals.size(); i++)
				{
					selected = (focusVis == i);
					ImGui::Dummy(ImVec2(1, 1));

					if (ImGui::Selectable(visuals[i].name, &selected))
					{
						focusVis = i;
						focusAim = -1;
						focusMisc = -1;
						Settings::Tab = i + aimbot.size() + 1;
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}
			ImGui::Dummy(ImVec2(1, 20));
			// Misc
			{
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
				ImGui::Text("Misc");
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
				ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

				for (unsigned int i = 0; i < misc.size(); i++)
				{
					selected = (focusMisc == i);
					ImGui::Dummy(ImVec2(1, 1));

					if (ImGui::Selectable(misc[i].name, &selected))
					{
						focusMisc = i;
						focusAim = -1;
						focusVis = -1;
						Settings::Tab = i + aimbot.size() + visuals.size() + 1;
					}
					if (selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::SetItemDefaultFocus();

			}
			
		}
		ImGui::EndChild();

		{
			//ImGui::SameLine(0);
			//ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
			//ImGui::Dummy(ImVec2(1, HEIGHT - 16));
			ImGui::SameLine();
		}

		// Right side		
		ImGui::BeginChild("##RightSide", ImVec2(651, (HEIGHT - 16)), true);
		{
			ImGui::BeginChild("##TopRightSide", ImVec2(635, 45), true);
			{
				ImGui::Dummy(ImVec2(0, 0.5f));
				ImGui::Dummy(ImVec2(362, 0.5f));
				ImGui::SameLine();
				// Search bar
				static ImGuiTextFilter filter;
				filter.Draw("Search...", 240);
			}
			ImGui::EndChild();
			
			ImGui::BeginChild("##BottomRightSide", ImVec2(635, 519), true);
			{
				// Tab 1	// Ragebot
				if (Settings::Tab == 1)
				{
					ImGui::Text("1");
				}
				// Tab 2	// Anti Aim
				if (Settings::Tab == 2)
				{					
					ImGui::BeginChild("##AALeft", ImVec2(310, 780), false);
					{
						ImGui::Text("Anti Aim");
						ImGui::Checkbox("Enable AA", &AntiAim::masterSwitch);
						// Empty until I fully understand how to impliment an AA
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##AARight", ImVec2(290, 780), false);
					{

					}
					ImGui::EndChild();
				}
				// Tab 3	// Legitbot
				if (Settings::Tab == 3)
				{
					ImGui::BeginChild("##LegitLeft", ImVec2(310, 780), false);
					{
						ImGui::Text("Aimbot");
						ImGui::Checkbox("Aimbot Master Switch", &Aimbot::masterSwitch);
						ImGui::Checkbox("Backtrack Enabled", &Aimbot::backtrackEnabled);

						if (Aimbot::backtrackEnabled)
						{
							ImGui::SliderInt("Backtrack Amount", &Aimbot::backtrackingAmount, 0, 12);
						}

						ImGui::Checkbox("Aim at backtrack", &Aimbot::aimAtBacktrack);

						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::Spacing();

						ImGui::Text("Triggerbot");
						ImGui::Checkbox("Enable triggerbot", &Triggerbot::masterSwitch);

						if (Triggerbot::masterSwitch)
						{
							ImGui::SliderFloat("Fire Delay", &Triggerbot::fireDelay, 0, 100);
							ImGui::SliderFloat("Hit Chance", &Triggerbot::hitchance, 0, 100);
						}

					}

					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##LegitRight", ImVec2(290, 780), false);
					{

					}
					ImGui::EndChild();
				}
				// Tab 4	// Players
				if (Settings::Tab == 4)
				{
					ImGui::BeginChild("##PlayersLeft", ImVec2(300, 780), false);
					{
						ImGui::Checkbox("Box ESP", &Visuals::boxESP);
						ImGui::Checkbox("Line ESP", &Visuals::lineESP);
						ImGui::Checkbox("Skeleton ESP", &Visuals::skeletonESP);
						ImGui::Checkbox("Health Bar", &Visuals::healthBar);
						ImGui::Checkbox("Glow ESP", &Visuals::glowESP);
						ImGui::Checkbox("Only Visible", &Visuals::onlyVisible);
						ImGui::Checkbox("On Visible", &Visuals::onVisible);
						ImGui::Checkbox("Head Position", &Visuals::showHeadPos);
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##PlayersRight", ImVec2(300, 780), false);
					{
						//ImGui::ColorEditOptionsPopup(Visuals::boxColor, );
					//ImGui::ColorPicker4("Box Color", Visuals::boxColor);
						ImGui::ColorEdit4("Box ESP", Visuals::boxColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::ColorEdit4("Line ESP", Visuals::lineColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::ColorEdit4("Skeleton ESP", Visuals::skeletonColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::ColorEdit4("Health Bar", Visuals::healthColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::ColorEdit4("Glow ESP", Visuals::glowColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::ColorEdit4("Visible Chams", Visuals::visibleColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::ColorEdit4("Hidden Chams", Visuals::hiddenColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::ColorEdit4("Head Dot", Visuals::headDotColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

						ImGui::Spacing();
						ImGui::Spacing();
						ImGui::Spacing();

						ImGui::Combo("Health Bar Pos", &Visuals::healthBarPos, Visuals::healthBarOptions, 4);
						ImGui::Combo("Glow Type", &Visuals::glowType, Visuals::glowOptions, 2);
						ImGui::Combo("Chams Type", &Visuals::chamsType, Visuals::chamsOptions, 3);
					}			
					ImGui::EndChild();
				}
				// Tab 5	//Weapon
				if (Settings::Tab == 5)
				{
					ImGui::BeginChild("##WeaponLeft", ImVec2(300, 780), false);
					{
						ImGui::Text("Weapon");
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##WeaponRight", ImVec2(300, 780), false);
					{

					}
					ImGui::EndChild();
				}
				// Tab 6	// Grenades
				if (Settings::Tab == 6)
				{
					ImGui::BeginChild("##GrenadeLeft", ImVec2(300, 780), false);
					{
						ImGui::Text("Grenades");
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##GrenadeRight", ImVec2(300, 780), false);
					{

					}
					ImGui::EndChild();
				}
				// Tab 7	// World
				if (Settings::Tab == 7)
				{
					ImGui::BeginChild("##WorldLeft", ImVec2(300, 780), false);
					{
						ImGui::Checkbox("Radar Reveal", &World::radarReveal);
						ImGui::Text("Removals");
						ImGui::Checkbox("Smoke", &World::removeSmoke);
						ImGui::Checkbox("Flash Bangs", &World::removeFlash);
						ImGui::Checkbox("World Color", &World::worldColor);
						ImGui::Checkbox("Bomb Timer", &World::bombTimer);
						ImGui::Checkbox("Hit Marker", &World::hitMarker);
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##WorldRight", ImVec2(300, 780), false);
					{
						ImGui::ColorEdit4("World", World::worldColorOption, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
					}
					ImGui::EndChild();
				}
				// Tab 8	// View
				if (Settings::Tab == 8)
				{
					ImGui::BeginChild("##ViewLeft", ImVec2(300, 780), false);
					{
						ImGui::Checkbox("Out of FOV arrows", &View::oofArrows);
						ImGui::Checkbox("Player FOV", &View::playerFOV);
						ImGui::Checkbox("Flash Bangs", &View::flashBangs);
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##ViewRight", ImVec2(300, 780), false);
					{
						ImGui::SliderFloat("Arrow Size", &View::oofArrowSize, 0, 10);
						ImGui::SliderFloat("Arrow Distance", &View::oofArrowDistance, 0, 10);
						ImGui::SliderFloat("Third Person Distance", &View::thirdPersonDistance, 0, 10);
						ImGui::SliderFloat("Aspect Ratio", &View::aspectRatioSize, 0, 10);
					}
					ImGui::EndChild();
				}
				// Tab 9	// Skins
				if (Settings::Tab == 9)
				{
					ImGui::BeginChild("##SkinsLeft", ImVec2(300, 780), false);
					{
						ImGui::Text("Skin Changer");
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##SkinsRight", ImVec2(300, 780), false);
					{
						
					}
					ImGui::EndChild();
				}
				// Tab 10	// Main
				if (Settings::Tab == 10)
				{
					ImGui::BeginChild("##MainLeft", ImVec2(300, 780), false);
					{
						ImGui::Text("Main");
						ImGui::Text("Menu Colors");
						ImGui::ColorEdit4("Background Color", Main::menuBackgroundColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::ColorEdit4("Outline Color", Main::menuLineColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
						ImGui::ColorEdit4("Child Color", Main::menuChildColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##MainRight", ImVec2(300, 780), false);
					{

					}
					ImGui::EndChild();
				}
				// Tab 11	// Scripts
				if (Settings::Tab == 11)
				{
					ImGui::BeginChild("##ScriptsLeft", ImVec2(300, 780), false);
					{
						ImGui::Text("Scripting");
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##ScriptsRight", ImVec2(300, 780), false);
					{

					}
					ImGui::EndChild();
				}
				// Tab 12	// Config
				if (Settings::Tab == 12)
				{
					ImGui::BeginChild("##ConfigLeft", ImVec2(300, 780), false);
					{
						ImGui::Text("Config");
					}
					ImGui::EndChild();

					ImGui::SameLine();

					ImGui::BeginChild("##ConfigRight", ImVec2(300, 780), false);
					{

					}
					ImGui::EndChild();
				}
				

			}
			ImGui::EndChild();

		}
		ImGui::EndChild();
		

		

		ImGui::End();
	}
}