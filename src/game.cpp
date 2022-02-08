#include "game.h"

#include <bits/stdc++.h>

#include <iostream>
using namespace std;
#include "game_object.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "text_renderer.h"
// Game-related State data
SpriteRenderer *Renderer;
TextRenderer *Text;
TextRenderer *TIME;

GameObject *Player;
Game::Game(unsigned int width, unsigned int height)
    : State(GAME_START),
      Keys(),
      Width(width),
      Height(height),
      light(Savera) {}

Game::~Game()
{
    delete Renderer;
    delete Player;
}

Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // up
        glm::vec2(1.0f, 0.0f),  // right
        glm::vec2(0.0f, -1.0f), // down
        glm::vec2(-1.0f, 0.0f)  // left
    };
    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

Collision CheckCollision(GameObject &one,
                         GameObject &two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                      two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                      two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    auto difference = one.Position - two.Position;
    if (collisionX && collisionY)
    {
        return std::make_tuple(true, VectorDirection(difference), difference);
    }
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

void Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level].walls)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Player, box);
            if (std::get<0>(collision)) // if collision is true
            {
                // destroy block if not solid
                if (!box.IsSolid)
                {
                    if (box.Enemy)
                    {
                        std::cout << "Enemy Collision " << std::endl;
                        this->State = GAME_LOST;
                        // this->Level = 1;
                    }
                    if (!box.Exit)
                    {
                        box.Destroyed = true;
                        if (this->light == Savera)
                            this->score += 1;
                        else
                            this->score += 5;
                        std::cout << "Score : " << this->score << std::endl;
                    }
                }
                // collision resolution
                else
                {
                    Direction dir = std::get<1>(collision);
                    glm::vec2 diff_vector = std::get<2>(collision);
                    if (dir == LEFT || dir == RIGHT) // horizontal collision
                    {
                        float penetration =
                            Player->Size.x / 2 - std::abs(diff_vector.x);
                        if (dir == LEFT)
                            Player->Position.x +=
                                penetration; // move Player to right
                        else
                            Player->Position.x -=
                                penetration; // move Player to left;
                    }
                    else // vertical collision
                    {
                        float penetration =
                            Player->Size.x / 2 - std::abs(diff_vector.y);
                        if (dir == UP)
                            Player->Position.y -=
                                penetration; // move Player back up
                        else
                            Player->Position.y +=
                                penetration; // move Player back down
                    }
                }
            }
        }
    }
}

bool helperFunction(GameObject &one, GameObject &two) // AABB - AABB collision
{
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                      two.Position.x + two.Size.x >= one.Position.x;
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                      two.Position.y + two.Size.y >= one.Position.y;

    return collisionX && collisionY;
}

void make_map(int level)
{
    srand(time(0));

    vector<pair<int, int>> path;

    int rows = 15, cols = 25;
    int map[rows][cols] = {0};
    int num_coins = (level + 1) * 3 + 5;
    int walls = level + 4;
    int num_enemies = (level + 1) * 3 + 5;
    pair<int, int> start = {rows - 1, cols / 2};

    for (int i = 0; i < rows; i++)
    {
        for (auto itr = 0; itr < walls; itr++)
        {
            int x = rand() % rows;
            map[i][x] = 1;
        }
        if (num_coins > 0)
        {
            int r3 = rand() % cols;
            map[i][r3] = 2;
            num_coins--;
        }
    }
    while (num_enemies--)
    {
        int r1 = rand() % cols;
        int r2 = rand() % rows;
        if (r1 != 0 && r1 != cols - 1 && r2 != 0 && r2 != rows - 1)
            map[r2][r1] = 3;
    }

    for (int i = 0; i < rows; i++)
        map[i][0] = map[i][cols - 1] = 1;
    for (int i = 0; i < cols; i++)
        map[0][i] = map[rows - 1][i] = 1;

    map[0][cols / 2 - 1] = 0;
    map[0][cols / 2] = 4;
    map[0][cols / 2 + 1] = 0;
    map[1][cols / 2 - 1] = 0;
    map[1][cols / 2 + 1] = 0;
    map[1][cols / 2] = 0;

    map[rows - 1][cols / 2 - 2] = 0;
    map[rows - 1][cols / 2 - 1] = 0;
    map[rows - 1][cols / 2] = 5;
    map[rows - 2][cols / 2 - 2] = 0;
    map[rows - 2][cols / 2 - 1] = 0;
    map[rows - 2][cols / 2] = 0;

    FILE *fptr;

    if (level == 0)
        fptr = fopen("/home/kjsd1232/2020101062_Assignment1/src/levels/one.lvl", "w");
    else if (level == 1)
        fptr = fopen("/home/kjsd1232/2020101062_Assignment1/src/levels/two.lvl", "w");
    else if (level == 2)
        fptr = fopen("/home/kjsd1232/2020101062_Assignment1/src/levels/three.lvl", "w");

    for (auto i = 0; i < rows; i++)
    {
        for (auto j = 0; j < cols; j++)
        {
            std::fprintf(fptr, "%d ", map[i][j]);
        }
        std::fprintf(fptr, "\n");
    }

    fclose(fptr);
}

