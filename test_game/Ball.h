#pragma once
#include "Framework.h"
#include "HeadSprite.h"
#include "Tile.h"

class Ball : public HeadSprite
{
public:
	Ball(double platform_x, double platform_y){
		current_ball = 2;
		for (int i = 1; i <= 4; i++)
		{
			sprites_ball[i] = createSprite(("data/platform/bullet/" + std::to_string(i) + ".png").c_str());
		}
		getSpriteSize(sprites_ball[current_ball], width, height);
		setXY(platform_x, (platform_y - height / 2));
		if (height == width)
		{
			radius = height / 2;
		}
		x_speed = 0;
		y_speed = 0;
		x_delay = 0;
		y_delay = 0;
	};

	void draw() {
		drawSprite(sprites_ball[current_ball], x - width / 2, y - height / 2);
	}

	void move(int window_w, int w, int h, double p_x, double p_y) {
		const double SAFE_ZONE = 10;
		if ((x - radius < 0) ||
			(x + radius > window_w)
			||
			(((x + radius > p_x - w / 2) && (x - radius < p_x + w / 2))
				&& (y - radius <= p_y + h / 2 - SAFE_ZONE) && (y + radius >= p_y - h / 2 + SAFE_ZONE)))
		{
			if (!x_delay)
			{
				x_speed *= -1;
			}
			x_delay -= x_delay;
		}

		if ((y - radius < 0) ||
			(((y + radius > p_y - h / 2) && (y - radius < p_y + h / 2))
				&& (x - radius <= p_x + w / 2 - SAFE_ZONE) && (x + radius >= p_x - w / 2 + SAFE_ZONE)))
		{
			if (!y_delay)
			{
				y_speed *= -1;
			}
			y_delay -= y_delay;
		}

		x += x_speed;
		y += y_speed;
	}

	bool shoot(double aim_x, double aim_y, int w) {
		if (aim_y < y && (aim_x < x - w / 6 || aim_x > x + w / 6))
		{
			x_speed = aim_x - x;
			y_speed = aim_y - y;
			double param = abs(y_speed) + abs(x_speed);
			param = speed / param;
			y_speed *= param;
			x_speed *= param;
			return 1;
		}
		return 0;
	}

	bool checkColission(Tile*& tile) {
		TileCollision temp_collision = tile->checkColission(x, y, radius);
		switch (temp_collision)
		{
		case TileCollision::no:
			return 0;
		case TileCollision::vertical:
			if (!y_delay)
			{
				y_speed *= -1;
			}
			y_delay -= y_delay;
			return 1;
		case TileCollision::horisontal:
			if (!x_delay)
			{
				x_speed *= -1;
			}
			x_delay -= x_delay;
			return 1;
		case TileCollision::both:
			if (!x_delay)
			{
				x_speed *= -1;
			}
			x_delay -= x_delay;
			if (!y_delay)
			{
				y_speed *= -1;
			}
			y_delay -= y_delay;
			return 1;
		default:
			return 0;
		}
	}



private:
	double x_speed;
	double y_speed;
	const int speed = 2;

	double radius;

	bool x_delay;
	bool y_delay;

	std::map<int, Sprite*> sprites_ball;
	int current_ball;
};
