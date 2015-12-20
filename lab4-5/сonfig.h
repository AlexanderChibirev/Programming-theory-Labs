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
	float left;
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

}typedef Picture;


void picture_mid(Vector2u window_size, Picture & picture);
