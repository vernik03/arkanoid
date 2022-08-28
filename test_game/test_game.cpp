
#include <random>
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <iterator>
#include <regex>
#include <sstream>
#include "Framework.h"
#include "Text.h"
#include "Tile.h"
#include "Platform.h"

class TileManager
{
public:
	TileManager() {
		for (int i = 0; i <= 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Tile* temp_tile = new Tile(int(rand() % 10 + 1));
				temp_tile->setSize(temp_tile->getWidth() / 2, temp_tile->getHeight() / 2);
				temp_tile->setXY(i * temp_tile->getWidth() + temp_tile->getWidth() / 2, j * temp_tile->getHeight() + temp_tile->getHeight() / 2);				
				tiles.push_back(temp_tile);
			}			
		}
	};
	~TileManager(){};

	void drawAll() {
		for (auto& tile : tiles)
		{
			tile->draw();
		}
	}


	void checkColission(std::unique_ptr<Platform>& platform) {
		for (auto tile : tiles)
		{
			if (platform->checkColission(tile))
			{
				
			}
			
		}
	}

private:
	std::vector <Tile*> tiles;
};

class Reticle : public HeadSprite
{
public:
	Reticle() {
		sprite = createSprite("data/reticle.png");
		getSpriteSize(sprite, width, height);
		setSize(getWidth() / 3, getHeight() / 3);
	}


private:

};

/* Test Framework realization */
class MyFramework : public Framework {

public:
	
	MyFramework(int w, int h, bool fs) : input_width(w), input_height(h), input_fullscreen(fs) {}

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = input_width;
		height = input_height;
		fullscreen = input_fullscreen;
		if (fullscreen)
		{
			window_w = 1920;
			window_h = 1080;
		}
		else
		{
			window_w = input_width;
			window_h = input_height;
		}
	}

	virtual bool Init() {

		text = std::make_unique<Text>();
		tile_m = std::make_unique<TileManager>();
		platform = std::make_unique<Platform>(window_w, window_h);		
		reticle = std::make_unique<Reticle>();
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
		showCursor(false);
		
		drawTestBackground();


		tile_m->checkColission(platform);
		
		tile_m->drawAll();
		platform->moveBall();
		platform->move();
		

		platform->draw();
		if (platform->checkBall())
		{
			return true;
		}

		reticle->draw();
		//text->print("Hello world!", 300, 300, Size::medium, Align::center, VAlign::center);
		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		reticle->setXY(x, y);
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
		switch (button)
		{
		case FRMouseButton::LEFT:
			platform->shootBall(reticle->getX(), reticle->getY());
			break;
		case FRMouseButton::MIDDLE:
			break;
		case FRMouseButton::RIGHT:
			break;
		case FRMouseButton::COUNT:
			break;
		default:
			break;
		}
	}

	virtual void onKeyPressed(FRKey k) {
		switch (k)
		{
		case FRKey::RIGHT:
			platform->moveRight();
			break;
		case FRKey::LEFT:
			platform->moveLeft();
			break;
		case FRKey::DOWN:
			break;
		case FRKey::UP:
			break;
		case FRKey::COUNT:
			break;
		default:
			break;
		}
		
	}

	virtual void onKeyReleased(FRKey k) {
		switch (k)
		{
		case FRKey::RIGHT:
			platform->stopRight();
			break;
		case FRKey::LEFT:
			platform->stopLeft();
			break;
		case FRKey::DOWN:
			break;
		case FRKey::UP:
			break;
		case FRKey::COUNT:
			break;
		default:
			break;
		}
	}

	virtual const char* GetTitle() override
	{
		return "Arcanoid";
	}

private:
	int input_width;
	int input_height;
	bool input_fullscreen;

	int window_w;
	int window_h;
	
	std::unique_ptr<Text> text;
	std::unique_ptr<TileManager> tile_m;
	std::unique_ptr<Reticle> reticle;
	std::unique_ptr<Platform> platform;
};

int main(int argc, char* argv[])
{
	int x = 800;
	int y = 600;
	bool fullscreen = false;
	for (int i = 0; i < argc - 1; i++)
	{
		if (strcmp(argv[i], "-window") == 0)
		{
			std::string str = std::string(argv[i + 1]);
			std::istringstream iss(argv[i]);
			static const std::regex rdelim{ "x" };
			std::vector<std::string> strPairs{
					std::sregex_token_iterator(str.begin(), str.end(), rdelim, -1),
					std::sregex_token_iterator()
			};
			x = stoi(strPairs[0]);
			y = stoi(strPairs[1]);
		}
		else if (strcmp(argv[i], "-fullscreen") == 0)
		{
			std::string str = std::string(argv[i + 1]);
			if (str == "true")
			{
				fullscreen = true;
			}
		}
	}

	srand(time(NULL));


	while (true)
	{
		run(new MyFramework(x, y, fullscreen));
	}
	return 0;
}
