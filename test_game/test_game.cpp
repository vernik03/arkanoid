
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

class TileManager
{
public:
	TileManager() {
		for (int i = 0; i <= 3; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Tile* temp_tile = new Tile(TileType::intact, int(rand() % 9 + 1));
				temp_tile->SetXY(i * temp_tile->getWidth() + temp_tile->getWidth() / 2, j * temp_tile->getHeight() + temp_tile->getHeight() / 2);
				temp_tile->setSize(temp_tile->getWidth() / 2, temp_tile->getHeight() / 2);\
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

private:
	std::vector <Tile*> tiles;
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
	}

	virtual bool Init() {

		text = std::make_unique<Text>();
		tile_m = std::make_unique<TileManager>();
		reticle = std::make_unique<HeadSprite>("data/reticle.png");
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
		showCursor(false);
		
		drawTestBackground();
		tile_m->drawAll();
		reticle->draw();
		//text->print("Hello world!", 300, 300, Size::medium, Align::center, VAlign::center);
		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		reticle->SetXY(x, y);
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {

	}

	virtual void onKeyPressed(FRKey k) {
	}

	virtual void onKeyReleased(FRKey k) {
	}

	virtual const char* GetTitle() override
	{
		return "Arcanoid";
	}

private:
	int input_width;
	int input_height;
	bool input_fullscreen;
	
	std::unique_ptr<Text> text;
	std::unique_ptr<TileManager> tile_m;
	std::unique_ptr<HeadSprite> reticle;
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
