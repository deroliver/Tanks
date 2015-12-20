#pragma once

#include <gengine/SpriteBatch.h>
#include <gengine/TileSheet.h>

#include <glm/glm.hpp>

#include<vector>
#include <string>


class Tank;

const int BULLET_RADIUS = 15;

class Bullet {

public:
	Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed);
	~Bullet();

	// Update returns true, delete bullet
	bool update(const float screenWidth, const float screenHeight, float deltatime);

	void draw(gengine::SpriteBatch& spriteBatch);

	bool collideWithTank(Tank* tank);

	float getDamage() { return _damage; }

private:

	bool collideWithWorld(const float screenWidth, const float screenHeight);

	float _damage;
	glm::vec2 _position;
	glm::vec2 _direction;
	float _speed;
	float _angle;

};

