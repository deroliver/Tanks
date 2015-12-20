#include "Cannon.h"

#include <glm/gtx/rotate_vector.hpp>
#include <gengine/ResourceManager.h>

#include "Tank.h"

#include <iostream>


Cannon::Cannon(std::string name, int tankStyle, int fireRate, int bulletsPerShot, float bulletSpeed, float bulletDamage, glm::vec2& tankPosition) :
	_name(name),
	_tankStyle(tankStyle),
	_fireRate(fireRate), 
	_bulletsPerShot(bulletsPerShot),
	_bulletSpeed(bulletSpeed),
	_bulletDamage(bulletDamage),
	_frameCounter(0),
	_position(tankPosition) { }



Cannon::~Cannon() {

}


void Cannon::init() {
	gengine::GLTexture texture = gengine::ResourceManager::getTexture("Textures/muzzle.png");

	_flareTileSheet.init(texture, glm::ivec2(23, 1));

	switch (_tankStyle) {
	case 1:
		_cannonTextureID = gengine::ResourceManager::getTexture("Textures/TankOne/barrel.png").id;

		_position.x = _position.x + 23;
		_position.y = _position.y + 21;

		_destRect.x = _position.x;
		_destRect.y = _position.y;
		_destRect.w = 82;
		_destRect.z = 82;
		break;

	case 2:
		_cannonTextureID = gengine::ResourceManager::getTexture("Textures/TankTwo/barrel.png").id;

		_position.x -= 20;
		_position.y -= 21;

		_destRect.x = _position.x;
		_destRect.y = _position.y;
		_destRect.w = 133;
		_destRect.z = 133;
		break;

	case 3:
		_cannonTextureID = gengine::ResourceManager::getTexture("Textures/TankThree/barrel.png").id;

		_position.x -= 10;
		_position.y -= 10;

		_destRect.x = _position.x;
		_destRect.y = _position.y;
		_destRect.w = 120;
		_destRect.z = 120;
		break;

	case 4:
		_cannonTextureID = gengine::ResourceManager::getTexture("Textures/TankFour/barrel.png").id;

		_position.x -= 5;
		_position.y -= 5;

		_destRect.x = _position.x;
		_destRect.y = _position.y;
		_destRect.w = 105;
		_destRect.z = 105;
		break;
	}
}


void Cannon::update(int currentTarget, const std::vector<Tank*>& tanks, std::vector<Bullet>& bullets, float deltaTime) {
	//glm::vec2 fireDir = tanks[currentTarget]->getPosition() - _position;
	//_rotation += (glm::acos(fireDir.x / fireDir.y) / 50.0f);

}

void Cannon::fire(const glm::vec2& direction, const glm::vec2& pos, std::vector<Bullet>& bullets) {
	for (int i = 0; i < _bulletsPerShot; i++) {	
		bullets.emplace_back(pos, direction, _bulletDamage, _bulletSpeed);
	}
}


void Cannon::draw(gengine::SpriteBatch& spriteBatch) {
	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	static int i = 0;

	gengine::ColorRGBA8 color;

	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;
	

	glm::vec4 flareDestRest;
	flareDestRest.x = _position.x + 42;
	flareDestRest.y = _position.y + 9;
	flareDestRest.w = 64;
	flareDestRest.z = 64;

	if (_frameCounter > 5) {
		if (i == 23) {
			i = 0;
		}
		i++;
		_frameCounter = 0;
	}
	else {
		_frameCounter++;
	}

	glm::vec2 halfDims(_destRect.z / 2.0f, _destRect.w / 2.0f);

	glm::vec4 turret(_destRect.z / 2.0f, _destRect.w / 2.0f, 0.0f, 0.0f);
	glm::vec4 flare(_destRect.z / 2.0f, _destRect.w / 2.0f, 42.0f, 9.0f);


	spriteBatch.draw(flareDestRest, _flareTileSheet.getUVs(i), _flareTileSheet.texture.id, 0.0f, color, 0.785398, flare);
	std::cout << "X : " << _destRect.x << "		Y: " << _destRect.y << std::endl;
	std::cout << "W : " << _destRect.w << "		Z: " << _destRect.z << std::endl;
	spriteBatch.draw(_destRect, uvRect, _cannonTextureID, 0.0f, color, 0.785398, turret);
}


glm::vec2 Cannon::rotatePoint(const glm::vec2& pos, float angle) {
	glm::vec2 newv;
	newv.x = pos.x * cos(angle) - pos.y * sin(angle);
	newv.y = pos.x * sin(angle) + pos.y * cos(angle);
	return newv;
}