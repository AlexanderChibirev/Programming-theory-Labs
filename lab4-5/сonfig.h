#pragma once
using namespace sf;
using namespace std;


struct Sys {
	Vector2u window_size = Vector2u(800, 600);
	string dir_path = "";
}typedef Sys;

struct Files {
	int name_size = 0;
	int arr_size = 0;

	string *files = nullptr;
	string path;

}typedef Files;

struct Picture {
	int num = 0;
	float zoom = 1.0;
	float left_bias;
	float top;
	float shift_x;
	float shift_y;
	float st_drag_x, st_drag_y;	
	float smoth_zoom = 0.25;
	bool drag_move = false;
	bool error = true;
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
	void is_initializing_toolbar(Picture &picture) {

		picture.left_image.loadFromFile("images/left.png");
		picture.right_image.loadFromFile("images/right.png");
		picture.plus_image.loadFromFile("images/plus.png");
		picture.minus_image.loadFromFile("images/minus.png");

		picture.left.setTexture(picture.left_image);
		picture.right.setTexture(picture.right_image);
		picture.plus.setTexture(picture.plus_image);
		picture.minus.setTexture(picture.minus_image);

		picture.left.setOrigin(Vector2f(0, left.getGlobalBounds().height));
		picture.right.setOrigin(Vector2f(right.getGlobalBounds().width, right.getGlobalBounds().height));
		picture.plus.setOrigin(Vector2f(0, plus.getGlobalBounds().height));
		picture.minus.setOrigin(Vector2f(0, plus.getGlobalBounds().height));

	}

}typedef Picture;


void picture_mid(Vector2u window_size, Picture & picture);