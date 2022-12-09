#pragma once
#include <iostream>
#include <vector>
#include <string>

namespace Settings
{
	static int Tab = 1;
}

namespace Visuals
{
	static bool masterSwitch = false;
	static bool boxESP = false;
	static bool lineESP = false;
	static bool skeletonESP = false;
	static bool healthBar = false;
	static bool glowESP = false;
	static bool onlyVisible = false;
	static bool onVisible = false;
	static bool showHeadPos = false;

	static float boxColor[] = {255.0f, 255.0f, 255.0f, 1.0f};
	static float lineColor[] = {255.0f, 255.0f, 255.0f, 1.0f};
	static float skeletonColor[] = { 255.0f, 255.0f, 255.0f, 1.0f };
	static float healthColor[] = {0.0f, 255.0f, 0.0f, 1.0f};
	static float glowColor[] = {255.0f, 0.0f, 0.0f, 1.0f};
	static float visibleColor[] = {0.0f, 0.0f, 255.0f, 1.0f};
	static float hiddenColor[] = {255.0f, 0.0f, 255.0f, 1.0f};
	static float headDotColor[] = {255.0f, 255.0f, 255.0f, 1.0f};

	// Health bar options
	static const char* healthBarOptions[]
	{
		"Top",
		"Left",
		"Right",
		"Bottom"
	};
	static int healthBarPos = 1;

	// Glow options
	static const char* glowOptions[]
	{
		"Static",
		"Pulse"
	};
	static int glowType = 0;

	// Chams options
	static const char* chamsOptions[]
	{
		"Flat",
		"Textured",
		"Glow"
	};
	static int chamsType = 1;

}

namespace World
{
	static bool radarReveal = false;
	static bool removeSmoke = false;
	static bool removeFlash = false;
	static bool worldColor = false;
	static bool bombTimer = false;
	static bool hitMarker = false;

	static float worldColorOption[] = { 0.0f, 0.0f, 0.0f, 1.0f };
}

namespace View
{
	static bool oofArrows = false;
	static bool playerFOV = false;
	static bool flashBangs = false;
	static bool thirdPerson = false;
	static bool aspectRatio = false;

	static float oofArrowSize = 1.0f;
	static float oofArrowDistance = 1.0f;
	static float thirdPersonDistance = 1.0f;
	static float aspectRatioSize = 1.0f;
}

namespace Aimbot
{
	static bool masterSwitch = false;
	static bool aimbotKeybind = false;
	static float aimbotFOV = 0.0f;
	static float aimbotSmoothing = 0.0f;
	static bool backtrackEnabled = false;
	static int backtrackingAmount = 0;
	static bool aimAtBacktrack = false;
	static bool ignoreSmoke = false;
	static bool ignoreFlash = false;
	static bool scopeOnly = false;
	
	// Hit Boxes options
	static const char* hitBoxes[]
	{
		"Head",
		"Neck",
		"Chest",
		"Lower Chest",
		"Legs"
	};
	static int hitBoxesSelected = 1;
}

namespace Triggerbot
{
	static bool masterSwitch = false;
	static bool keybindKeybind = false;
	static float fireDelay = 0.0f;
	static float hitchance = 0.0f;
}

namespace AntiAim
{
	static bool masterSwitch = false;
	static bool inverterKeybind = false;
	static float rotationAmount = 0.0f;
	static float fakelagAmount = 0.0f;
}

namespace Grenade
{
	static bool masterSwitch = false;
}

namespace Misc
{
	static bool revealRanks = false;
	static bool clanTagChanger = false;
	static string clanTag = "CHEAT";
	static bool spectatorList = false;
	static bool forceSniperCrosshair = false;
	static bool bunnyHop = false;
}

namespace Main
{
	static float menuBackgroundColor[] = { 28.0f, 28.0f, 28.0f, 1.0f };
	static float menuLineColor[] = { 255.0f, 255.0f, 255.0f, 1.0f };
	static float menuChildColor[] = { 255.0f, 255.0f, 255.0f, 1.0f };
}

namespace Scripts
{
	static bool masterSwitch = false;
}

namespace Config
{
	static bool saveConfig = false;
	static bool loadConfig = false;
}