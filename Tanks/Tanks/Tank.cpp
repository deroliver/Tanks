#include "Tank.h"

#include <random>
#include <iostream>
#include <ctime>


#include <gengine/ResourceManager.h>
#include <glm/gtx/vector_angle.hpp>

const int ROCKET_DAMAGE = 20;

Tank::Tank(std::string name, glm::vec2 pos, Behavior beh) : _tankStyle(0), _targetAngle(-1.0f),_currentTarget(-1), _health(100), _frameCounter(0), _hasTarget(false), _position(pos), _name(name), _behavior(beh) {

}


Tank::~Tank(){

}


void Tank::init(std::vector<Bullet>* bullets) {
	_speed = .5f;

	_bullets = bullets;

	_color.a = 255;
	_color.b = 255;
	_color.r = 255;
	_color.g = 255;

	double angleRand = ((double)rand() / (double)RAND_MAX);
	
	_bodyAngle = 0;// PI * angleRand;

	_barrelAngle = PI / 2;// PI * angleRand;

	static std::mt19937 randomEngine(time(nullptr));
	std::uniform_int_distribution<int> randStyle(1, 4);

	_tankStyle = 1;// randStyle(randomEngine);

	_cannon = new Cannon("Cannon", 1, 10, 1, 50.0f, ROCKET_DAMAGE, _position);

	switch (_tankStyle) {
	case 1:
		_bodyTextureID = gengine::ResourceManager::getTexture("Textures/TankOne/body.png").id;

		_hitBox.x = _position.x;
		_hitBox.y = _position.y;
		_hitBox.w = _position.x + 113;
		_hitBox.z = _position.y + 113;
		break;

	case 2:
		_bodyTextureID = gengine::ResourceManager::getTexture("Textures/TankTwo/body.png").id;

		_hitBox.x = _position.x;
		_hitBox.y = _position.y;
		_hitBox.w = _position.x + 95;
		_hitBox.z = _position.y + 95;
		break;

	case 3:
		_bodyTextureID = gengine::ResourceManager::getTexture("Textures/TankThree/body.png").id;

		_hitBox.x = _position.x;
		_hitBox.y = _position.y;
		_hitBox.w = _position.x + 102;
		_hitBox.z = _position.y + 102;
		break;

	case 4:
		_bodyTextureID = gengine::ResourceManager::getTexture("Textures/TankFour/body.png").id;

		_hitBox.x = _position.x;
		_hitBox.y = _position.y;
		_hitBox.w = _position.x + 96;
		_hitBox.z = _position.y + 96;
		break;
	}

	_cannon->init();
}


void Tank::update(const std::vector<Tank*>& tanks, float deltaTime) {

	if (_barrelAngle > 6.28318531) {
		_barrelAngle = 0.0f;
	}

	_direction.x = cos(_bodyAngle);
	_direction.y = sin(_bodyAngle);

	_barrelDirection.x = cos(_barrelAngle);
	_barrelDirection.y = sin(_barrelAngle);

	scan(tanks);

	switch (_behavior) {
		case ATTACK:
			_cannon->update(1, tanks, *_bullets, deltaTime);
			
			break;

		case WIN:
			
			break;

		case KAMIKAZE:

			break;

		case SITDIE:

			break;
	}
}


void Tank::draw(gengine::SpriteBatch& spriteBatch) {
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 bodyDestRect;
	glm::vec4 barrelDestRect;
	glm::vec4 explosionDestRect;

	explosionDestRect.x = _position.x;
	explosionDestRect.y = _position.y;

	explosionDestRect.z = 127;
	explosionDestRect.w = 127;

	switch (_tankStyle) {
	case 1: 
		bodyDestRect.x = _position.x;
		bodyDestRect.y = _position.y;
		bodyDestRect.z = 127;
		bodyDestRect.w = 127;

		barrelDestRect.x = _position.x + 23;
		barrelDestRect.y = _position.y + 21;
		barrelDestRect.z = 82;
		barrelDestRect.w = 82;

		break;

	case 2:
		bodyDestRect.x = _position.x;
		bodyDestRect.y = _position.y;
		bodyDestRect.z = 95;
		bodyDestRect.w = 95;

		barrelDestRect.x = _position.x - 20;
		barrelDestRect.y = _position.y - 21;
		barrelDestRect.z = 133;
		barrelDestRect.w = 133;

		break;

	case 3:
		bodyDestRect.x = _position.x;
		bodyDestRect.y = _position.y;
		bodyDestRect.z = 102;
		bodyDestRect.w = 102;

		barrelDestRect.x = _position.x - 10;
		barrelDestRect.y = _position.y - 10;
		barrelDestRect.z = 120;
		barrelDestRect.w = 120;

		break;

	case 4:
		bodyDestRect.x = _position.x;
		bodyDestRect.y = _position.y;
		bodyDestRect.z = 96;
		bodyDestRect.w = 96;

		barrelDestRect.x = _position.x - 5;
		barrelDestRect.y = _position.y - 5;
		barrelDestRect.z = 105;
		barrelDestRect.w = 105;

		break;
	}

	glm::vec4 halfDims(bodyDestRect.z / 2.0f, bodyDestRect.w / 2.0f, 0.0f, 0.0f);

	_cannon->draw(spriteBatch);
	spriteBatch.draw(bodyDestRect, uvRect, _bodyTextureID, 1, _color, _bodyAngle, halfDims);

}


void Tank::scan(const std::vector<Tank*>& tanks) {
	if (_currentTarget == -1) {
		acquireTarget(tanks);
	}

	glm::vec2 targetTank = tanks[_currentTarget]->getPosition() - _position;

	targetTank = glm::normalize(targetTank);
	_barrelDirection = glm::normalize(_barrelDirection);

	_targetAngle = glm::angle(targetTank, _barrelDirection);
	rotateTurret();	
}


void Tank::rotateTurret() {
	_barrelAngle += _targetAngle / 75.0f;
}


void Tank::acquireTarget(const std::vector<Tank*>& tanks) {
	int numTanks = tanks.size();
	if (_currentTarget == -1) {
		static std::mt19937 randomEngine(time(nullptr));
		std::uniform_int_distribution<int> randTarget(0, numTanks - 1);

		_currentTarget = randTarget(randomEngine);
	}

	if (tanks[_currentTarget] == this) {
		if (_currentTarget == numTanks - 1) {
			_currentTarget--;
		}
		else {
			_currentTarget++;
		}
	}
	std::cout << _currentTarget << std::endl;
}


void Tank::explode(gengine::SpriteBatch& spriteBatch) {
	static int lastFrame = 0;

	static int i = 0;
	if (i == 21)
		i = 0;

	if (lastFrame > 10) {
		lastFrame = 0;
		i++;
	}
	else {
		lastFrame++;
	}


	//spriteBatch.draw(_hitBox, _explosionTexture.getUVs(i), _explosionTexture.texture.id, 0, _color, 0.0f);
}

bool Tank::applyDamage(float damage) {
	_health -= damage;

	if (_health <= 0) {
		return true;
	}

	return false;
}