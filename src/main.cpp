#include "../../ClayMan/clayman.hpp"
#include "../../ClayMan/include/raylib/clay_renderer_raylib.c"

#include "database.hpp"

#include <array>
#include <cstdint>
#include <functional>
#include <string>
#include <print>


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

	Database database{};

	// set monitor to 0 to keep the window on middle one
	SetWindowMonitor(0);

	//Load fonts after initializing raylib
	fonts[0] = LoadFontEx("../../ClayMan/examples/quick-example/resources/Roboto-Regular.ttf", 48, nullptr, 400);
	
	//Set fonts texture filters
	SetTextureFilter(fonts[0].texture, TEXTURE_FILTER_BILINEAR);

	// ELEMENTS
	enum navElementTypes : uint8_t{
		HOME,
		TODOS,
		BROWSE,
		CONTACT,
		ABOUT,
		MISC,
	};

	std::array<std::string, 6> navElements {"HOME", "TODOS", "BROWSE","CONTACT", "ABOUT", "MISC"};
	std::string selectedElement {};
	std::string hoveredElement {};


	// COLORS
	constexpr Clay_Color backgroundColor{230,230,255,30};
	constexpr Clay_Color selectedColor{255,0,0,127};
	constexpr Clay_Color highlightColor{230,230,255,40};

	std::function<void (std::string, std::string, int)> buttonElement = [&](const std::string elementID, const std::string buttonText, const int xSize){
		bool overElement = clayMan.pointerOver(elementID);
		bool mousePressed = clayMan.mousePressed();

		if (overElement && mousePressed) {
			selectedElement = elementID;
		} else if (overElement) {
			hoveredElement = elementID;
		}

		Clay_Color hoverColor = (overElement ?  highlightColor: (Clay_Color){230,230,255,0});

		clayMan.element({
				.id = clayMan.hashID(elementID),
				.layout = {
					.sizing = clayMan.expandY(),
					.padding = Clay_Padding(8,8,8,8),
					.childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER},
					.layoutDirection = CLAY_LEFT_TO_RIGHT,
				},
				// .hoverColor = {230,230,255,0},
				.backgroundColor = (selectedElement == elementID) ? selectedColor : hoverColor , 
				.cornerRadius = Clay_CornerRadius{8,8,8,8},
				// .border = {Clay_Color{255,255,255,255}, {2,2,2,2}},
		}, [&]{
			clayMan.textElement(
				buttonText, {
				.textColor = {255,255,255,255},
				.fontSize = static_cast<uint16_t>(24),
				.textAlignment = CLAY_TEXT_ALIGN_CENTER,
			});
		}
		);
	};

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

		clayMan.element(
			{
			.id = clayMan.hashID("Body"),
			.layout = {
				.sizing = clayMan.expandXY(),
				.padding = Clay_Padding(8,8,8,8),
				.childGap = 8,
				.layoutDirection = CLAY_TOP_TO_BOTTOM,
			},
			.backgroundColor = {230,230,255,20},
			},[&] {
				clayMan.element({
					.id = clayMan.hashID("NavBar"),
					.layout = {
						.sizing = clayMan.expandXfixedY(50),
						.layoutDirection = CLAY_LEFT_TO_RIGHT,
					},
					.backgroundColor = backgroundColor,
					.cornerRadius = Clay_CornerRadius(8),
				},[&]
				{
					for (const auto &element : navElements) {
						buttonElement(element, element, 100);
					}
				});
				clayMan.element({
						.id = clayMan.hashID("Content"),
						.layout = {
							.sizing = clayMan.expandXY(),
							.padding = Clay_Padding(8,8,8,8),
							.layoutDirection = CLAY_LEFT_TO_RIGHT,
						},
						.backgroundColor = backgroundColor,
						.cornerRadius = Clay_CornerRadius(8),
				},[&]{
					if (selectedElement == navElements[navElementTypes::HOME]) {
						clayMan.textElement("HOME",
								{
								.textColor = {255,255,255,255},
								.fontSize = 20,
						});
					}
					if (selectedElement == navElements[navElementTypes::ABOUT]) {
						clayMan.textElement("ABOUT",
								{
								.textColor = {255,255,255,255},
								.fontSize = 20,
						});
					}
					if (selectedElement == navElements[navElementTypes::BROWSE]) {
						clayMan.textElement("BROWSE",
								{
								.textColor = {255,255,255,255},
								.fontSize = 20,
						});
					}
					if (selectedElement == navElements[navElementTypes::CONTACT]) {
						clayMan.textElement("CONTACT",
								{
								.textColor = {255,255,255,255},
								.fontSize = 20,
						});
					}
					if (selectedElement == navElements[navElementTypes::MISC]) {
						clayMan.textElement("MISC",
								{
								.textColor = {255,255,255,255},
								.fontSize = 20,
						});
					}
					if (selectedElement == navElements[navElementTypes::TODOS]) {
						clayMan.textElement("TODOS",
								{
								.textColor = {255,255,255,255},
								.fontSize = 20,
						});
					}
				});
			}
		);

		// std::println("SELECTED: {} HOVERED: {}", selectedElement, hoveredElement);

		//Pass your layout to the manager to get the render commands
		Clay_RenderCommandArray renderCommands = clayMan.endLayout(); 

		BeginDrawing(); //Start Raylib's draw block
		ClearBackground(BLACK); //Raylib's clear function
		Clay_Raylib_Render(renderCommands, fonts); //Render Clay Layout
		EndDrawing(); //End Raylib's draw block
	}
	return 0;
}
