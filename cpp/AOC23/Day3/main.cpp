#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <ranges>
#include <ExecutionTime.h>

typedef struct vec2i_s {
	int x, y;

	bool operator==(const vec2i_s& other) const {
		return (x == other.x && y == other.y);
	}
} vec2i;

class PartNumber {
public:
	int value;
	std::vector<vec2i> range;

	PartNumber() {
		value = 0;
	}


	void imply_lateral_range(int y, int x, int length)
	{
		range.clear();
		range.resize(length);

		int iter = 0;
		for (int i = x; i < x + length; ++i) {
			range[iter] = { i, y };
			iter++;
		}
	}


};

class Part {
public:
	char part_ref;
	vec2i point;

	Part(char _ref, vec2i _point) {
		part_ref = _ref;
		point = _point;
	}

	std::vector<vec2i> get_range() {
		return std::vector<vec2i> {
			{ point.x - 1, point.y     }, // LEFT
			{ point.x - 1, point.y - 1 }, // TOP LEFT
			{ point.x    , point.y - 1 }, // TOP
			{ point.x + 1, point.y - 1 }, // TOP RIGHT 
			{ point.x + 1, point.y },     // RIGHT 
			{ point.x + 1, point.y + 1 }, // BOTTOM RIGHT 
			{ point.x    , point.y + 1 }, // BOTTOM
			{ point.x - 1, point.y + 1 }  // BOTTOM LEFT
		};
	}

	bool collides(PartNumber number)
	{
		auto bbox = get_range();
		for (int i = 0; i < number.range.size(); ++i) {
			for (int b = 0; b < bbox.size(); ++b) {
				if (number.range[i] == bbox[b]) {
					std::cout << "number(" << number.range[i].x << "," << number.range[i].y << ") == bbox(" << bbox[b].x << "," << bbox[b].y << ")" << std::endl;
					return true;
				}
			}
		}
		return false;
	}
};

static inline std::vector<Part> parts = std::vector<Part>();
static inline std::vector<PartNumber> part_numbers = std::vector<PartNumber>();

std::unordered_map<std::string, char> symbol_map = {
	{"star",	'*'},
	{"dollar",	'$' },
	{"amp",		'&' },
	{"slash",	'/' },
	{"at",		'@' },
	{"perc",	'%' },
	{"eq",		'=' },
	{"add",		'+' },
	{"sub",		'-' },
	{"hash",	'#' }
};


int combineToInt(const std::vector<int>& numbers) {
	std::stringstream ss;

	// Append all integers from the vector to stringstream
	for (int num : numbers) {
		ss << num;
	}

	// Convert stringstream to string and then parse to integer
	int combinedNumber;
	ss >> combinedNumber;

	return combinedNumber;
}

void line_parser(std::string line, int line_num)
{
	std::cout << line << std::endl;

	auto skv = std::views::values(symbol_map);
	std::vector<char> symbols = { skv.begin(), skv.end() };

	for (int i = 0; i < line.length(); ++i) {
		char ch = line[i];
		
		{
			// discover symbols
			auto sfind = std::find(symbols.begin(), symbols.end(), ch);
			if (sfind != symbols.end()) {
				//std::cout << "Found the " << ch << std::endl;

				parts.push_back({ ch, {i, line_num} });

				continue;
			}
		}

		{
			// discover # - #*n integers, those are part numbers (probably)
			int ichar = 0;
			//std::vector<int> parsed_number(3);
			char parsed_number[]{ '\0', '\0', '\0', '\0'};
			PartNumber part_num;
			for (;;) {
				// we need to find out if we've hit the edge as well, watch out for some redundant code here
				if ((i + ichar) > line.length()) {
					// apply number
					part_num.value = atoi(parsed_number);
					part_num.imply_lateral_range(line_num, i, ichar);
					part_numbers.push_back(part_num);

					break;
				}

				char char_char = line[i + ichar];
				int int_char = char_char - '0';
				if (int_char < 0 || int_char > 9) { // not an int we're interested in
					if (ichar > 0) {
						part_num.value = atoi(parsed_number);
						part_num.imply_lateral_range(line_num, i, ichar);
						part_numbers.push_back(part_num);

						// reset ichar and jump this much
						i += (ichar - 1);
						ichar = 0;
					}


					break;
				}

				// apply number
				parsed_number[ichar] = char_char;
				ichar++;
			}
		}
		
		// 
	}
}

void read_file(std::string filename, void (*parser)(std::string line, int line_num))
{
	std::cout << "Loading input.." << std::endl;

	std::ifstream file(filename);

	// Phase 1: Read the data from file
	int line_num = 0;
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line)) {
			parser(line, line_num);
			line_num++;
		}
	}

	file.close();
}

void build_cache() 
{

	read_file("../../../_puzzle_input/day3/input_sample.txt", line_parser);
}

int main()
{
	ExecutionTimer<std::chrono::milliseconds> timer;

	std::cout << "Advent of Code 2023, Day 3: Gear Ratios." << std::endl;
	std::cout << "========================================" << std::endl;

	build_cache();

	int score = 0;
	for (int n = 0; n < part_numbers.size(); ++n) {
		PartNumber number = part_numbers[n];

		std::cout << "Part Number " << number.value << " with range " ;
		for (int r = 0; r < number.range.size(); ++r) {
			std::cout << "(" << number.range[r].x << ", " << number.range[r].y << "), ";
		}
		std::cout << std::endl;


		for (int p = 0; p < parts.size(); ++p) {
			Part part = parts[p];


			if (part.collides(number)) {
				std::cout << "Collision of " << number.value << " with part " << part.part_ref << " at " << part.point.x << "," << part.point.y << " (" << part.point.x +1 << "," << part.point.y + 1 << ")" << std::endl;
				score += number.value;
				break;
			}
		}
	}

	std::cout << "Part 1 Score: " << score << std::endl;

}