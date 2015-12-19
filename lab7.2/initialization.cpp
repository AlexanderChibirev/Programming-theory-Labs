#include "initialization.h"
#include "Config.h"

Block::Block(Vector2f position, Vector2f size, float block_speed) {
	block_position = position;
	block_size = size;
	block = new RectangleShape(block_size);
    block_color1 = rand() % 255 + 1;
	block_color2 = rand() % 255 + 1;
	block_color3 = rand() % 255 + 1;
	block->setPosition(block_position);
	start_position = block_position;
	block->setFillColor(Color(block_color1, block_color2, block_color3));
	block->setOrigin(block_size.x / 2, block_size.y / 2);
	speed = block_speed;
}

void Colony::up() {
	for (Block &thing : blocks) {
		if (thing.block->getPosition().y > UPPER_LIMIT) {
			thing.block->move(0, -thing.speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[6].block->getPosition().y)) {
		situation = "effect3";
	}
}

void Colony::down() {
	for (Block &thing : blocks) {
		if (thing.block->getPosition().y < LOWER_LIMIT) {
			thing.block->move(0, thing.speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[6].block->getPosition().y)) {
		situation = "effect3";
	}
}

void Colony::come_back_up() {
	for (Block &thing : blocks) {
		if (thing.block->getPosition().y < thing.block_position.y) {
			thing.block->move(0, +thing.speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[0].block_position.y) &&  int(blocks[6].block->getPosition().y) == int(blocks[6].block_position.y)) {
		situation = "generation_effect";
	}
}

void Colony::come_back_down() {
	for (Block &thing : blocks) {
		if (thing.block->getPosition().y > thing.block_position.y) {
			thing.block->move(0, -thing.speed);
		}
	}
	if (int(blocks[0].block->getPosition().y) == int(blocks[0].block_position.y) &&  int(blocks[6].block->getPosition().y) == int(blocks[6].block_position.y)) {
		situation = "generation_effect";
	}
}




void Colony::effect_rotate() {
	for (Block &thing : blocks)
		thing.block->setRotation(float(thing.block->getRotation() + 0.1));
}


void initiation(Colony & colony) {
	for (int i = 0; i < 7; ++i) {
		Block new_block({ START_POS.x - i * SIZE_BLOCK.x, START_POS.y - i * SIZE_BLOCK.y }, SIZE_BLOCK, SPEED); //смещение
		colony.blocks.push_back(new_block);
	}
}


void draw(Colony & colony, RenderWindow & window) 
{
	for(Block &thing: colony.blocks)
		window.draw(*thing.block);
	window.display();
	window.clear(Color::White);
}


void delete_blocks(Colony & colony) {
	for (Block &thing : colony.blocks)
		delete &thing;
}