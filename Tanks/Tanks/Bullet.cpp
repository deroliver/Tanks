#include "Bullet.h"

#include <gengine/ResourceManager.h>
#include <gengine/GLTexture.h>

#include "Tank.h"



Bullet::Bullet(glm::vec2 pos, glm::vec2 dir, float damage, float speed) : _position(pos), _direction(dir), _damage(damage), _speed(speed) {

}


Bullet::~Bullet() {

}


bool Bullet::update(const float screenWidth, const float screenHeight, float deltatime) {

	_position += _direction * _speed * deltatime;

	return collideWithWorld(screenWidth, screenHeight);
}


void Bullet::draw(gengine::SpriteBatch& spriteBatch) {
	glm::vec4 destRect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);

	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

	gengine::ColorRGBA8 color;

	color.r = 255;
	color.g = 0;
	color.b = 0;
	color.a = 255;

	spriteBatch.draw(destRect, uvRect, gengine::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, color);
}



bool Bullet::collideWithTank(Tank* tank) {
	return true;
}




bool Bullet::collideWithWorld(const float screenWidth, const float screenHeight) {
	return (_position.x > screenWidth || _position.x < 0 || _position.y > screenHeight || _position.y < 0);
}