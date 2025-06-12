#ifndef INCLUDE_INCLUDE_CONFIG_HPP_
#define INCLUDE_INCLUDE_CONFIG_HPP_

#include "../../ClayMan/clayman.hpp"
#include "../../ClayMan/include/raylib/clay_renderer_raylib.c"

#include "database.hpp"
#include "raylib/raylib.h"
#include <cstdint>

constexpr uint8_t fontSize{20};

constexpr Clay_Color BACKGROUND_COLOR{23,23,26,255};
constexpr Clay_Color FOREGROUND_COLOR{33,33,36,255};
constexpr Clay_Color BUTTON_COLOR{106,41,41,255};
constexpr Clay_Color HOVER_COLOR{134,51,51,255};
constexpr Clay_Color SELECTED_COLOR{196,49,49,255};
constexpr Clay_Color WHITE_COLOR{255,255,255,255};

struct AppData {
	std::string selectedElement;
	std::string todoText;
	float animationPercentage;
	float animationSpeed{2};

	// update animation values
	void updateAnimation(){
		if (animationPercentage < 1.0F) {
			animationPercentage += animationSpeed*GetFrameTime();
		}
		// use std::min for setting the boundary for the animation percentage
		animationPercentage = std::min(animationPercentage, 1.0F);
	}

	bool isSelected(const std::string &elementID) {
		return selectedElement == elementID;
	}
	
	bool isAnimationFinished() {
		return animationPercentage == 1.0F;
	}
};

//Reserve memory for raylib fonts
Font fonts[1];

//Initialize ClayMan object using the constructor
ClayMan clayMan(1024, 786, Raylib_MeasureText, fonts);

AppData appData;

Database database{};

//Initialize Raylib
void initRaylib() {
	Clay_Raylib_Initialize(
		clayMan.getWindowWidth(), 
		clayMan.getWindowHeight(), 
		"My Mother Fucking Project", 
		FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT
	);

	SetTargetFPS(80);

	// set monitor to 0 to keep the window on middle one
	SetWindowMonitor(0);

	//Load fonts after initializing raylib
	fonts[0] = LoadFontEx("../../ClayMan/examples/quick-example/resources/Roboto-Regular.ttf", 48, nullptr, 400);

	//Set fonts texture filters
	SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);
}

#endif  // INCLUDE_INCLUDE_CONFIG_HPP_
