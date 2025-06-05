#include "../../ClayMan/clayman.hpp"
#include "../../ClayMan/include/raylib/clay_renderer_raylib.c"
#include <array>
#include <string>
#include <vector>

//Your project's main entry
int main() {
	
	//Reserve memory for raylib fonts
	Font fonts[1];

	//Initialize ClayMan object using the constructor
	ClayMan clayMan(1024, 786, Raylib_MeasureText, fonts);

	//Initialize Raylib
	Clay_Raylib_Initialize(
		clayMan.getWindowWidth(), 
		clayMan.getWindowHeight(), 
		"My Mother Fucking Project", 
		FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT
	);

	// set monitor to 0 to keep the window on middle one
	SetWindowMonitor(0);

	//Load fonts after initializing raylib
	fonts[0] = LoadFontEx("../../ClayMan/examples/quick-example/resources/Roboto-Regular.ttf", 48, nullptr, 400);
	
	//Set fonts texture filters
	SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);

	//Raylib render loop
	while (!WindowShouldClose()) { 
		 
		//Raylib mouse position and scroll vectors
		Vector2 mousePosition = GetMousePosition(); 
		Vector2 scrollDelta = GetMouseWheelMoveV(); 
		
		//Update clay state (window size, mouse position/scroll, time delta, left button state)
		clayMan.updateClayState(
			GetScreenWidth(), //Raylib window width
			GetScreenHeight(), //Raylib window height
			mousePosition.x, 
			mousePosition.y, 
			scrollDelta.x, 
			scrollDelta.y, 
			GetFrameTime(), //Raylib frame delta
			IsMouseButtonDown(0) //Raylib left button down
		);

		//Prep for layout
		clayMan.beginLayout();

		std::vector<std::string> stringList = {};
		stringList.emplace_back("First");
		stringList.emplace_back("Second");
		stringList.emplace_back("Third");
		stringList.emplace_back("Fourth");

		clayMan.element(
			{
				.id = clayMan.hashID("MyElementID"),
				.layout = {
					.sizing = clayMan.expandXY(),
					.padding = CLAY_PADDING_ALL(8),
					.childGap = 8,
					.layoutDirection = CLAY_TOP_TO_BOTTOM
				},
				.backgroundColor = {255,0,0,255}
			},
			[&]{
				clayMan.element({
					.id = clayMan.hashID("NavElement"),
					.layout = {
						.sizing = clayMan.expandXfixedY(100),
						.childGap = 16,
						.layoutDirection = CLAY_TOP_TO_BOTTOM,
					},
					.backgroundColor = {127,0,0,255},
					.cornerRadius = CLAY_CORNER_RADIUS(10),
					},[&]{
					 //Empty
				});
				clayMan.element({
					.id = clayMan.hashID("SubElement"),
					.layout = {
						.sizing = clayMan.expandXY(),
						.childGap = 8,
						.layoutDirection = CLAY_LEFT_TO_RIGHT,
					},
					.backgroundColor = {127,0,0,0},
					.cornerRadius = CLAY_CORNER_RADIUS(10),
					},[&]{
					clayMan.element({
						.id = clayMan.hashID("SideBarElement"),
						.layout = {
							.sizing = clayMan.expandYfixedX(300),
							.childGap = 16,
							.layoutDirection = CLAY_TOP_TO_BOTTOM,
						},
						.backgroundColor = {127,0,0,255},
						.cornerRadius = CLAY_CORNER_RADIUS(10),
						},[&]{
							for (const auto &string : stringList) {
								clayMan.textElement(string, 
								{
								.textColor = {255,255,255,255},
								.fontId = 0,
								.fontSize = 30
								});
							}
					});
					clayMan.element({
						.id = clayMan.hashID("ContentElement"),
						.layout = {
							.sizing = clayMan.expandXY(),
							.childGap = 16,
							.layoutDirection = CLAY_TOP_TO_BOTTOM,
						},
						.backgroundColor = {127,0,0,255},
						.cornerRadius = CLAY_CORNER_RADIUS(10),
						},[&]{
					});
				});
				}
		);
		

		//Pass your layout to the manager to get the render commands
		Clay_RenderCommandArray renderCommands = clayMan.endLayout(); 

		BeginDrawing(); //Start Raylib's draw block
		ClearBackground(BLACK); //Raylib's clear function
		Clay_Raylib_Render(renderCommands, fonts); //Render Clay Layout
		EndDrawing(); //End Raylib's draw block
	}
	return 0;
}
