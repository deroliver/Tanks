#pragma once

#include <glm/glm.hpp>
#include <gengine/SpriteBatch.h>
#include <gengine/TileSheet.h>

#include "Bullet.h"

#include <string>
#include <vector>


class Cannon {
public:
	Cannon(std::string name, int fireRate, int tankStyle, int bulletsPerShot, float bulletSpeed, float bulletDamage, glm::vec2& tankPosition);
	~Cannon();

	void init();

	void update(int currentTarget, const std::vector<Tank*>& tanks, std::vector<Bullet>& bullets, float deltaTime);

	void draw(gengine::SpriteBatch& spriteBatch);

	glm::vec2 rotatePoint(const glm::vec2& pos, float angle);

private:
	void fire(const glm::vec2& direction, const glm::vec2& pos, std::vector<Bullet>& bullets);

	std::string _name;
	
	int _tankStyle;

	int _fireRate; ///< Fire rate in terms of frames

	int _bulletsPerShot; ///< How many bullets fired at a time

	float _bulletSpeed;

	float _bulletDamage;

	float _frameCounter;

	glm::vec2 _position;

	glm::vec4 _destRect;

	gengine::TileSheet _flareTileSheet;

	int _cannonTextureID;

	float _rotation;
};