void Game::Init()
{
    // load shaders
    ResourceManager::LoadShader("/home/kjsd1232/2020101062_Assignment1/src/shaders/sprite.vs",
                                "/home/kjsd1232/2020101062_Assignment1/src/shaders/sprite.frag",
                                nullptr, "sprite");
    // configure shaders
    glm::mat4 projection =
        glm::ortho(0.0f, static_cast<float>(this->Width),
                   static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Shader y;
    Text = new TextRenderer(this->Width * 3, this->Height * 3);
    string s("/home/kjsd1232/2020101062_Assignment1/src/fonts/ocratext.ttf");
    Text->Load(s, 24);
    TIME = new TextRenderer(this->Width * 3, this->Height * 3);
    string s1("/home/kjsd1232/2020101062_Assignment1/src/fonts/ocratext.ttf");
    TIME->Load(s1, 24);

    y = ResourceManager::GetShader("sprite");
    Renderer = new SpriteRenderer(y);
    // load textures
    ResourceManager::LoadTexture(
        "/home/kjsd1232/2020101062_Assignment1/src/textures/background.jpg", false,
        "background");
    ResourceManager::LoadTexture(
        "/home/kjsd1232/2020101062_Assignment1/src/textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture(
        "/home/kjsd1232/2020101062_Assignment1/src/textures/meowth1.png", true, "meowth");

    ResourceManager::LoadTexture(
        "/home/kjsd1232/2020101062_Assignment1/src/textures/pokeball.png", true, "ball");

    ResourceManager::LoadTexture(
        "/home/kjsd1232/2020101062_Assignment1/src/textures/block_solid.png", false,
        "wall");
    ResourceManager::LoadTexture(
        "/home/kjsd1232/2020101062_Assignment1/src/textures/pikachu.png", true, "pikachu");
    ResourceManager::LoadTexture("/home/kjsd1232/2020101062_Assignment1/src/textures/exit.png",
                                 true, "exit");

    make_map(0);
    make_map(1);
    make_map(2);

    GameLevel one;
    one.Load("/home/kjsd1232/2020101062_Assignment1/src/levels/one.lvl", this->Width,
             this->Height);
    GameLevel two;
    two.Load("/home/kjsd1232/2020101062_Assignment1/src/levels/two.lvl", this->Width,
             this->Height);
    GameLevel three;
    three.Load("/home/kjsd1232/2020101062_Assignment1/src/levels/three.lvl", this->Width,
               this->Height);
    GameLevel four;
    four.Load("/home/kjsd1232/2020101062_Assignment1/src/levels/four.lvl", this->Width,
              this->Height);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    time(&this->start);
    // configure game objects
    glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
                                    this->Height - PLAYER_SIZE.y);

    Player = new GameObject(playerPos, PLAYER_SIZE * 0.7f,
                            ResourceManager::GetTexture("pikachu"));
}

bool Game::checkEnemyCollisonHelperFunction(GameObject &enemy)
{
    for (GameObject &box : this->Levels[this->Level].walls)
    {
        if (!box.Destroyed && !box.IsCoin && box.idx != enemy.idx)
        {
            if (helperFunction(enemy, box))
            {
                return true;
            }
        }
    }
    if (helperFunction(*Player, enemy))
    {
        this->State = GAME_LOST;
        return true;
    }
    return false;
}

void Game::Update(float dt)
{
    // update objects
    Player->Move(dt, this->Width);
    // check for collisions
    this->DoCollisions();
    float velocity = PLAYER_VELOCITY * dt / 2;
    for (GameObject &enemy : this->Levels[this->Level].walls)
    {
        if (enemy.Enemy)
        {
            if (!enemy.Destroyed)
            {
                if (enemy.Direction && enemy.Position.x >= 0.0f)
                {
                    enemy.Position.x -= velocity;
                    if (checkEnemyCollisonHelperFunction(enemy))
                    {
                        auto vec = Player->Position - enemy.Position;
                        enemy.Position.x += velocity;
                        enemy.Destroyed = true;
                    }
                }
                else if (!enemy.Direction && enemy.Position.y >= 0.0f)
                {
                    enemy.Position.y -= velocity;
                    if (checkEnemyCollisonHelperFunction(enemy))
                    {
                        auto vec = Player->Position - enemy.Position;
                        enemy.Position.y += velocity;
                        enemy.Destroyed = true;
                    }
                }
                else
                {
                    enemy.Destroyed = true;
                }
            }
            if (enemy.Destroyed)
            {

                if (enemy.Direction && enemy.Position.x <= this->Width - enemy.Size.x)
                {
                    enemy.Position.x += velocity;
                    if (checkEnemyCollisonHelperFunction(enemy))
                    {
                        enemy.Position.x -= velocity;
                        enemy.Destroyed = false;
                    }
                }
                else if (!enemy.Direction && enemy.Position.y <= this->Height - enemy.Size.y)
                {
                    enemy.Position.y += velocity;
                    if (checkEnemyCollisonHelperFunction(enemy))
                    {
                        enemy.Position.y -= velocity;
                        enemy.Destroyed = false;
                    }
                }
                else
                    enemy.Destroyed = false;
            }
        }
    }

    //  if (this->State == GAME_ACTIVE)
    {
        int var = (this->light == Savera);
        glm::vec3 lightSource = glm::vec3(Player->Position.x, Player->Position.y, 0.0f);
        // std::cout << lightSource.x << " " << lightSource.y << " " << lightSource.z << "\n";
        ResourceManager::GetShader("sprite").Use().SetVector3f("lightSource", lightSource);
        ResourceManager::GetShader("sprite").Use().SetInteger("lightSwitch", var);
    }
}

