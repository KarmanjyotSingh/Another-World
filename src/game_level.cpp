#include "game_level.h"
#include "game.h"
#include <fstream>
#include <iostream>
#include <sstream>

#include "game_object.h"

void GameLevel::Load(const char *file, unsigned int levelWidth,
                     unsigned int levelHeight)
{
    // clear old data
    this->coins.clear();
    this->walls.clear();
    this->enemy.clear();
    // load from file
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->walls)
    {
        if (!tile.Enemy && !tile.Destroyed)
        {
            tile.Draw(renderer);
        }
        if (tile.Enemy)
            tile.Draw(renderer);
    }
}

bool GameLevel::IsCompleted()
{
    for (GameObject &coin : this->coins)
        if (!coin.Destroyed) // in if( tileData[y][x]== t)he coin is not yet
                             // picked up
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData,
                     unsigned int levelWidth, unsigned int levelHeight)
{
    int itr = 0;
    srand(time(0));
    // get the height of the level frame
    int m = tileData.size();
    // get the width of the level frame
    unsigned int n = tileData[0].size();

    for (unsigned int y = 0; y < m; ++y)
    {
        for (unsigned int x = 0; x < n; ++x)
        {
            // tile Data can be
            // 0 -> empty space
            // 1 -> wall or obstacle
            // 2 -> enemy ( meowth )
            // 3 -> coin ( pokeball )
            // 4 -> exit
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) // solid
            {
                glm::vec2 pos(((float)levelWidth / (float)n) * x, ((float)levelHeight / (float)m) * y);
                glm::vec2 size(((float)levelWidth / (float)n), ((float)levelHeight / (float)m));
                GameObject obj(pos, size, ResourceManager::GetTexture("wall"),
                               glm::vec3(0.8f, 0.8f, 0.7f));
                obj.IsSolid = true;
                obj.Enemy = false;
                obj.IsCoin = false;
                obj.Exit = false;
                obj.idx = itr++;
                this->walls.push_back(obj);
            }
            else if (tileData[y][x] == 2)
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                glm::vec2 pos(((float)levelWidth / (float)n) * x, ((float)levelHeight / (float)m) * y);
                glm::vec2 size(((float)levelWidth / (float)n), ((float)levelHeight / (float)m));
                size = size * 0.7f;
                GameObject obj(pos, size, ResourceManager::GetTexture("ball"),
                               color);
                this->coins.push_back(obj);
                obj.Enemy = false;
                obj.IsCoin = true;
                obj.IsSolid = false;
                obj.Exit = false;
                obj.idx = itr++;

                this->walls.push_back(obj);
            }
            else if (tileData[y][x] == 3)
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                glm::vec2 pos(((float)levelWidth / (float)n) * x, ((float)levelHeight / (float)m) * y);
                glm::vec2 size(((float)levelWidth / (float)n), ((float)levelHeight / (float)m));
                size = PLAYER_SIZE * 0.8f;
                GameObject obj(pos, size, ResourceManager::GetTexture("meowth"),
                               color);
                this->enemy.push_back(obj);
                obj.Enemy = true;
                obj.Destroyed = false;
                obj.IsCoin = false;
                obj.Exit = false;
                obj.IsSolid = false;
                obj.Direction = rand() % 2;
                obj.idx = itr++;
                this->walls.push_back(obj);
            }
            else if (tileData[y][x] == 4) // non-solid; now determine its
                                          // color based on level data
            {
                glm::vec3 color = glm::vec3(1.0f); // original: white
                glm::vec2 pos(((float)levelWidth / (float)n) * x, ((float)levelHeight / (float)m) * y);
                glm::vec2 size(((float)levelWidth / (float)n), ((float)levelHeight / (float)m));
                size = size * 0.7f;

                Texture2D sh;
                sh = ResourceManager::GetTexture("exit");
                GameObject obj(pos, size, sh, color);

                obj.Enemy = false;
                obj.IsCoin = true;
                obj.IsSolid = false;
                obj.Exit = true;
                obj.idx = itr++;

                this->exit = pos;
                this->walls.push_back(obj);
            }
            else if (tileData[y][x] == 5)
            {
                glm::vec2 pos(((float)levelWidth / (float)n) * x, ((float)levelHeight / (float)m) * y);
                this->start = pos;
            }
        }
    }
}