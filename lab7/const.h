#pragma once
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace sf;

const int RADIUS = 400;
const double WINDOW_WIDTH = 2.5 * RADIUS;
const float START_X = RADIUS + 140, START_Y = RADIUS + 100;
const double WINDOW_HEIGHT = 2.7 * RADIUS;
const int ONE_SECOND = 6;
const int AMOUNT_OF_POINT = 60;
const int RAD_DIAL = RADIUS +20; // маштаб штрихов
float ThisIsAPi = float(M_PI);

struct init_shapes {
	CircleShape clockWall;
	RectangleShape hourArrow;
	RectangleShape minuteArrow;
	RectangleShape secondArrow;
	RectangleShape dot;
	
};

init_shapes sh;
