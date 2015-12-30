
#include <string>
#include <array>
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <clocale>
#include <locale>
#include <tchar.h>
#include <cwchar>
#include <vector>
#include <stdio.h>
#include "config.h"




bool is_image(string file_name, string path) { 
	
	if (!strrchr(file_name.c_str(), '.')) {
		return false;
	}
	std::array<string, 5> extArray = { "jpg","jpeg","png","gif","bmp" };
	if(std::any_of(extArray.begin(), extArray.end(), [&](const std::string &ext) {
		return ext == file_name.substr(file_name.find_last_of(".") + 1);
	})){
	
		Image image;
		if (!(image.loadFromFile(path + file_name))) {
			return false;
		}
		else {
			return true;
		}

	}
	return true;
}

bool directory_exists(string &fileName) { 
	int code = GetFileAttributes((LPCWSTR)fileName.c_str());
	return (code != -1) && (FILE_ATTRIBUTE_DIRECTORY & code);
	//return true;
}

string narrow(std::wstring const& text)
{
	locale const loc("");
	wchar_t const* from = text.c_str();
	size_t const len = text.size();
	vector<char> buffer(len + 1);
	use_facet<std::ctype<wchar_t> >(loc).narrow(from, from + len, '_', &buffer[0]);
	return string(&buffer[0], &buffer[len]);
}


vector<string> get_file_list(string const & oldPath) {
	string path = oldPath + string("*");
	vector<string> files;
	
	WIN32_FIND_DATA fileData;
	HANDLE firstFile = FindFirstFile((LPCWSTR)path.c_str(), &fileData);//поиск  файла
	if (firstFile != INVALID_HANDLE_VALUE) {
		do {
			if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				if (is_image(narrow(fileData.cFileName),oldPath)) {
					files.push_back(narrow(fileData.cFileName));
				}
			}

		} while (FindNextFile(firstFile, &fileData));
		
		FindClose(firstFile);
	}
	
	return files;
}


void sprite_for_window(Picture &pict, Sys &sys, vector<string> const &files) {
	pict.sprite = new Sprite;
	pict.sprite->setPosition(0, 0);
	pict.sprite->setTexture(*(pict.texture));
	if (pict.texture->getSize().x > 800 && pict.texture->getSize().y > 600 && pict.zoom == 1) {
		pict.zoom = float(sys.window_size.x) / pict.texture->getSize().x;
		pict.smoth_zoom = pict.zoom * 25 / 100;
	}
	pict.sprite->setScale(pict.zoom, pict.zoom);
	pict.sprite->setOrigin(float(pict.texture->getSize().x * 0.5), float(pict.texture->getSize().y / 2.0));
	pict.title = string(files[pict.num]);
}

string init_picture(RenderWindow & window, vector<string> const &files, string const &path, Picture &pic, Sys & sys) {
	if (!(pic.num < 0)) {
		if (!pic.error) {
			delete(pic.sprite);
		}
		Image *image = new Image;
		string path = path + files[pic.num];

		if (!(image->loadFromFile(path + files[pic.num]))) {
			std::cout << "error with: \n" << path + files[pic.num] << '\n';
			pic.error = true;
		}
		else {
			pic.error = false;
		}
		delete(pic.texture);
		pic.texture = new Texture;
		pic.texture->loadFromImage(*image);
		delete(image);
		sprite_for_window(pic, sys, files);
		picture_mid(window.getSize(), pic);
	}
	return pic.title;
}

void drag(Picture &picture, Vector2f & cursor, Sys sys) {
	Vector2f picture_pos = picture.sprite->getPosition();
	if (cursor.x < sys.window_size.x && cursor.x > 0 && cursor.y < sys.window_size.y && cursor.y > 0)
		picture.sprite->setPosition(cursor.x + picture.shift_xy.x, cursor.y + picture.shift_xy.y);
}

void picture_mid(Vector2u window_size, Picture &picture) {
	picture.left_bias = window_size.x *0.5f;
	picture.top = window_size.y *0.5f;
	picture.sprite->setPosition(Vector2f(picture.left_bias, picture.top));
}

void switch_prev(RenderWindow & window, Picture & picture, vector<string> & files, string const &path, Sys & sys) {
	picture.zoom = 1;
	picture.smoth_zoom = 0.25;
	if (picture.num == 0) {
		picture.num = files.size();
	}
	picture.num--;
	window.setTitle(init_picture(window, files, path, picture, sys));
}

void switch_next(RenderWindow & window, Picture &picture, vector<string> & files, string const &path, Sys & sys) {
	picture.zoom = 1;
	picture.smoth_zoom = 0.25;
	if (picture.num + 1 == files.size()) {
		picture.num = 0;
	}
	else {
		picture.num++;
	}
	window.setTitle(init_picture(window, files, path, picture, sys));
}


void draw_buttons(RenderWindow & window, Sys & sys, Picture & picture, char &activeZoom) {
	picture.left.setPosition(Vector2f(0, float(sys.window_size.y * 0.5)));
	picture.right.setPosition(Vector2f(float(sys.window_size.x), float(sys.window_size.y * 0.5)));
	picture.plus.setPosition(Vector2f(float(sys.window_size.x) / 2 + 25, float(sys.window_size.y)));
	picture.minus.setPosition(Vector2f(float(sys.window_size.x) / 2 - 25, float(sys.window_size.y)));
	window.draw(picture.left);
	window.draw(picture.right);
	if (activeZoom != 1)
		window.draw(picture.plus);
	if (activeZoom != 2)
		window.draw(picture.minus);
}


