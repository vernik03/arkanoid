#pragma once
#include <map>
#include <string>
#include "Framework.h"
#include "HeadSprite.h"

enum class Condition {
	simple,
	shoot,
	big,
	small
};

class Platform : public HeadSprite
{
public:
	Platform(int new_window_w, int new_window_h) {
		for (int i = 1; i <= 3; i++)
		{
			sprites_simple[i] = createSprite(("data/platform/simple/" + std::to_string(i) + ".png").c_str());
			setSize(sprites_simple[i], getWidth() / 2, getHeight() / 2);
		}
		for (int i = 1; i <= 3; i++)
		{
			sprites_shoot[i] = createSprite(("data/platform/shoot/" + std::to_string(i) + ".png").c_str());
			setSize(sprites_simple[i], getWidth() / 2, getHeight() / 2);
		}
		sprite_big = createSprite("data/platform/big.png");
		setSize(sprite_big, getWidth() / 2, getHeight() / 2);
		sprite_small = createSprite("data/platform/small.png");
		setSize(sprite_small, getWidth() / 2, getHeight() / 2);
		window_w = new_window_w;
		window_h = new_window_h;
		getSpriteSize(sprites_simple[1], width, height);		
		SetXY(window_w / 2, window_h - height);
		condition = Condition::simple;
		timer = getTickCount();
	}
	~Platform() {};

	void draw() {
		getSpriteSize(sprites_simple[1], width, height);
		if (getTickCount() - timer > 50)
		{
			//rand_sprite = int(rand() % 3 + 1);
			timer = getTickCount();
		}
		switch (condition)
		{
		case Condition::simple:
			drawSprite(sprites_simple[int(timer % 3 + 1)], x - width / 2, y - height / 2);
			break;
		case Condition::shoot:
			drawSprite(sprites_shoot[int(timer % 3 + 1)], x - width / 2, y - height / 2);
			break;
		case Condition::big:
			drawSprite(sprite_big, x - width / 2, y - height / 2);
			break;
		case Condition::small:
			drawSprite(sprite_big, x - width / 2, y - height / 2);
			break;
		default:
			break;
		}
	}

	void moveLeft() {
		x -= speed;
	}

	void moveRight() {
		x += speed;
	}

	void setSize(Sprite*& temp_sprite, int w, int h) {
		getSpriteSize(temp_sprite, w, h);
		setSpriteSize(temp_sprite, w/1.5, h/1.5); // O_o		
	}

private:
	double speed = 20;


	unsigned int timer; 
	//int rand_sprite;
	
	int window_w;
	int window_h;

	Condition condition;
	std::map<int, Sprite*> sprites_simple;
	std::map<int, Sprite*> sprites_shoot;
	Sprite* sprite_big;
	Sprite* sprite_small;
};