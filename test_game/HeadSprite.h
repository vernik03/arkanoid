#pragma once
#include "Framework.h"

class HeadSprite
{
public:
	HeadSprite(){}
	
	HeadSprite(const char* path, double new_x = 0, double new_y = 0) {
		sprite = createSprite(path);
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

	void setSize(int w, int h) {
		//setSpriteSize(sprite, w, h); // O_o
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
	Sprite* sprite;

	int width;
	int height;

	double x;
	double y;
};