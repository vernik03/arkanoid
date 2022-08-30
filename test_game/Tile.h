#pragma once
#include <string>
#include "Framework.h"
#include "HeadSprite.h"

enum class TileType {
	intact,
	broken
};

enum class TileCollision {
	no,
	vertical,
	horisontal,
	both
};

enum class TileColor {
	blue,
	light_green,
	purple,
	red,
	orange,
	light_blue,
	yellow,
	green,
	grey,
	brown
};

class Tile : public HeadSprite
{
public:
	Tile() {};
	
	Tile(TileColor new_color, double new_x = 0, double new_y = 0) {
		tile_sprites[TileType::broken] = createSprite(("data/tiles/" + str(TileType::broken) + "/" + std::to_string(static_cast<int>(new_color)+1) + ".png").c_str());
		tile_sprites[TileType::intact] = createSprite(("data/tiles/" + str(TileType::intact) + "/" + std::to_string(static_cast<int>(new_color)+1) + ".png").c_str());
		getSpriteSize(tile_sprites[TileType::intact], width, height);
		setXY(new_x, new_y);
		is_broken = 0;
		is_enable = 1;
		color = new_color;
	}

	Tile(int new_color, double new_x = 0, double new_y = 0) {
		is_broken = 0;
		is_enable = 1;

		if (new_color == 0)
		{
			is_broken = 1000;
			is_enable = 0;
			new_color = 1;
		}
		
		tile_sprites[TileType::broken] = createSprite(("data/tiles/" + str(TileType::broken) + "/" + std::to_string(new_color) + ".png").c_str());
		tile_sprites[TileType::intact] = createSprite(("data/tiles/" + str(TileType::intact) + "/" + std::to_string(new_color) + ".png").c_str());
		getSpriteSize(tile_sprites[TileType::intact], width, height);
		setXY(new_x, new_y);
		color = static_cast<TileColor>(new_color-1);
		
	}

	virtual TileCollision checkColission(double ball_x, double ball_y, double ball_radius, int& score) {
		if (is_broken > 0 || !is_enable)
		{
			return TileCollision::no;
		}
		const double SAFE_ZONE = 5;
		TileCollision result = TileCollision::no;
		if (((x + width / 2 > ball_x - ball_radius) && (x - width / 2 < ball_x + ball_radius))
			&& (y - height / 2 <= ball_y + ball_radius - SAFE_ZONE) && (y + height / 2 >= ball_y - ball_radius + SAFE_ZONE))
		{
			result = TileCollision::horisontal;
		}
		if (((y + height / 2 > ball_y - ball_radius) && (y - height / 2 < ball_y + ball_radius))
			&& (x - width / 2 <= ball_x + ball_radius - SAFE_ZONE) && (x + width / 2 >= ball_x - ball_radius + SAFE_ZONE))
		{
			if (result == TileCollision::horisontal)
			{
				result = TileCollision::both;
			}
			else
			{
				result = TileCollision::vertical;
			}
		}
		if (result != TileCollision::no)
		{
			is_broken++;
			score++;
		}
		return result;
	}
	
	void breakTile(int& score) {
		if (is_broken == 0)
		{
			is_broken++;
			score++;
		}
	}

	virtual void draw() {
		getSpriteSize(tile_sprites[TileType::intact], width, height);

		const int broken_maximum = 70;
		if (is_broken >= broken_maximum && is_enable)
		{
			is_enable = 0;
		}
		if (is_broken > 0 && is_broken < broken_maximum)
		{
			drawSprite(tile_sprites[TileType::broken], x - width / 2, y - height / 2);
			is_broken++;
		}
		else if(is_enable)
		{
			drawSprite(tile_sprites[TileType::intact], x - width / 2, y - height / 2);
		}
		
	}

	TileColor getColor() {
		return color;
	}

	virtual void setSize(int w, int h) {
		setSpriteSize(tile_sprites[TileType::broken], w, h);	
		setSpriteSize(tile_sprites[TileType::intact], w, h);
		width = w;
		height = h;
	}

	bool isBroken() {
		if (is_broken > 0 || !is_enable)
		{
			return true;
		}
		return false;
	}

	bool isEnable() {
		if (is_enable)
		{
			return true;
		}
		return false;
	}

	void disable() {
		is_enable = 0;
	}
	
protected:
	std::string str(TileType type);

	TileColor color;

	std::map<TileType, Sprite*> tile_sprites;

	int is_broken;
	bool is_enable;
	
};


std::string Tile::str(TileType type) {
	switch (type)
	{
	case TileType::intact:
		return "intact";
	case TileType::broken:
		return "broken";
	default:
		return "";
	}
}

enum class Abilities {
	slow,
	fast,
	big,
	small,
	three
};

class Bonus : public Tile
{
public:
	Bonus(Abilities new_type, int w, int h, double new_x, double new_y){
		sprite = createSprite(("data/abilities/" + std::to_string(static_cast<int>(new_type) + 1) + ".png").c_str());
		setXY(new_x, new_y);
		setSize(w, h);
		is_enable = 1;
		type = new_type;
	};

	Bonus(int new_type, int w, int h, double new_x, double new_y) {
		sprite = createSprite(("data/abilities/" + std::to_string(new_type) + ".png").c_str());
		setXY(new_x, new_y);
		setSize(w, h);
		is_enable = 1;
		type = static_cast<Abilities>(new_type - 1);
	};
	
	void draw() override {
		if (is_enable)
		{
			getSpriteSize(sprite, width, height);
			drawSprite(sprite, x - width / 2, y - height / 2);
		}
	}

	void move() {
		y += speed;
	}

	void setSize(int w, int h) override {
		getSpriteSize(sprite, width, height);
		height *= (double)w / (double)width;
		setSpriteSize(sprite, w, height);
		getSpriteSize(sprite, width, height);
	}

	bool checkCatch(int w, int h, double p_x, double p_y) {
		if ((((x + width / 2 > p_x - w / 2) && (x - width / 2 < p_x + w / 2))
			&& (y - height / 2 <= p_y + h / 2) && (y + height / 2 >= p_y - h / 2))
			||
			(((y + height / 2 > p_y - h / 2) && (y - height / 2 < p_y + h / 2))
				&& (x - width / 2 <= p_x + w / 2) && (x + width / 2 >= p_x - w / 2)))
		{
			return 1;
		}
		return 0;
	}

	Abilities getType() {
		return type;
	}

	bool checkBonusColission(double p_x, double p_y, double w, double h) {
		const double SAFE_ZONE = 5;
		TileCollision result = TileCollision::no;
		if ((((x + width / 2 > p_x - w / 2) && (x - width / 2 < p_x + w / 2))
			&& (y - height / 2 <= p_y + h / 2 - SAFE_ZONE) && (y + height / 2 >= p_y - h / 2 + SAFE_ZONE))
		|| (((y + height / 2 > p_y - h / 2) && (y - height / 2 < p_y + h / 2))
			&& (x - width / 2 <= p_x + w / 2 - SAFE_ZONE) && (x + width / 2 >= p_x - w / 2 + SAFE_ZONE)))
		{
			return 1;
		}
		return 0;
	}

private:
	const int speed = 5;
	Abilities type;
};
