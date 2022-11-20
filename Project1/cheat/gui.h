#pragma once

#include <d3d9.h>

namespace gui
{
	// Constants for window size
	constexpr int WIDTH = 800; 
	constexpr int HEIGHT = 600;

	inline bool exit = true;

	// winapi window vars
	inline HWND window = nullptr;
	inline WNDCLASSEXA windowClass = {};

	// points for window movement
	inline POINTS position = {};

	// direct x state vars
	inline PDIRECT3D9 d3d = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline D3DPRESENT_PARAMETERS presentParameters = {};

	// handling the creation and destruction of the window
	void createHWindow(const char* windowName, const char* className) noexcept;
	void destroyHWindow() noexcept;

	// handling device creation and destruction
	bool createDevice() noexcept;
	void resetDevice() noexcept;
	void destroyDevice() noexcept;

	// handling the ImGui creation and destruction
	void createImGui() noexcept;
	void destroyImGui() noexcept;

	void beginRender() noexcept;
	void endRender() noexcept;
	void render() noexcept;
}
