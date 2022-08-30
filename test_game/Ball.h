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

	void draw() override {
		drawSprite(sprites_ball[current_ball], x - width / 2, y - height / 2);
	}

	bool move(int window_w, int w, int h, double p_x, double p_y) {
		const double SAFE_ZONE = 5;
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

		int dir = checkPlatformSideCollision(w, h, p_x, p_y);
		x_speed += dir;
		if ((dir == -1) && (abs(p_x - x) < w / 2 + radius)) {
			x = p_x - w / 2 - radius - 5;
		}
		if ((dir == 1) && (abs(p_x - x) < w / 2 + radius)) {
			x = p_x + w / 2 + radius + 5;
		}
		return checkCatch(w, h, p_x, p_y);
	}

	int checkPlatformSideCollision(int w, int h, double p_x, double p_y) {
		if (y > p_y) {
			if ((p_x - w / 2 - x >= 0) && (p_x - w / 2 - x < radius)) {
				return -1;
			}
			else if ((p_x + w / 2 - x >= 0) && (p_x + w / 2 - x < radius)) {
				return 1;
			}
		}
		else if (y > p_y - h / 2) {
			if ((p_x - w / 2 - x >= 0) && (p_x - w / 2 - x < radius)) {
				return -1;
			}
			else if ((p_x + w / 2 - x >= 0) && (p_x + w / 2 - x < radius)) {
				return 1;
			}
		}
		return 0;
	}

	bool checkCatch(int w, int h, double p_x, double p_y) {
		if (y + radius > p_y - h / 2)
		{
			if (abs(p_x - (x - radius)) <= (w / 2) * 0.95) {
				return 1;
			}

		}

		return 0;
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

	
	 bool checkCollision(Tile*& tile, int& score) {
		if (dynamic_cast<Bonus*>(tile) != nullptr) {
			return 0;
		}
		TileCollision temp_collision = tile->checkCollision(x, y, radius, score);
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

	bool checkBonusCollision(Tile*& bounus, int& score) {
		TileCollision temp_collision = bounus->checkCollision(x, y, radius, score);
		switch (temp_collision)
		{
		case TileCollision::no:
			return 0;
		case TileCollision::both:
			return 1;
		default:
			return 0;
		}
	}

	Abilities getType() {
		return type;
	}

	void removeSpeed(double number) {
		x_speed /= number;
		y_speed /= number;
		speed /= number;
		type = Abilities::slow;
	}

	void addSpeed(double number) {
		x_speed *= number;
		y_speed *= number;
		speed *= number;
		type = Abilities::fast;
	}


private:
	double x_speed;
	double y_speed;
	double speed = 2;

	double radius;

	bool x_delay;
	bool y_delay;

	Abilities type;

	std::map<int, Sprite*> sprites_ball;
	int current_ball;
};
