#pragma once
#include <string>
#include "Framework.h"

enum class TileType {
	intact,
	broken,
	icon
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

class Tile
{
public:
	Tile(TileType type, TileColor color, double new_x = 0, double new_y = 0) {
		sprite = createSprite(("data/tiles/" + str(type) + "/" + str(color) + ".png").c_str());
		getSpriteSize(sprite, width, height);
		SetXY(new_x, new_y);
	}

	Tile(TileType type, int color, double new_x = 0, double new_y = 0) {
		sprite = createSprite(("data/tiles/" + str(type) + "/" + std::to_string(color) + ".png").c_str());
		getSpriteSize(sprite, width, height);
		SetXY(new_x, new_y);
	}

	void draw() {
		drawSprite(sprite, x, y);
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	double getX() {
		return x;
	}

	double getY() {
		return y;
	}

	void SetX(double new_x) {
		x = new_x;
	}

	void SetY(double new_y) {
		y = new_y;
	}

	void SetXY(double new_x, double new_y) {
		x = new_x;
		y = new_y;
	}

protected:
	std::string str(TileType type);
	std::string str(TileColor color);

	Sprite* sprite;

	int width;
	int height;

	double x;
	double y;
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

