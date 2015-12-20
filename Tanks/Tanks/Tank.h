#pragma once

#include <gengine/SpriteBatch.h>
#include <gengine/TileSheet.h>

#include "Bullet.h"
#include "Cannon.h"

#include <glm/glm.hpp>

#include <math.h>

enum Behavior {ATTACK, KAMIKAZE, WIN, SITDIE};

const double PI = 3.141592653589793238463;
const double TANKWIDTH = 50;

class Tank {
public:
	Tank(std::string name, glm::vec2 pos, Behavior beh);
	~Tank();

	void init(std::vector<Bullet>* bullets);

	void update(const std::vector<Tank*>& tanks, float deltaTime);

	void draw(gengine::SpriteBatch& spriteBatch);

	glm::vec2 getPosition() const { return glm::vec2(_position.x + _hitBox.w / 2, _position.y + _hitBox.z / 2); }

	void fireCannon(glm::vec4& muzzleDest, gengine::SpriteBatch& spriteBatch);

	void scan(const std::vector<Tank*>& tanks);

	glm::vec4 getHitBox() { return _hitBox; }

	void rotateTurret();

	void acquireTarget(const std::vector<Tank*>& tanks);

	void explode(gengine::SpriteBatch& spriteBatch);

	void attack(std::vector<Tank*>& tanks);

	bool applyDamage(float damage);

private:

	glm::vec2 _barrelLocation;

	Cannon* _cannon;

	/// The tank's position
	glm::vec2 _position;

	/// The tank's direction
	glm::vec2 _direction;

	/// The tank's barrel direction
	glm::vec2 _barrelDirection;

	/// The tank's hitbox
	glm::vec4 _hitBox;
	
	/// The tank's color
	gengine::ColorRGBA8 _color;

	/// Angle between 0 and 360
	float _bodyAngle;

	/// Angle between 0 and 360
	float _barrelAngle;

	/// The tanks behavior
	Behavior _behavior;

	/// The style of the tank
	int _tankStyle;

	/// The body texture ID
	int _bodyTextureID;

	/// The barrel texture ID
	int _barrelTextureID;

	/// Speed of the tank
	float _speed;

	/// Found a target?
	bool foundTarget;

	/// Angle between turret and the target
	float _targetAngle;

	/// Current target
	int _currentTarget;

	/// Explosion tilesheet
	gengine::TileSheet _explosionTexture;

	/// Health of the tank
	int _health;

	/// Vector of bullets
	std::vector<Bullet>* _bullets;


	int _frameCounter;


	bool _hasTarget;

	std::string _name;
};