void Game::ProcessInput(float dt)
{
    //  std ::cout << "x : " << Player->Position.x << " y : " << Player->Position.y << std ::endl;
    // std ::cout << "x : " << this->Levels[this->Level].exit.x << " y : " << this->Levels[this->Level].exit.y << std ::endl;
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
            //		std::cout << "x : " << Player->Position.y << std::endl;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
        }
        if (this->Keys[GLFW_KEY_W])
        {
            if (Player->Position.y >= 0.0f)
                Player->Position.y -= velocity;
            //		std::cout << "y : " << Player->Position.y << std::endl;
        }
        if (this->Keys[GLFW_KEY_S])
        {
            if (Player->Position.y <= this->Height - Player->Size.y)
                Player->Position.y += velocity;

            //		std::cout << Player->Position.y << std::endl;
        }
        if (this->Keys[GLFW_KEY_ENTER])
        {
            this->State = GAME_ACTIVE;
        }
        if (glm ::distance(Player->Position, this->Levels[this->Level].exit) < PLAYER_SIZE.x / 2.0f)
        {
            this->Level++;
            Player->Position = this->Levels[this->Level].start;
        }

        // std::cout << Player->Position.x << " " << Player->Position.y
        // 		  << std::endl;
        // std::cout << "Level : " << this->Level << std::endl;
        // if (Player->Position.y == 0.0f && Player->Position.x > 462.0f) {

        // 	if (this->Level == 3)
        // 		this->State = GAME_WIN;
        // 	else
        // 		this->Level++;
        // 	this->Level++;
        // 	std::cout << this->Level << std::endl;
        // }
    }
}

void Game::Render()
{
    if (this->State == GAME_START)
    {
        Texture2D y;
        y = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(y, glm::vec2(0.0f, 0.0f),
                             glm::vec2(this->Width, this->Height), 0.0f);

        std::stringstream ss;
        ss << "Press Enter to start";
        Text->RenderText(ss.str(), 220.0f, 200.0f, 3.0f);
    }
    if (this->State == GAME_ACTIVE)
    {
        // draw background
        Texture2D y;
        y = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(y, glm::vec2(0.0f, 0.0f),
                             glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        // Text->RenderText("Lives:" + ss.str(), 5.0f, 5.0f, 1.0f);
        if (this->Level == 3)
        {
            std::cout << "Game Won" << std::endl;
            this->State = GAME_WIN;
            return;
        }
        this->Levels[this->Level].Draw(*Renderer);
        // draw player
        Player->Draw(*Renderer);

        std::stringstream ss;
        ss << this->score;
        time_t curr_time;
        time(&curr_time);
        auto display = double(curr_time - this->start);
        std::stringstream fs;
        fs << display;
        std::stringstream sts;
        sts << this->Level + 1;
        Text->RenderText("Score->" + ss.str(), 220.0f, 200.0f, 2.0f, glm ::vec3(1.0f, 1.0f, 1.0f));
        TIME->RenderText("Time ->" + fs.str(), 220.0f, 300.0f, 2.0f, glm ::vec3(1.0f, 1.0f, 1.0f));
        Text->RenderText("Level->" + sts.str(), 220.0f, 400.0f, 2.0f, glm ::vec3(1.0f, 1.0f, 1.0f));
    }

    if (this->State == GAME_LOST)
    {
        Texture2D y;
        y = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(y, glm::vec2(0.0f, 0.0f),
                             glm::vec2(this->Width, this->Height), 0.0f);

        std::stringstream ss;

        ss << "LOST";
        auto display =this->score;
        std::stringstream fs;
        fs << display;

        Text->RenderText("Score :" + fs.str(), 220.0f, 400.0f, 8.0f, glm ::vec3(1.0f, 1.0f, 1.0f));
        Text->RenderText(ss.str(), 220.0f, 200.0f, 10.0f, glm ::vec3(1.0f, 1.0f, 1.0f));
    }
    if (this->State == GAME_WIN)
    {
        Texture2D y;
        y = ResourceManager::GetTexture("background");
        Renderer->DrawSprite(y, glm::vec2(0.0f, 0.0f),
                             glm::vec2(this->Width, this->Height), 0.0f);

        std::stringstream ss;
        ss << "WON GAME";
        Text->RenderText(ss.str(), 220.0f, 200.0f, 10.0f);
    }
}