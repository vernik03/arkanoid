#pragma once
#include <string>
#include <algorithm>

#include "Framework.h"

enum class Color {
	white
};

enum class Align {
	center,
	left
};

enum class VAlign {
	top,
	center,
	bottom
};

enum class Transform {
	none,
	uppercase,
	lowercase
};

enum class Decoration { //not added
	none, 
	underline,
	overline,
	line_through
};

enum class Size {
	small,
	medium
};

struct Symbol {
	Sprite* symbol;
	int height = 27;
	int width = 18;
};

class Text
{
public:
	Text() {
		InitAlphabet(alphabet_white_25, Size::small, Color::white);
		InitAlphabet(alphabet_white_72, Size::medium, Color::white);
		
	}
	~Text() {}
	
	void print( std::string text,
				int x, int y,
				Size font_size = Size::small,
				Align align = Align::left,
				VAlign valign = VAlign::top,
				Transform text_transform = Transform::none,
				Color color = Color::white,
				int line_height = 27,
				int letter_spacing = 0,
				Decoration text_decoration = Decoration::none);
	
	void println(	std::string text,
					int x, int y,
					Size font_size,
					Color color,
					int letter_spacing,
					int line_height,
					Align align,
					VAlign valign);


	template <typename T>
	void printlnTemplate(
		std::string& text,
		int x, int y,
		int letter_spacing,
		int line_height,
		T& sprites,
		Align align,
		VAlign valign);

	template <typename T>
	void InitAlphabet(
		T& sprites, Size font_size, Color color);

private:
	std::map<char, Symbol> alphabet_white_25;
	std::map<char, Symbol> alphabet_white_72;
};

void Text::print(	std::string text,
					int x, int y,
					Size font_size,
					Align align,
					VAlign valign,
					Transform text_transform,
					Color color,
					int line_height,
					int letter_spacing,
					Decoration text_decoration) {
	

	std::string temp = "";
	int i = 0;
	for (char ch : text) {
		if (ch == '\n') {
			switch (text_transform)
			{
			case Transform::uppercase:
				std::transform(text.begin(), text.end(), text.begin(), ::toupper);
				break;
			case Transform::lowercase:
				std::transform(text.begin(), text.end(), text.begin(), ::tolower);
				break;
			default:
				break;
			}
			println(temp, x, y + (line_height * i), font_size, color, letter_spacing, line_height, align, valign);
			i++;
		}
		else {
			temp += ch;
		}
	}
	if (text[text.size() - 1] != '\n')
	{
		println(temp, x, y + (line_height * i), font_size, color, letter_spacing, line_height, align, valign);
	}
	
}

void Text::println(	std::string text,
					int x, int y,
					Size font_size,
					Color color,
					int letter_spacing,
					int line_height,
					Align align,
					VAlign valign) {
	if (color == Color::white)
	{
		if (font_size == Size::small)
		{
			printlnTemplate(text, x, y, letter_spacing, line_height, alphabet_white_25, align, valign);
		}
		else if (font_size == Size::medium)
		{
			printlnTemplate(text, x, y, letter_spacing, line_height, alphabet_white_72, align, valign);
		}
	}
}

template <typename T>
void Text::printlnTemplate(
	std::string& text,
	int x, int y,
	int letter_spacing,
	int line_height,
	T& sprites,
	Align align,
	VAlign valign) {

	if (align == Align::center)
	{
		int temp_x = 0;
		for (char ch : text)
		{
			temp_x += sprites[ch].width + letter_spacing;
		}
		temp_x /= 2;
		x -= temp_x;
	}
	if (valign == VAlign::center)
	{
		if (line_height >= sprites[text[0]].height)
		{
			y -= line_height / 2;
		}
		else
		{
			y -= sprites[text[0]].height / 2;
		}
	}
	else if (valign == VAlign::bottom)
	{
		if (line_height >= sprites[text[0]].height)
		{
			y -= line_height;
		}
		else
		{
			y -= sprites[text[0]].height;
		}
	}
	for (char& ch : text) {
		if (sprites.find(ch) != sprites.end()) {
			drawSprite(sprites[ch].symbol, x, y);
			x += sprites[ch].width + letter_spacing;
		}
	}
}


template <typename T>
void Text::InitAlphabet(
	T& sprites, Size font_size, Color color) {
	double factor;
	int real_size;
	if (font_size == Size::small)
	{
		factor = 1;
		real_size = 25;
	}
	else if (font_size == Size::medium)
	{
		factor = 2.777777777777778;
		real_size = 72;
	}

	for (int i = 32; i <= 126; i++)
	{		
		if (color == Color::white)
		{
			sprites[i].symbol = createSprite(("data/text/white/" + std::to_string(real_size) + "/" + std::to_string(i) + ".png").c_str());
		}

		if (font_size == Size::small)
		{	
			sprites[i].height = 27;
		}
		else if (font_size == Size::medium)
		{
			sprites[i].height = 74;
		}

		
		if (i == 32 || i == 43 || i == 52 || i == 54 || i == 97 || i == 16 || i == 110 ||
			i == 65 || i == 70 || i == 75 || i == 76 || i == 83 || i == 95 || i == 112)
		{
			sprites[i].width = int(16 * factor);
		}
		else if (i == 33 || i == 39 || i == 44 || i == 46 || i == 105 || i == 108 || i == 124)
		{
			sprites[i].width = int(5 * factor);
		}
		else if (i == 34 || i == 36 || i == 47 || i == 48 || i == 50 || i == 92 || i == 100 || i == 102 ||
			i == 51 || i == 55 || i == 56 || i == 57 || i == 61 || i == 63 || i == 94 || i == 101 ||
			i == 103 || i == 104 || i == 107 || i == 111 || i == 113 || i == 117 || i == 121 || i == 122)
		{
			sprites[i].width = int(15 * factor);
		}
		else if (i == 35 || i == 38 || i == 53 || i == 66 || i == 84 || i == 85 || i == 86 || i == 87 ||
			i == 68 || i == 69 || i == 72 || i == 77 || i == 78 || i == 80 || i == 88 || i == 89 ||
			i == 90 || i == 98 || i == 118 || i == 120 || i == 126)
		{
			sprites[i].width = int(18 * factor);
		}
		else if (i == 37)
		{
			sprites[i].width = int(22 * factor);
		}
		else if (i == 40 || i == 41 || i == 42 || i == 49 || i == 60 || i == 62)
		{
			sprites[i].width = int(11 * factor);
		}
		else if (i == 45 || i == 73 || i == 116 || i == 123 || i == 125)
		{
			sprites[i].width = int(10 * factor);
		}
		else if (i == 58 || i == 59)
		{
			sprites[i].width = int(7 * factor);
		}
		else if (i == 64 || i == 109)
		{
			sprites[i].width = int(24 * factor);
		}
		else if (i == 67 || i == 71 || i == 119)
		{
			sprites[i].width = int(19 * factor);
		}
		else if (i == 74)
		{
			sprites[i].width = int(13 * factor);
		}
		else if (i == 79 || i == 81 || i == 82)
		{
			sprites[i].width = int(21 * factor);
		}
		else if (i == 91 || i == 93 || i == 96)
		{
			sprites[i].width = int(8 * factor);
		}
		else if (i == 106 || i == 114 || i == 115)
		{
			sprites[i].width = int(13 * factor);
		}
	}
	
}