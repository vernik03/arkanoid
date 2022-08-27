#pragma once
#include <map>
#include <string>
#include "Framework.h"
#include "HeadSprite.h"
#include "Ball.h"

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
		setXY(window_w / 2, window_h - height);
		condition = Condition::simple;
		timer = getTickCount();
		is_in_game = 0;
		ball = new Ball(x, y - height / 2);
	}
	~Platform() {};

	void draw() {
		getSpriteSize(sprites_simple[1], width, height);
		ball->draw();
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
		if (x > getWidth() / 2)
		{
			x -= speed;
		}
	}

	void moveRight() {
		if (x < window_w - getWidth() / 2)
		{
			x += speed;
		}
	}

	void setSize(Sprite*& temp_sprite, int w, int h) {
		getSpriteSize(temp_sprite, w, h);
		setSpriteSize(temp_sprite, w/1.5, h/1.5); // O_o		
	}

	void moveBall() {
		if (!is_in_game)
		{
			ball->setX(x);
		}
		else
		{
			ball->move(window_w, width, height, x, y);
		}
	}

	bool checkBall() {
		if (ball->getY() + ball->getHeight() / 2 >= window_h)
		{
			return 1;
		}
		return 0;
	}

	bool checkColission(Tile*& tile) {
		return ball->checkColission(tile);
	}

	void shootBall(double aim_x, double aim_y) {
		if (!is_in_game)
		{
			is_in_game = ball->shoot(aim_x, aim_y);
		}
	}

private:
	double speed = 40;
	bool is_in_game;

	Ball* ball;

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