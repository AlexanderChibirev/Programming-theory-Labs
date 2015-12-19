#pragma once
#include "SFML\Graphics.hpp"
#include <vector>

using namespace sf;

struct Block {
	Block(Vector2f position, Vector2f size, float block_speed);
	RectangleShape * block;
	Vector2f block_size;
	Vector2f block_position;
	Vector2f start_position;
	float speed;

	int block_color1;
	int block_color2;
	int block_color3;
	float color_iter;

	float block_rotate;

};

struct Colony {
	std::vector<Block> blocks;
	String situation = "generation_effect";
	//int location = 1;

	int direction;

	void up();
	void down();

	void come_back_up();
	void come_back_down();

	void effect_rotate();

};

void initiation(Colony & colony);

void draw(Colony & colony, RenderWindow & window);

void delete_blocks(Colony & colony);