#pragma once

#include <string>

#include <gengine/SpriteBatch.h>

class Arena {
public:
	Arena();
	~Arena();

	void draw();

	// Getters

private:
	gengine::SpriteBatch _spriteBatch;

};

