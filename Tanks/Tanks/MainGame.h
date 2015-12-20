#pragma once

#include <gengine/Window.h>
#include <gengine/GLSLProgram.h>
#include <gengine/Camera2D.h>
#include <gengine/InputManager.h>
#include <gengine/SpriteBatch.h>

#include "Tank.h"
#include "Arena.h"

enum class GameState {
	PLAY, EXIT
};

class MainGame {
public:
	MainGame();
	~MainGame();

	/// Runs the game
	void run();

private:
	/// Initializes the core systems
	void initSystems();

	/// Initializes the level and sets everything up
	void initLevel();

	/// Initializes the shaders
	void initShaders();

	/// Adds a tank to the arena
	void initTank();

	/// Main game loop for the program
	void gameLoop();

	/// Updates all tanks
	void updateTanks(float deltaTime);

	/// Update all bullets
	void updateBullets(float deltaTime);

	/// Handles input processing
	void processInput();

	/// Renders the game
	void drawGame();


	/// Member Variables
	gengine::Window _window; ///< The game window

	gengine::GLSLProgram _textureProgram; ///< The shader program

	gengine::InputManager _inputManager; ///< Handles input

	gengine::Camera2D _camera; ///< Main Camera

	gengine::SpriteBatch _tankSpriteBatch; ///< Draws all tanks

	int _screenWidth, _screenHeight; ///< Screen dimensions

	float _fps; ///< Frames per second

	std::vector<Tank*> _tanks; ///< Vector of all the tanks in the game

	GameState _gameState; ///< The current game state

	Arena* _arena; ///< The arena object

	std::vector<Bullet> _bullets;
};

