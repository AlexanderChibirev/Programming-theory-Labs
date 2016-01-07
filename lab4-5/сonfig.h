#pragma once
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;


struct Sys {
	Vector2u window_size = Vector2u(800, 600);
	string dir_path = "";
};

struct Picture {
	int num = 0;
	float zoom = 1.0;
	float left_bias;
	float top;
	float smoth_zoom = 0.25;
	bool drag_move = false;
	bool error = true;
	Vector2f st_drag_xy;
	Vector2f shift_xy;
	string title = "Error";
	Texture *texture = nullptr;
	Sprite *sprite = nullptr;
	Sprite left;
	Sprite right;
	Sprite plus;
	Sprite minus;
	Texture left_image;
	Texture right_image;
	Texture plus_image;
	Texture minus_image;
	void is_initializing_toolbar(Picture &picture);
	
};


void picture_mid(Vector2u window_size, Picture & picture);
