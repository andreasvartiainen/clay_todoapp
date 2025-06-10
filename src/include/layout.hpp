#ifndef INCLUDE_SRC_LAYOUT_CPP_
#define INCLUDE_SRC_LAYOUT_CPP_

#include "../../ClayMan/clayman.hpp"
#include "../../ClayMan/include/raylib/clay_renderer_raylib.c"

class Layout {
private:
	ClayMan clayMan{0,0,nullptr,nullptr};

public:
	Layout () {
		//Reserve memory for raylib fonts
		Font fonts[1];

		//Initialize ClayMan object using the constructor
		clayMan = {1024, 786, Raylib_MeasureText, fonts};

		//Initialize Raylib
		Clay_Raylib_Initialize(
			clayMan.getWindowWidth(), 
			clayMan.getWindowHeight(), 
			"My Mother Fucking Project", 
			FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT
		);
	}
};

#endif  // INCLUDE_SRC_LAYOUT_CPP_
