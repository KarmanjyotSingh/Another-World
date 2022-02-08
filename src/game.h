#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "game_level.h"

const float SCR_WIDTH = 1200.0f;
const float SCR_HEIGHT = 800.0f;

// Represents the current state of the game
enum GameState
{
	GAME_START,
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_LOST
};

// Initial size of the player pikachu
const glm::vec2 PLAYER_SIZE(50.0f, 50.0f);
// Initial velocity of the player pikachu
const float PLAYER_VELOCITY(300.0f);
enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};
enum Light
{
	Andhera,
	Savera
};
typedef std::tuple<bool, Direction, glm::vec2> Collision;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
	// game state
	GameState State;
	bool Keys[1024];
	unsigned int Width;
	unsigned int Height;
	std::vector<GameLevel> Levels;
	unsigned int Level;
	int score = 0;
	Light light;
	time_t start;
	// constructor
	Game(unsigned int width, unsigned int height);
	// destructor
	~Game();
	// initialize game state (load all shaders/textures/levels)
	void Init();
	void handleLighting();
	// game loop
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
	void DoCollisions();
	Direction VectorDirection(glm ::vec2 target);
	bool checkEnemyCollisonHelperFunction(GameObject &enemy);
};

#endif
