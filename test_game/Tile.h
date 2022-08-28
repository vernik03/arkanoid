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
	Orange,
	light_blue,
	yellow,
	green,
	grey,
	brown
};

class Tile : public HeadSprite
{
public:
	Tile(TileColor color, double new_x = 0, double new_y = 0) {
		tile_sprites[TileType::broken] = createSprite(("data/tiles/" + str(TileType::broken) + "/" + str(color) + ".png").c_str());
		tile_sprites[TileType::intact] = createSprite(("data/tiles/" + str(TileType::intact) + "/" + str(color) + ".png").c_str());
		getSpriteSize(tile_sprites[TileType::intact], width, height);
		setXY(new_x, new_y);
		is_broken = 0;
		is_enable = 1;
	}

	Tile(int color, double new_x = 0, double new_y = 0) {
		tile_sprites[TileType::broken] = createSprite(("data/tiles/" + str(TileType::broken) + "/" + std::to_string(color) + ".png").c_str());
		tile_sprites[TileType::intact] = createSprite(("data/tiles/" + str(TileType::intact) + "/" + std::to_string(color) + ".png").c_str());
		getSpriteSize(tile_sprites[TileType::intact], width, height);
		setXY(new_x, new_y);
		is_broken = 0;
		is_enable = 1;
	}

	TileCollision checkColission(double ball_x, double ball_y, double ball_radius) {
		if (is_broken > 0 || !is_enable)
		{
			return TileCollision::no;
		}
		const double SAFE_ZONE = 3;
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
		}
		return result;
	}

	void draw() {
		getSpriteSize(tile_sprites[TileType::intact], width, height);

		const int broken_maximum = 20;
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


	void setSize(int w, int h) {
		setSpriteSize(tile_sprites[TileType::broken], w, h);	
		setSpriteSize(tile_sprites[TileType::intact], w, h);
		width = w;
		height = h;

	}

protected:
	std::string str(TileType type);
	std::string str(TileColor color);

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

std::string Tile::str(TileColor color) {
	switch (color)
	{
	case TileColor::blue:
		return "1";
		break;
	case TileColor::light_green:
		return "2";
		break;
	case TileColor::purple:
		return "3";
		break;
	case TileColor::red:
		return "4";
		break;
	case TileColor::Orange:
		return "5";
		break;
	case TileColor::light_blue:
		return "6";
		break;
	case TileColor::yellow:
		return "7";
		break;
	case TileColor::green:
		return "8";
		break;
	case TileColor::grey:
		return "9";
		break;
	case TileColor::brown:
		return "10";
		break;
	default:
		break;
	}
}

