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
		extra_ball_1 = new Ball(x, y - height / 2);
		extra_ball_2 = new Ball(x, y - height / 2);
		extra_ball_1->setY(window_h + 200);
		extra_ball_2->setY(window_h + 200);
		catch_delay = 0;
		bonus = Abilities::none;
	}
	~Platform() {};

	void draw() override {
		ball->draw();
		if (ballIsAlive(extra_ball_1))
		{
			extra_ball_1->draw();
		}
		if (ballIsAlive(extra_ball_2))
		{
			extra_ball_2->draw();
		}

		if (getTickCount() - timer > 50)
		{
			timer = getTickCount();
		}
		switch (condition)
		{
		case Condition::simple:
			getSpriteSize(sprites_simple[1], width, height);
			drawSprite(sprites_simple[int(timer % 3 + 1)], x - width / 2, y - height / 2);
			break;
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
		setSpriteSize(temp_sprite, w/2, h/2); 	
	}
	
	void setBig() {
		condition = Condition::big;
		getSpriteSize(sprites_big[1], width, height);

	}

	void setSmall() {
		condition = Condition::small;
		getSpriteSize(sprite_small, width, height);
	}

	void addBalls() {
		extra_ball_1->setX(ball->getX());
		extra_ball_1->setY(ball->getY());
		extra_ball_2->setY(ball->getY());
		extra_ball_2->setX(ball->getX());
		
		extra_ball_2->setSpeed(ball->getXSpeed() / 1.1, ball->getYSpeed() * 1.1);
		extra_ball_1->setSpeed(ball->getXSpeed() * 1.1, ball->getYSpeed() / 1.1);
		
	}

	void stopLeft() {
		left_speed = 0;
	}

	void stopRight() {
		right_speed = 0;
	}

	void moveBall() {
		moveEveryBall(ball);
		if (ballIsAlive(extra_ball_1))
		{
			moveEveryBall(extra_ball_1);
		}
		if (ballIsAlive(extra_ball_2))
		{
			moveEveryBall(extra_ball_2);
		}
	}

	bool ballIsAlive(Ball*& every_ball) {
		return every_ball->getY() < window_h + 20;
	}

	bool allBallsIsAlive() {
		return ballIsAlive(extra_ball_1) && ballIsAlive(extra_ball_2) && ballIsAlive(ball);
	}

	bool anyBallIsAlive() {
		return ballIsAlive(extra_ball_1) || ballIsAlive(extra_ball_2);
	}

	void moveEveryBall(Ball*& every_ball) {
		if(!is_in_game)
		{
			every_ball->setX(x);
		}
		else
		{
			if (every_ball->move(window_w, width, height, x, y) && !anyBallIsAlive() && bonus != Abilities::teleport)
			{
				tryToCatch();
			}
		}
	}

	bool checkBall() {
		if (allBallsIsAlive())
		{
			return 0;
		}
		else if (!ballIsAlive(ball))
		{
			if (ballIsAlive(extra_ball_1))
			{
				ball->setXY(extra_ball_1->getX(), extra_ball_1->getY());
				ball->setSpeed(extra_ball_1->getXSpeed(), extra_ball_1->getYSpeed());
				
				extra_ball_1->setY(window_h + 200);
				return 0;
			}
			else if (ballIsAlive(extra_ball_2))
			{
				ball->setXY(extra_ball_2->getX(), extra_ball_2->getY());
				ball->setSpeed(extra_ball_2->getXSpeed(), extra_ball_2->getYSpeed());
				
				extra_ball_2->setY(window_h + 200);
				return 0;
			}
			else
			{
				return 1;
			}
		} 
		return 0;
	}

	bool checkCollision(Tile*& tile, int& score) {
		int collision = 0;
		collision += ball->checkCollision(tile, score);
		if (ballIsAlive(extra_ball_1))
		{
			collision += extra_ball_1->checkCollision(tile, score);
		}
		if (ballIsAlive(extra_ball_2))
		{
			collision += extra_ball_2->checkCollision(tile, score);
		}
		return collision;
	}

	bool checkBonusColission(Bonus*& b, int& score) {
		bool res = b->checkBonusColission(x, y, width, height);
		if (res)
		{
			bonus = b->getType();
			switch (bonus)
			{
			case Abilities::slow:
				ball->removeSpeed(3);
				if (ballIsAlive(extra_ball_1))
				{
					extra_ball_1->removeSpeed(3);
				}
				if (ballIsAlive(extra_ball_2))
				{
					extra_ball_2->removeSpeed(3);
				}
				break;
			case Abilities::fast:
				ball->addSpeed(2);
				if (ballIsAlive(extra_ball_1))
				{
					extra_ball_1->addSpeed(2);
				}
				if (ballIsAlive(extra_ball_2))
				{
					extra_ball_2->addSpeed(2);
				}
				break;
			case Abilities::big:
				setBig();
				break;
			case Abilities::small:
				setSmall();
				break;
			case Abilities::three:
				addBalls();
				break;
			case Abilities::teleport:
				teleportBall();
				break;
			default:
				break;
			}
		}
		return res;
	}

	void teleportBall() {
		ball->setX(rand() % window_w);
		ball->setY(rand() % (window_h / 2));
	}

	Abilities getType() {
		return bonus;
	}

	Abilities getBallType() {
		return ball->getType();
	}

	void shootBall(double aim_x, double aim_y) {
		if (!is_in_game)
		{
			is_in_game = ball->shoot(aim_x, aim_y, width);

			extra_ball_1->setY(window_h + 200);
			extra_ball_2->setY(window_h + 200);
		}
	}

	void tryToCatch() {
		int percent = rand() % 100;
		if (percent < 15)
		{
			if (catch_delay == 0)
			{
				is_in_game = 0;
				Abilities temp_a = ball->getType();
				double temp_s = ball->getSpeedNum();
				ball = new Ball(x, y - height / 2);
				ball->setType(temp_a, temp_s);

				
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
	Ball* extra_ball_1;
	Ball* extra_ball_2;

	unsigned int timer; 
	
	int window_w;
	int window_h;

	int catch_delay;

	Condition condition;
	std::map<int, Sprite*> sprites_simple;
	std::map<int, Sprite*> sprites_big;
	Sprite* sprite_big;
	Sprite* sprite_small;

	Abilities bonus;
};