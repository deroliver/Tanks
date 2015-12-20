#include "MainGame.h"

#include "Parser.h"

#include <gengine/gengine.h>
#include <gengine/Timing.h>

#include <SDL/SDL.h>
#include <iostream>
#include <time.h>
#include <algorithm>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

MainGame::MainGame() : _screenWidth(SCREEN_WIDTH), _screenHeight(SCREEN_HEIGHT), _gameState(GameState::PLAY), _fps(0) {
	srand((unsigned)time(NULL));

	Parser parser;
	parser.parseText("program.txt");
	parser.analyze(_tanks);
}


MainGame::~MainGame(){

}


void MainGame::run() {

	initSystems();
	//initTank(glm::vec2(500, 300), ATTACK);
	//initTank(glm::vec2(100, 100), ATTACK);
	//initTank(glm::vec2(100, 400), ATTACK);
	//initTank(glm::vec2(600, 200), ATTACK);
	initTank();
	gameLoop();

}


void MainGame::initSystems() {
	gengine::init();

	_window.create("Tanks", _screenWidth, _screenHeight, 0);
	glClearColor(0.6f, 0.8f, 1.0f, 1.0f);

	initShaders();

	_tankSpriteBatch.init();

	_camera.init(_screenWidth, _screenHeight);

	_arena = new Arena();
}


void MainGame::initShaders() {
	// Compile the color shader
	_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}


void MainGame::initTank() {
	//Tank *newTank = new Tank();

	std::cout << _tanks.size() << std::endl;

	for (int i = 0; i < _tanks.size(); i++) {
		_tanks[i]->init(&_bullets);
	}

	//newTank->init(pos, beh, &_bullets);

	//_tanks.push_back(newTank);
}


void MainGame::gameLoop() {
	const float DESIRED_FPS = 60.0f;
	const int MAX_PHYSICS_STEPS = 6;

	gengine::FpsLimiter fpsLimiter;
	fpsLimiter.setMaxFPS(60.0f);

	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float previousTicks = SDL_GetTicks();

	while (_gameState == GameState::PLAY) {
		fpsLimiter.beginFrame();

		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks;
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAMETIME;

		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) {
			float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
			updateTanks(deltaTime);
			updateBullets(deltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}

		_camera.setPosition(glm::vec2(_screenWidth / 2, _screenHeight / 2));
		_camera.update();
		

		drawGame();

		_fps = fpsLimiter.end();
	}
}


void MainGame::updateTanks(float deltaTime) {
	// Update all tanks
	for (int i = 0; i < _tanks.size(); i++) {
		_tanks[i]->update(_tanks, deltaTime);
	}
}


void MainGame::updateBullets(float deltaTime) {
	for (int i = 0; i < _bullets.size();) {
		if (_bullets[i].update(SCREEN_WIDTH, SCREEN_HEIGHT, deltaTime)) {
			_bullets[i] = _bullets.back();
			_bullets.pop_back();
		}
		else {
			i++;
		}
	}

	bool wasBulletRemoved;

	for (int i = 0; i < _bullets.size(); i++) {
		wasBulletRemoved = false;
		for (int j = 0; j < _tanks.size();) {
			if (_bullets[i].collideWithTank(_tanks[j])) {
				if (_tanks[j]->applyDamage(_bullets[i].getDamage())) {
					_tanks[j]->explode(_tankSpriteBatch);
				}
				else {
					j++;
				}
				_bullets[i] = _bullets.back();
				_bullets.pop_back();
				wasBulletRemoved = true;
				i--;
				break;
			}
			else {
				j++;
			}
		}
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			// Exit the game here!
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}


void MainGame::drawGame() {
	// Set the base depth to 1.0
	glClearDepth(1.0);
	// Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	_textureProgram.use();

	// Make sure the shader uses texture 0
	glActiveTexture(GL_TEXTURE0);

	// Make sure the shader uses texture 0
	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	// Grab the camera matrix
	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint pUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	// Draw the level
	_arena->draw();

	// Begin drawing agents
	_tankSpriteBatch.begin();

	// Draw the humans
	for (int i = 0; i < _tanks.size(); i++) {
		_tanks[i]->draw(_tankSpriteBatch);
	}

	for (int i = 0; i < _bullets.size(); i++) {
		_bullets[i].draw(_tankSpriteBatch);
	}

	_tankSpriteBatch.end();

	_tankSpriteBatch.renderBatch();

	_textureProgram.unuse();

	// Swap our buffer and draw everything to the screen!
	_window.swapBuffer();
}