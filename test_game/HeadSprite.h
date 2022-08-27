#pragma once
#include "Framework.h"

class HeadSprite
{
public:
	HeadSprite(){}
	
	HeadSprite(const char* path, double new_x = 0, double new_y = 0) {
		sprite = createSprite(path);
		getSpriteSize(sprite, width, height);
		setXY(new_x, new_y);
	}

	void draw() {
		drawSprite(sprite, x - width / 2, y - height / 2);
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	void setSize(int w, int h) {
		setSpriteSize(sprite, w, h); // O_o
		getSpriteSize(sprite, width, height);
	}

	double getX() {
		return x;
	}

	double getY() {
		return y;
	}

	void setX(double new_x) {
		x = new_x;
	}

	void setY(double new_y) {
		y = new_y;
	}

	void setXY(double new_x, double new_y) {
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