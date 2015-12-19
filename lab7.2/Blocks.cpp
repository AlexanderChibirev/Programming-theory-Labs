#include <SFML/Graphics.hpp>
#include "initialization.h"
#include "Config.h"
#include <random>
#include <iostream>

void update(RenderWindow & window, Colony & colony) {
	if (colony.situation == "generation_effect") {
		colony.direction = rand() % 2 + 1;;
		colony.situation = "effect2";
	}
	if (colony.situation == "effect2") {
		if (colony.direction == 1){
			colony.up(); 
			colony.effect_rotate();	}
		else
			colony.down();
	}
	if (colony.situation == "effect3") {

		if (colony.direction == 1)
		{
			colony.effect_rotate(); 
			colony.come_back_up();
		}
		else
			colony.come_back_down();
	}
}

int main() {
	Colony *colony = new Colony;
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(800, 800), "Blocks", sf::Style::Default, settings);
	initiation(*colony);

	while (window.isOpen()) {
		update(window, *colony);
		draw(*colony, window);
	}
	delete_blocks(*colony);
	delete colony;

}