void draw_elements(RenderWindow & window, Sys & sys, Picture & picture) {
	picture.sprite->setTexture((*picture.texture));
	window.draw((*picture.sprite));

	char activeZoom = 0;
	if (picture.zoom > 5 - picture.smoth_zoom) {
		activeZoom = 1;
	}
	else if (picture.zoom <= picture.smoth_zoom) {
		activeZoom = 2;
	}
	draw_buttons(window, sys, picture, activeZoom);
}


void click_btn_mouse(RenderWindow &window, vector<string> files, string const &path,Picture &picture, Sys sys, Vector2f pos) {
	if (!picture.drag_move) {
		if (picture.left.getGlobalBounds().contains(pos.x, pos.y)) {
			switch_prev(window, picture, files, path, sys);
		}
		else if (picture.right.getGlobalBounds().contains(pos.x, pos.y)) {
			switch_next(window, picture, files, path, sys);
		}
		else if (picture.plus.getGlobalBounds().contains(pos.x, pos.y) && picture.zoom < 5) {
			picture.zoom += picture.smoth_zoom;
			window.setTitle(init_picture(window, files, path, picture, sys));
		}
		else if (picture.minus.getGlobalBounds().contains(pos.x, pos.y) && picture.zoom > picture.smoth_zoom) {
			picture.zoom -= picture.smoth_zoom;
			window.setTitle(init_picture(window, files, path, picture, sys));
		}
	}
}

void click_btn(RenderWindow &window, vector<string> files, string const &path, Picture &picture, Sys sys, Vector2f pos) {
	if (!picture.drag_move) {
		if (Keyboard::isKeyPressed(Keyboard::Left)) {
			switch_prev(window, picture, files, path, sys);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right)) {
			switch_next(window, picture, files, path, sys);
		}
		if (Keyboard::isKeyPressed(Keyboard::Add) && Keyboard::isKeyPressed(Keyboard::LControl) && picture.zoom < 5) {
			picture.zoom += picture.smoth_zoom;
			window.setTitle(init_picture(window, files, path, picture, sys));
		}
		if (Keyboard::isKeyPressed(Keyboard::Subtract) && Keyboard::isKeyPressed(Keyboard::LControl) && picture.zoom > picture.smoth_zoom) {
			picture.zoom -= picture.smoth_zoom;
			window.setTitle(init_picture(window, files, path, picture, sys));
		}
	}
}

void picture_check_drag(Picture &picture, Vector2f pos, Sys sys) {
	Vector2f picture_scale = picture.sprite->getScale();
	FloatRect picture_size = picture.sprite->getGlobalBounds();
	Vector2f picture_pos = picture.sprite->getPosition();
	float pictHeight = picture_size.height * picture_scale.x;
	float pictWidth = picture_size.width * picture_scale.y;
	if (pos.y < picture_pos.y + pictHeight / 2 && pos.x < picture_pos.x + pictWidth / 2 &&
		pos.y > picture_pos.y - pictHeight / 2 && pos.x > picture_pos.x - pictWidth / 2) {
		picture.drag_move = true;
		picture.st_drag_xy.x = pos.x;
		picture.st_drag_xy.y = pos.y;
		picture.shift_xy.x = picture_pos.x - picture.st_drag_xy.x;
		picture.shift_xy.y = picture_pos.y - picture.st_drag_xy.y;
	}
}
void start_program(RenderWindow &window, vector<string> files, string const &path, Picture &picture, Sys sys, View view) {
	while (window.isOpen()) {
		Event event;
		Vector2i pixelPos = Mouse::getPosition(window);
		Vector2f pos = window.mapPixelToCoords(pixelPos);
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				files.clear(); // очистка памяти
				window.close();
			}
			if (picture.drag_move) {
				drag(picture, pos, sys);
			}
			if (event.type == Event::MouseButtonPressed && event.key.code == Mouse::Left) {
				click_btn_mouse(window, files, path, picture, sys, pos);
				picture_check_drag(picture, pos, sys);
			}

			if (event.type == Event::MouseButtonReleased && event.key.code == Mouse::Left) {
				picture.drag_move = false;
			}

			click_btn(window, files, path, picture, sys, pos);

			if (event.type == Event::Resized) {
				sys.window_size = window.getSize();
				picture_mid(sys.window_size, picture);
			}

		}

		window.clear(Color::White);
		view = View(FloatRect(0, 0, float(sys.window_size.x), float(sys.window_size.y)));
		window.setView(view);

		draw_elements(window, sys, picture);
		window.display();
		sys.window_size = window.getSize();
	}
}

int main() {
	Sys sys;
	Picture picture;
	cout << "input path " << "\n";
	//
	string path;
	while (!directory_exists(sys.dir_path)) {
		sys.dir_path = "C:\\images\\";
		path = sys.dir_path;
		//cin >> sys.dir_path;
	}
	//
	RenderWindow window(VideoMode(sys.window_size.x, sys.window_size.y), "Image Viewer");
	window.setVerticalSyncEnabled(true);
	View view = View(FloatRect(0, 0, float(sys.window_size.x), float(sys.window_size.y)));
	window.setView(view);
	vector<string> files = get_file_list(sys.dir_path);

	if (files.size() > 0) {
		window.setTitle(init_picture(window, files, path, picture, sys));
	}
	picture.is_initializing_toolbar(picture);
	sys.window_size = window.getSize();
	picture_mid(sys.window_size, picture);

	start_program(window, files, path, picture, sys, view);

	return 0;
}
