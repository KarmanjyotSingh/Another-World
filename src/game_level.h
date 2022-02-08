#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <vector>

#include "game_object.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

/// GameLevel holds all Tiles as part of a AnotherWorld level and
/// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
	// level state
	// walls and obstructions
	std::vector<GameObject> walls;
	std ::vector<GameObject> coins;
	std ::vector<GameObject> enemy;
	glm ::vec2 exit;
	glm ::vec2 start;
	

	// constructor
	GameLevel() {}
	// loads level from file
	void Load(const char *file, unsigned int levelWidth,
			  unsigned int levelHeight);
	// render level
	void Draw(SpriteRenderer &renderer);
	// check if the level is completed (all non-solid tiles are destroyed)
	bool IsCompleted();

private:
	// initialize level from tile data
	void init(std::vector<std::vector<unsigned int>> tileData,
			  unsigned int levelWidth, unsigned int levelHeight);
};

#endif
