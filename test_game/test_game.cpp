
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
#include "TileManager.h"

class Background : public HeadSprite
{
public:
	Background(int w) {
		sprite = createSprite("data/background.png");
		width = w / 4;
		height = width;
		setSize(width, height);
	};

	void draw(int h) {
		int temp = h / height + 1;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < temp; j++)
			{
				drawSprite(sprite, width * i, height * j);
			}
		}
	}

private:

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

class Map
{
public:
	Map(int w, int h, std::string path = "") {
		counter_w = w / 192;
		counter_h = (h - 300) / 64;
		if (path == "")
		{
			createMap();
		}
	};

	void createMap() {
		for (int i = 0; i < counter_h; i++)
		{
			std::vector <int> temp;
			for (int j = 0; j < counter_w; j++)
			{
				int t = rand() % 9 + 1;
				int percent = rand() % 100;
				if (percent < 30)
				{
					t = 0;
				}
				temp.push_back(t);
			}
			tiles.push_back(temp);
		}
	}

	const std::vector<std::vector <int>> getMap() {
		return tiles;
	}

	int getCounterW() {
		return counter_w;
	}
	
	int getCounterH() {
		return counter_h;
	}
	

private:
	int counter_w;
	int counter_h;

	std::vector<std::vector <int>> tiles;
};

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
		platform = std::make_unique<Platform>(window_w, window_h);		
		reticle = std::make_unique<Reticle>();
		background = std::make_unique<Background>(window_w);
		map = std::make_unique<Map>(window_w, window_h);
		tile_m = std::make_unique<TileManager>(map->getMap(), window_w);

		score = 0;
		timer = 0;
		is_winner = 0;
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {

		
		if (getTickCount() - timer > 1500 && timer > 0)
		{
			return true;
		}
		
		showCursor(false);
		
		background->draw(window_h);
		tile_m->deleteDroppers(window_h);
		tile_m->checkCollision(platform, score);
		tile_m->drawAll();
		if (timer == 0)
		{
			platform->moveBall();
			platform->move();
		}
		else if (is_winner)
		{
			text->print("YOU WON!", window_w / 2, window_h / 2, Size::medium, Align::center, VAlign::center);
		}
		else
		{
			platform->moveBall();
			text->print("GAME OVER!", window_w / 2, window_h / 2, Size::medium, Align::center, VAlign::center);
		}
		
		

		platform->draw();
		if (timer == 0)
		{
			if (platform->checkBall())
			{
				timer = getTickCount();
			}
			if (tile_m->checkVictory())
			{
				timer = getTickCount();
				is_winner = 1;
			}
		}
		

		text->print(std::to_string(score), platform->getX(), platform->getY() + 2, Size::small, Align::center, VAlign::top);

		reticle->draw();
		
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

	unsigned int timer;

	bool is_winner;

	int score;
	
	std::unique_ptr<Text> text;
	std::unique_ptr<TileManager> tile_m;
	std::unique_ptr<Reticle> reticle;
	std::unique_ptr<Platform> platform;
	std::unique_ptr<Background> background;
	std::unique_ptr<Map> map;
};

int main(int argc, char* argv[])
{
	int x = 1800;
	int y = 1600;
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
