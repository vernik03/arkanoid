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

		const int BONUS_SPEED = 1;
		for (auto& bonus_tile : bonus_tiles)
		{
			if (canMoveBonus(bonus_tile)) {
				bonus_tile->setY(bonus_tile->getY() + BONUS_SPEED);
			}
			bonus_tile->draw();
		}
	}

	bool canMoveBonus(Bonus* bonus_tile) {
		for (auto& tile_vector : tiles) {
			for (auto& tile : tile_vector) {
				if (tile->isEnable()) {
					if (tile->getX() == bonus_tile->getX()) {
						double dist = tile->getY() - (bonus_tile->getY() + bonus_tile->getHeight());
						if (dist == 1) {
							return 0;
						}
					}
				}
			}
		}
		return 1;
	}

	bool checkCollision(std::unique_ptr<Platform>& platform, int& score) {
		for (int i = 0; i < tiles.size(); i++)
		{
			for (int j = 0; j < tiles[i].size(); j++)
			{
				if (platform->checkCollision(tiles[i][j], score))
				{
					if (tiles[i][j]->getColor() == TileColor::orange)
					{
						if (i + 1 < tiles.size())
						{
							if (!checkScore(tiles[i + 1][j], score, platform))
								tiles[i + 1][j]->breakTile(score);
							if (j + 1 < tiles[i].size())
							{
								if (!checkScore(tiles[i + 1][j + 1], score, platform))
									tiles[i + 1][j + 1]->breakTile(score);
							}
							if (j - 1 >= 0)
							{
								if (!checkScore(tiles[i + 1][j - 1], score, platform))
									tiles[i + 1][j - 1]->breakTile(score);
							}
						}

						if (j + 1 < tiles[i].size())
						{
							if (!checkScore(tiles[i][j + 1], score, platform))
								tiles[i][j + 1]->breakTile(score);
						}

						if (i - 1 >= 0)
						{
							if (!checkScore(tiles[i - 1][j], score, platform))
								tiles[i - 1][j]->breakTile(score);
							if (j - 1 >= 0)
							{
								if (!checkScore(tiles[i - 1][j - 1], score, platform))
									tiles[i - 1][j - 1]->breakTile(score);
							}
							if (j + 1 < tiles[i].size())
							{
								if (!checkScore(tiles[i - 1][j + 1], score, platform))
									tiles[i - 1][j + 1]->breakTile(score);
							}
						}

						if (j - 1 >= 0)
						{
							if (!checkScore(tiles[i][j - 1], score, platform))
								tiles[i][j - 1]->breakTile(score);
						}

						explosion->boom(tiles[i][j]->getX(), tiles[i][j]->getY());

					}
					else
					{
						checkScore(tiles[i][j], score, platform);
					}
					return 1;
				}
			}
		}

		for (auto& bonus_tile : bonus_tiles) {
			if (platform->checkBonusColission(bonus_tile, score))
			{
				bonus_tile->disable();
			}
		}
		return 0;
	}
	
	bool checkScore(Tile*& tile, int& score, std::unique_ptr<Platform>& platform) {
		if (score % 2 == 0 && score != 0 && tile->isEnable())
		{
			int i = 0;
			Abilities type;
			do {
				type = static_cast<Abilities>(rand() % 4);
				i++;

			} while (type == platform->getType() || type == platform->getBallType() || i < 10);
			
			Bonus* new_tile = new Bonus(type,
				tile->getWidth(), tile->getHeight(),
				tile->getX(), tile->getY());
			tile->breakTile(score);
			bonus_tiles.push_back(new_tile);
			return 1;
		}
		return 0;
	}

	bool checkVictory() {
		for (auto& tile_line : tiles)
		{
			for (auto& tile : tile_line)
			{
				if (!tile->isBroken() && dynamic_cast<Bonus*>(tile) == nullptr)
				{
					return 0;
				}
			}
		}
		return 1;
	}

	void deleteDroppers(int window_h) {
		std::vector<Bonus*> to_erase = {};
		for (auto bonus_tile : bonus_tiles) {
			if (bonus_tile->getY() >= window_h) {
				to_erase.push_back(bonus_tile);
			}
		}

		for (auto bonus_tile : to_erase) {
			bonus_tiles.erase(std::find(bonus_tiles.begin(), bonus_tiles.end(), bonus_tile));
			delete bonus_tile;
		}
	}

private:
	std::vector<std::vector <Tile*>> tiles;
	std::vector<Bonus*> bonus_tiles;
	Explosion* explosion;

};