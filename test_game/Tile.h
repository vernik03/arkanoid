#pragma once
#include <string>
#include "Framework.h"
#include "HeadSprite.h"

enum class TileType {
	intact,
	broken,
	icon
};

enum class TileCollision {
	no,
	vertical,
	horisontal
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
	Tile(TileType type, TileColor color, double new_x = 0, double new_y = 0) {
		sprite = createSprite(("data/tiles/" + str(type) + "/" + str(color) + ".png").c_str());
		getSpriteSize(sprite, width, height);
		setXY(new_x, new_y);
	}

	Tile(TileType type, int color, double new_x = 0, double new_y = 0) {
		sprite = createSprite(("data/tiles/" + str(type) + "/" + std::to_string(color) + ".png").c_str());
		getSpriteSize(sprite, width, height);
		setXY(new_x, new_y);
	}

	TileCollision checkColission(double ball_x, double ball_y, double ball_radius) {
		if (((y + height / 2 > ball_y - ball_radius) && (y - height / 2 < ball_y + ball_radius))
				&& (x - width / 2 <= ball_x + ball_radius) && (x + width / 2 >= ball_x - ball_radius))			
		{
			return TileCollision::vertical;
		}
		else if (((x + width / 2 > ball_x - ball_radius) && (x - width / 2 > ball_x + ball_radius)) &&
				((y + height / 2 < ball_y - ball_radius) && (y - height / 2 > ball_y + ball_radius)))
		{
			return TileCollision::horisontal;
		}
		else
		{
			return TileCollision::no;
		}
	}

protected:
	std::string str(TileType type);
	std::string str(TileColor color);
};


std::string Tile::str(TileType type) {
	switch (type)
	{
	case TileType::intact:
		return "intact";
	case TileType::broken:
		return "broken";
	case TileType::icon:
		return "icons";
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

