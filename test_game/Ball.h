#pragma once
#include "Framework.h"
#include "HeadSprite.h"

class Ball : public HeadSprite
{
public:
	Ball(double platform_x, double platform_y){
		current_ball = 1;
		for (int i = 1; i <= 4; i++)
		{
			sprites_ball[i] = createSprite(("data/platform/bullet/" + std::to_string(i) + ".png").c_str());
		}
		getSpriteSize(sprites_ball[current_ball], width, height);
		setXY(platform_x, platform_y - height / 2);
		x_speed = 0;
		y_speed = 0;
		speed = 4;
	};

	void draw() {
		drawSprite(sprites_ball[current_ball], x - width / 2, y - height / 2);
	}

	void move(int window_w, int w, int h, int p_x, int p_y) {
		if ((x - width / 2 < 0) ||
			(x + width / 2 > window_w)
			//|| ((x + width / 2 > p_x - w / 2) && ((y + height / 2 >= p_y - h / 2) && (y - height / 2 <= p_y + h / 2)))||
			//((x - width / 2 > p_x + w / 2) && ((y + height / 2 >= p_y - h / 2) && (y - height / 2 <= p_y + h / 2)))
			)
		{
			x_speed *= -1;
		}

		if ((y - height / 2 < 0) ||
			((y - height / 2 > p_y - h / 2)
				&& (x - width / 2 <= p_x + w / 2) && (x + width / 2 >= p_x - w / 2)))
		{
			y_speed *= -1;
		}

		x += x_speed;
		y += y_speed;
	}

	void shoot(double aim_x, double aim_y) {
		x_speed = aim_x - x;
		y_speed = aim_y - y;
		double param = abs(y_speed) + abs(x_speed);
		param = speed / param;
		y_speed *= param;
		x_speed *= param;
	}

private:
	int x_speed;
	int y_speed;
	int speed;

	std::map<int, Sprite*> sprites_ball;
	int current_ball;
};
