#include "config.hpp"
#include "layout.hpp"

#include <print>

//Your project's main entry
int main() {
	initRaylib();

	// database.insertUser();
	// database.insertTodo("shit todo message");
	// database.fetchTodoTable();

	for (const auto &todo :todoList) {
		std::println("{} {} {}", todo.ID, todo.content, todo.completed);
	}
	
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

		// update animation state
		appData.updateAnimation();
		mainLayout();

		//Pass your layout to the manager to get the render commands
		Clay_RenderCommandArray renderCommands = clayMan.endLayout(); 

		BeginDrawing(); //Start Raylib's draw block
		ClearBackground(BLACK); //Raylib's clear function
		Clay_Raylib_Render(renderCommands, fonts); //Render Clay Layout
		EndDrawing(); //End Raylib's draw block
	}
	return 0;
}
