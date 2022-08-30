#pragma once
#include <map>
#include <string>
#include "Framework.h"
#include "HeadSprite.h"
#include "Ball.h"

enum class Condition {
	simple,
	//shoot,
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
		/*for (int i = 1; i <= 3; i++)
		{
			sprites_shoot[i] = createSprite(("data/platform/shoot/" + std::to_string(i) + ".png").c_str());
			setSize(sprites_simple[i], getWidth() / 2, getHeight() / 2);
		}*/
		for (int i = 1; i <= 3; i++)
		{
			sprites_big[i] = createSprite(("data/platform/big/" + std::to_string(i) + ".png").c_str());
			setSize(sprites_big[i], getWidth() / 2, getHeight() / 2);
		}
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
		catch_delay = 0;
	}
	~Platform() {};

	void draw() override {
		ball->draw();
		if (getTickCount() - timer > 50)
		{
			//rand_sprite = int(rand() % 3 + 1);
			timer = getTickCount();
		}
		switch (condition)
		{
		case Condition::simple:
			getSpriteSize(sprites_simple[1], width, height);
			drawSprite(sprites_simple[int(timer % 3 + 1)], x - width / 2, y - height / 2);
			break;
		/*case Condition::shoot:
			drawSprite(sprites_shoot[int(timer % 3 + 1)], x - width / 2, y - height / 2);
			break;*/
		case Condition::big:
			getSpriteSize(sprites_big[1], width, height);
			drawSprite(sprites_big[int(timer % 3 + 1)], x - width / 2, y - height / 2);
			break;
		case Condition::small:
			getSpriteSize(sprite_small, width, height);
			
			drawSprite(sprite_small, x - width / 2, y - height / 2);
			break;
		default:
			break;
		}
	}

	void move() {
		
		x += right_speed;
		x += left_speed;
		if (x < getWidth() / 2)
		{
			stopLeft();
		}
		if (x > window_w - getWidth() / 2)
		{
			stopRight();
		}
	}

	void moveLeft() {
		left_speed = -c_speed;
	}

	void moveRight() {
		right_speed = c_speed;
	}

	void setSize(Sprite*& temp_sprite, int w, int h) {
		getSpriteSize(temp_sprite, w, h);
		setSpriteSize(temp_sprite, w/2, h/2); // O_o		
	}
	
	void setBig() {
		condition = Condition::big;
		getSpriteSize(sprites_big[1], width, height);

	}

	void setSmall() {
		condition = Condition::small;
		getSpriteSize(sprite_small, width, height);
	}


	void stopLeft() {
		left_speed = 0;
	}

	void stopRight() {
		right_speed = 0;
	}

	void moveBall() {
		if (!is_in_game)
		{
			ball->setX(x);
		}
		else
		{
			
			if (ball->move(window_w, width, height, x, y))
			{
				tryToCatch();
			}
		}
	}

	bool checkBall() {
		if (ball->getY() + ball->getHeight() / 2 >= window_h)
		{
			return 1;
		}
		return 0;
	}

	bool checkCollision(Tile*& tile, int& score) {
		return ball->checkCollision(tile, score);
	}

	bool checkBonusColission(Bonus*& bonus, int& score) {
		bool res = bonus->checkBonusColission(x, y, width, height);
		if (res)
		{
			Abilities type = bonus->getType();
			switch (type)
			{
			case Abilities::slow:
				break;
			case Abilities::fast:
				
				break;
			case Abilities::big:
				setBig();
				break;
			case Abilities::small:
				setSmall();
				break;
			case Abilities::three:
				break;
			default:
				break;
			}
		}
		return res;
	}

	void shootBall(double aim_x, double aim_y) {
		if (!is_in_game)
		{
			is_in_game = ball->shoot(aim_x, aim_y, width);
		}
	}

	void tryToCatch() {
		int percent = rand() % 100;
		if (percent < 15)
		{
			if (catch_delay == 0)
			{
				is_in_game = 0;
				ball = new Ball(x, y - height / 2);
			}
			catch_delay ++;
			if (catch_delay > 10)
			{
				catch_delay = 0;
			}
		}
	}

private:
	const int c_speed = 3;
	double right_speed;
	double left_speed;
	bool is_in_game;

	Ball* ball;

	unsigned int timer; 
	//int rand_sprite;
	
	int window_w;
	int window_h;

	int catch_delay;

	Condition condition;
	std::map<int, Sprite*> sprites_simple;
	//std::map<int, Sprite*> sprites_shoot;
	std::map<int, Sprite*> sprites_big;
	Sprite* sprite_big;
	Sprite* sprite_small;
};