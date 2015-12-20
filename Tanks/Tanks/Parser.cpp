#include "Parser.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>  

Parser::Parser() {

}


Parser::~Parser() {

}


void Parser::parseText(std::string filePath) {
	std::ifstream input(filePath);

	for (std::string line; getline(input, line);) {
		_lines.push_back(line);
	}
}


void Parser::analyze(std::vector<Tank*>& tanks) {
	std::string tank = "tank";
	std::string newtank = "new";
	std::string position = "position";
	std::string behavior = "behavior";

	std::vector<TankObject> tempObject;

	TankObject temp;

	std::string tankName;

	std::string firstPos, secondPos;
	
	Behavior beh;

	for (int i = 0; i < _lines.size(); i++) {
		if (_lines[i].find(newtank) != std::string::npos) {
			tankName = _lines[i].substr(9, 1);
			temp.name = tankName;
			tempObject.push_back(temp);
		}
		else if (_lines[i].find(position) != std::string::npos) {
			firstPos = _lines[i].substr(13, 3);
			secondPos = _lines[i].substr(18, 3);

			int f = atoi(firstPos.c_str());
			int s = atoi(secondPos.c_str());

			std::string name = _lines[i].substr(0, 1);
			for (int i = 0; i < tempObject.size(); i++) {
				if (tempObject[i].name == name) {
					tempObject[i].position = glm::vec2(f, s);
				}
			}
		}
		else if (_lines[i].find(behavior) != std::string::npos) {
			std::string behavior = _lines[i].substr(13, _lines[i].length() - 14);
			
			std::string name = _lines[i].substr(0, 1);


			if (behavior == "ATTACK") {
				beh = Behavior::ATTACK;
			}
			else if (behavior == "KAMIKAZE") {
				beh = Behavior::KAMIKAZE;
			}
			else if (behavior == "WIN") {
				beh = Behavior::WIN;
			}
			else if (behavior == "SITDIE") {
				beh = Behavior::SITDIE;
			}

			for (int i = 0; i < tempObject.size(); i++) {
				if (tempObject[i].name == name) {
					tempObject[i].beh = beh;
				}
			}
		}
	}

	Tank* newTank;

	for (int i = 0; i < tempObject.size(); i++) {
		std::cout << tempObject[i].position.x << tempObject[i].position.y << std::endl;
		newTank = new Tank(tempObject[i].name, tempObject[i].position, tempObject[i].beh);
		tanks.push_back(newTank);
	}

	std::cout << tanks.size() << std::endl;
}
