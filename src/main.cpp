#include <GLFW/glfw3.h>
#include <glad/glad.h>
#define GLFW_INCLUDE_GLCOREARB
#include <iostream>

#include "game.h"
#include "resource_manager.h"

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void toggle_key(GLFWwindow *window, int key, int scancode, int action,
                int mode);

Game AnotherWorld(SCR_WIDTH, SCR_HEIGHT);

void initialise_screen()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

// for creating the opengl window

GLFWwindow *create_window()
{
    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Assignment0", NULL, NULL);
    if (window == NULL)
    {
        std ::cout << "Failed to create GLFW window" << std ::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std ::cout << "Failed to initialize GLAD" << std ::endl;
        return NULL;
    }
    return window;
}
int main()
{

    initialise_screen();
    auto window = create_window();
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    glfwSetKeyCallback(window, toggle_key);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL configuration
    // --------------------
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // initialize game
    // ---------------
    AnotherWorld.Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // manage user input
        // -----------------
        AnotherWorld.ProcessInput(deltaTime);

        // update game state
        // -----------------
        AnotherWorld.Update(deltaTime);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        AnotherWorld.Render();

        glfwSwapBuffers(window);
    }
    // std ::cout << "Bye" << std ::endl;

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

    //	glfwTerminate();
    return 0;
}

void toggle_key(GLFWwindow *window, int key, int scancode, int action,
                int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property
    // to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
        AnotherWorld.Keys[key] = (action == GLFW_PRESS);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        std ::cout << "Hi" << std ::endl;
        AnotherWorld.light == Savera ? AnotherWorld.light = Andhera : AnotherWorld.light = Savera;
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        if (AnotherWorld.State == GAME_START)
            AnotherWorld.State = GAME_ACTIVE;
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width
    // and height will be significantly larger than specified on retina
    // displays.
    glViewport(0, 0, width, height);
}