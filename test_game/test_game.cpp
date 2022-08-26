
#include <random>
#include <iostream>
#include <vector>
#include <iterator>
#include <string>
#include <iterator>
#include <regex>
#include <sstream>
#include "Framework.h"

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

		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {
		drawTestBackground();
		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {

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
