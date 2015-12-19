
#include <iostream> 
#include <windows.h>
#include "const.h"



using namespace std;



void arrow_pos(RectangleShape &arrow, float height, float depth) {
	arrow.setRotation(-90);
	arrow.setSize(sf::Vector2f(float(height), float(depth)));
	arrow.setPosition(START_X, START_Y);
	arrow.setOrigin(0, float(depth / 2));
}

void shapes_position(init_shapes &sh) {
	arrow_pos(sh.hourArrow, 150, 10);
	sh.hourArrow.setFillColor(sf::Color::Green);
	arrow_pos(sh.minuteArrow, 200, 6);
	sh.minuteArrow.setFillColor(sf::Color::Yellow);
	arrow_pos(sh.secondArrow, 250, 3);
	sh.secondArrow.setFillColor(sf::Color::Magenta);
	sh.dot.setSize(sf::Vector2f(2, 2));
	sh.dot.setFillColor(sf::Color::White);
	sh.clockWall.setRadius(RADIUS);
	sh.clockWall.setPosition(140, 100);
	sh.clockWall.setOutlineThickness(50);

}

void coord_dots(sf::Vector2f(&coordinatesDots)[AMOUNT_OF_POINT]) {
	sf::Vector2f coorDot;
	for (int i = 0; i < AMOUNT_OF_POINT; i++) {
		coorDot.x = START_X + RAD_DIAL * cos(i * ONE_SECOND * float(ThisIsAPi) / 180);
		coorDot.y = START_Y + RAD_DIAL * sin(i * ONE_SECOND * float(ThisIsAPi) / 180);
		coordinatesDots[i] = { coorDot.x, coorDot.y };
	}
}

void draw_dots(RenderWindow &window, init_shapes &sh, sf::Vector2f(&coorDots)[AMOUNT_OF_POINT]) {
	for (int i = 0; i < AMOUNT_OF_POINT; i++) {
		if (i % 5 == 0) {
			sh.dot.setSize(sf::Vector2f(8, 8));
			sh.dot.setOrigin(4 / 2, 4 / 2);
		}
		else {
			sh.dot.setSize(sf::Vector2f(4, 4));
			sh.dot.setOrigin(2 / 2, 2 / 2);
		}
		sh.dot.setPosition(coorDots[i].x, coorDots[i].y);
		window.draw(sh.dot);
	}
}


void start_move_time(RenderWindow &window, init_shapes &sh) {
	sf::Vector2f coordDots[AMOUNT_OF_POINT];
	SYSTEMTIME sysTime;
	coord_dots(coordDots);
	while (window.isOpen()) {
		GetSystemTime(&sysTime);
		sh.secondArrow.setRotation(float(sysTime.wSecond * 6 - 90));
		sh.minuteArrow.setRotation(float(sysTime.wMinute * 6 - 90));
		sh.hourArrow.setRotation(float((sysTime.wHour + 3) * 30 - 90));
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		window.draw(sh.clockWall);
		draw_dots(window, sh, coordDots);
		window.draw(sh.hourArrow);
		window.draw(sh.minuteArrow);
		window.draw(sh.secondArrow);
		window.display();
	}
}

int main() {
	shapes_position(sh);

	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	Texture background_texture;//создаем объект Texture (текстура)
	background_texture.loadFromFile("background.jpeg");//передаем в него объект Image (изображения)
	sh.clockWall.setOutlineColor(sf::Color(250, 150, 100));
	sh.clockWall.setTexture(&background_texture);

	sf::RenderWindow window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "Analog clock", sf::Style::Default, settings);

	start_move_time(window, sh);
	return 0;
}
