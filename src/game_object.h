#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "sprite_renderer.h"
#include "texture.h"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // object state
    int idx;
    glm::vec2 Position;
    glm ::vec2 Size;
    glm ::vec2 Velocity;
    glm::vec3 Color;
    bool change_dir;
    float Rotation;
    bool IsSolid;
    bool Enemy;
    bool Destroyed;
    bool IsCoin;
    bool Exit;
    bool Direction;
    // render state
    Texture2D Sprite;
    // constructor(s)
    GameObject();

    glm::vec2 Move(float dt, unsigned int window_width);

    void Reset(glm::vec2 position, glm::vec2 velocity);

    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite,
               glm::vec3 color = glm::vec3(1.0f),
               glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

    // draw sprite
    virtual void Draw(SpriteRenderer &renderer);
};

#endif
