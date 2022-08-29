#pragma once
#include <vector>
#include <map>
#include "Framework.h"
#include "HeadSprite.h"
#include "Tile.h"
#include "Platform.h"

struct Boom
{
	int start_tick;
	double x;
	double y;
};

class Explosion : public HeadSprite
{
public:
	Explosion() {
		for (int i = 1; i <= 7; i++)
		{
			explosion_sprites[i] = createSprite(("data/explosion/" + std::to_string(i) + ".png").c_str());
			getSpriteSize(explosion_sprites[i], width, height);
			setSpriteSize(explosion_sprites[i], width / 1.5, height / 1.5);
		}
		getSpriteSize(explosion_sprites[1], width, height);
	};

	void boom(double x, double y) {
		explosion_info.push_back(Boom(getTickCount(), x, y));
	}

	void draw() {
		for (auto& boom : explosion_info)
		{
			int temp = getTickCount() - boom.start_tick;
			if (temp >= 80 && temp <= 560)
			{

				drawSprite(explosion_sprites[temp / 80], boom.x - width / 2, boom.y - height / 2);
			}
		}
	}

private:

	std::map<int, Sprite*> explosion_sprites;
	std::vector<Boom> explosion_info;
};

class TileManager
{
public:
	TileManager(std::vector<std::vector <int>> map, int w) {

		for (int i = 0; i < map.size(); i++)
		{
			std::vector <Tile*> temp_t;
			for (int j = 0; j < map[i].size(); j++)
			{
				Tile* temp_tile = new Tile(map[i][j]);
				temp_tile->setSize(w / map[i].size(), (w / map[i].size()) / 3);
				temp_tile->setXY(j * temp_tile->getWidth() + temp_tile->getWidth() / 2, i * temp_tile->getHeight() + temp_tile->getHeight() / 2);
				temp_t.push_back(temp_tile);
			}
			tiles.push_back(temp_t);
		}
		explosion = new Explosion();
	};
	~TileManager() {};

	void drawAll() {
		for (auto& tile_line : tiles)
		{
			for (auto& tile : tile_line)
			{
				tile->draw();
			}
		}
		explosion->draw();
	}


	bool checkColission(std::unique_ptr<Platform>& platform, int& score) {
		for (int i = 0; i < tiles.size(); i++)
		{
			for (int j = 0; j < tiles[i].size(); j++)
			{
				if (platform->checkColission(tiles[i][j], score))
				{
					if (tiles[i][j]->getColor() == TileColor::orange)
					{
						if (i + 1 < tiles.size())
						{
							tiles[i + 1][j]->breakTile(score);
							if (j + 1 < tiles[i].size())
							{
								tiles[i + 1][j + 1]->breakTile(score);
							}
							if (j - 1 >= 0)
							{
								tiles[i + 1][j - 1]->breakTile(score);
							}
						}

						if (j + 1 < tiles[i].size())
						{
							tiles[i][j + 1]->breakTile(score);
						}

						if (i - 1 >= 0)
						{
							tiles[i - 1][j]->breakTile(score);
							if (j - 1 >= 0)
							{
								tiles[i - 1][j - 1]->breakTile(score);
							}
							if (j + 1 < tiles[i].size())
							{
								tiles[i - 1][j + 1]->breakTile(score);
							}
						}

						if (j - 1 >= 0)
						{
							tiles[i][j - 1]->breakTile(score);
						}

						explosion->boom(tiles[i][j]->getX(), tiles[i][j]->getY());

					}
					if ((score + 1) % 10 == 0)
					{
						tiles[i][j] = new Bonus(rand() % 5 + 1,
							tiles[i][j]->getWidth(), tiles[i][j]->getHeight(),
							tiles[i][j]->getX(), tiles[i][j]->getY());
					}
					return 1;
				}
			}
		}
		return 0;
	}

	bool checkVictory() {
		for (auto& tile_line : tiles)
		{
			for (auto& tile : tile_line)
			{
				if (!tile->isBroken())
				{
					return 0;
				}
			}
		}
		return 1;
	}

private:
	std::vector<std::vector <Tile*>> tiles;
	Explosion* explosion;

};