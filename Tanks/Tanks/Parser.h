#pragma once

#include "Tank.h"

#include <string>
#include <vector>

struct TankObject {
	std::string name;
	glm::vec2 position;
	Behavior beh;
};

class Parser {
public:
	Parser();
	~Parser();

	void parseText(std::string filePath);

	void analyze(std::vector<Tank*>& tanks);

private:

	std::vector<std::string> _lines;
};

