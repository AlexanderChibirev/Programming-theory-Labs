#include <SFML/Graphics.hpp>
#include <iostream> 
#include <windows.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;
using namespace std;

static const int WINDOW_WIDTH = 460;
static const int WINDOW_HEIGHT = 1500;
static const int LEFT_WHEEL_X = 115;
static const int LEFT_WHEEL_Y = 230;
static const int RIGHT_WHEEL_X = 380;
static const int RIGHT_WHEEL_Y = 230;
static const float RADIUS_WHEEL = 30;
static const float DIV_TIME = RADIUS_WHEEL*10;
static const float CIRCUMFERENCE = RADIUS_WHEEL * 2 * M_PI;

struct CarSprites {
	Sprite bodyCar;
	Sprite wheel;
	Texture bodyCarImage, wheelImage;
	float rotation = 0;
	float speed = 0;
	float acceleration = 0.00005;
	sf::Vector2f coordCar = {0, 200 };

	void is_initializing(CarSprites &homemade) {

		homemade.bodyCarImage.loadFromFile("car.png");
		homemade.wheelImage.loadFromFile("wheel.png");

		homemade.bodyCar.setTexture(homemade.bodyCarImage);
		homemade.wheel.setTexture(homemade.wheelImage);
	}

	void set_position_car() {
		wheel.setRotation(rotation);
		bodyCar.setPosition(coordCar);
		wheel.setPosition(coordCar.x + LEFT_WHEEL_X, coordCar.y + LEFT_WHEEL_Y);
	}

	void calculation_speed_and_rotation(Event event, float time) {
		if (event.type == Event::KeyPressed && event.key.code == Keyboard::Right) {
			speed += acceleration * time;
		}
		else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Left) {
			speed -= acceleration * time;
		}
		else {
			((speed < 0) ? speed += (acceleration * time) / 2 : speed -= (acceleration * time) / 2);
		}
		coordCar.x += speed;
		rotation = coordCar.x * 360 / CIRCUMFERENCE; //350 for reality
		set_position_car();
	}
	
	void is_drawing_car(RenderWindow &window, CarSprites &homemade) {
		window.draw(homemade.bodyCar);
	}
};
	

void start_initializing_and_move_time(RenderWindow &window, CarSprites &homemade) {
	homemade.is_initializing(homemade);
	Clock clock;
	while (window.isOpen()) {
		sf::Event event;
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / DIV_TIME;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}
		homemade.calculation_speed_and_rotation(event, time);

		window.clear(Color::White);
		homemade.is_drawing_car(window, homemade);
		homemade.wheel.setOrigin(RADIUS_WHEEL, RADIUS_WHEEL);
		homemade.wheel.setPosition(homemade.coordCar.x + LEFT_WHEEL_X, homemade.coordCar.y + LEFT_WHEEL_Y);
		window.draw(homemade.wheel);
		homemade.wheel.setPosition(homemade.coordCar.x + RIGHT_WHEEL_X, homemade.coordCar.y + RIGHT_WHEEL_Y);
		window.draw(homemade.wheel);
		window.display();
	}
}


int main() {
	CarSprites homemade;
	sf::RenderWindow window(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "CAR MADE IN HOME");
	start_initializing_and_move_time(window, homemade);
	return 0;
}
