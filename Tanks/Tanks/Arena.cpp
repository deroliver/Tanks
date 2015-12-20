#include "Arena.h"

#include <iostream>

#include <gengine/Errors.h>
#include <gengine/ResourceManager.h>

Arena::Arena() {
	_spriteBatch.init();
	_spriteBatch.begin();

	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	glm::vec4 destRect(0, 0, 1024, 768);
	gengine::ColorRGBA8 whiteColor;

	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;

	_spriteBatch.draw(destRect, uvRect, gengine::ResourceManager::getTexture("Textures/arena.png").id, 0.0f, whiteColor);

	_spriteBatch.end();
}


Arena::~Arena() {

}

void Arena::draw() {
	_spriteBatch.renderBatch();
}
