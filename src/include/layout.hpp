#ifndef INCLUDE_SRC_LAYOUT_CPP_
#define INCLUDE_SRC_LAYOUT_CPP_

#include "clayman.hpp"
#include "config.hpp"
#include "raylib/raylib.h"
#include <algorithm>
#include <print>
#include <string>
#include <string_view>

// function pointer type for callback parameter is elementID
using func_t = void(*)(const std::string_view);

struct ReusableElements {
	static ReusableElements& getInstance() {
		static ReusableElements instance;
		return instance;
	}

	// param elementID, text, callback function
	void button(const std::string elementID, const std::string text = "", func_t callback = nullptr) {;
		bool overElement{clayMan.pointerOver(elementID)};
		bool mousePressed{clayMan.mousePressed()};

		if (overElement && mousePressed) {
			// check that the callback is valid
			if (callback != nullptr) {
				callback(elementID);
			}
		}

		clayMan.element({
			.id = clayMan.hashID(elementID),
			.layout = {
				.sizing = clayMan.expandYfixedX(80),
				.padding = CLAY_PADDING_ALL(8),
				.childAlignment = {CLAY_ALIGN_X_CENTER},
			},
			.backgroundColor = appData.isSelected(elementID) ? SELECTED_COLOR : overElement ? HOVER_COLOR : BUTTON_COLOR,
			.cornerRadius = Clay_CornerRadius(4),
		},[&](){
			clayMan.textElement((text == "") ? elementID : text, {
				.textColor = WHITE_COLOR,
				.fontId = 0,
				.fontSize = fontSize,
			});
		});
	}

	private:
		ReusableElements() = default;
};

struct ModularElements {
	static ModularElements& getInstance() {
		static ModularElements instance;
		return instance;
	}

	void todoElement() {

	}

	private:
		ModularElements() = default;
};


void mainLayout() {
		ReusableElements reusableElements = ReusableElements::getInstance();
		// ModularElements modularElements = ModularElements::getInstance();

		// body element
		clayMan.element({
			.id = clayMan.hashID("body"),
			.layout = {
				.sizing = clayMan.expandXY(),
				.padding = CLAY_PADDING_ALL(8),
				.childGap = 8,
				.layoutDirection = CLAY_TOP_TO_BOTTOM,
			},
			.backgroundColor = BACKGROUND_COLOR,
		},[&](){
			// navigation bar element
			clayMan.element( {
					.id = clayMan.hashID("navBar"),
					.layout = {
						.sizing = clayMan.expandXfixedY(50),
						.padding = CLAY_PADDING_ALL(8),
						.childGap = 8,
						.layoutDirection = CLAY_LEFT_TO_RIGHT,
					},
					.backgroundColor = FOREGROUND_COLOR,
					.cornerRadius = Clay_CornerRadius{4,4,4,4},
			},[&](){
				// callback for switching the selected element
				func_t callback = [](const std::string_view elementID){
					appData.selectedElement = elementID;
					appData.animationPercentage = 0;
				};

				reusableElements.button("File", "" ,callback);
				reusableElements.button("Edit", "" ,callback);
				reusableElements.button("Todo", "" ,callback);
				reusableElements.button("View", "" ,callback);
				reusableElements.button("About", "" ,callback);
			});

			// container for all kinds of content

			clayMan.element( {
					.id = clayMan.hashID("container"),
					.layout = {
						.sizing = {
							.width = (Clay_SizingAxis { .size = { .percent = appData.animationPercentage }, .type = CLAY__SIZING_TYPE_PERCENT}),
							.height = (Clay_SizingAxis { .size = { .minMax = {}}, .type = CLAY__SIZING_TYPE_GROW})},
						.padding = CLAY_PADDING_ALL(8),
						.childGap = 8,
					},
					.backgroundColor = FOREGROUND_COLOR,
					.cornerRadius = Clay_CornerRadius{4,4,4,4},
			},[&](){
				// draw child elements if animation is finished
				if (appData.isAnimationFinished()) {
					std::println("ANIMATION FINISHED");
				}
			});
		});
}

#endif  // INCLUDE_SRC_LAYOUT_CPP_
