#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <bits/stdc++.h>
using namespace std;

#include "shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// Global Variables

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const char *SCR_TITLE = "Assignment0 2020101062";

// variables

vector<float> vertex;			   // array to store the vertex array
vector<unsigned int> vertex_index; // array to store the vertex_indexing of the vertexes

GLFWwindow *create_window()
{
	// initialise the window and window hints

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// conditional compilation for APPLE
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// create the window object
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, SCR_TITLE, NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return NULL;
	}
	// change the current context
	glfwMakeContextCurrent(window);

	// incase the view port size is changed , adjust the view port
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// load the glad library
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return NULL;
	}

	// set the view port
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	return window;
}

void generate_prism(int n)
{
	float beta = (360.0f / ((float)n));

	vertex.push_back(0);
	vertex.push_back(0);
	vertex.push_back(0);
	vertex.push_back(0);
	vertex.push_back(0);
	vertex.push_back(0);

	for (auto i = 0; i < n; i++)
	{
		float alpha = glm ::radians((float)i * beta);
		vertex.push_back(glm ::cos(alpha));
		vertex.push_back(glm::sin(alpha));
		vertex.push_back(0);

		vertex.push_back(glm ::cos(alpha));
		vertex.push_back(glm::sin(alpha));
		vertex.push_back(glm ::sin(2 * alpha));
	}

	for (auto i = 1; i < n; i++)
	{
		vertex_index.push_back(0);
		vertex_index.push_back(i + 1);
		vertex_index.push_back(i);

		vertex_index.push_back(n + 1);
		vertex_index.push_back((i + 1) % n);
		vertex_index.push_back(i);
	}

	vertex_index.push_back(0);
	vertex_index.push_back(1);
	vertex_index.push_back(n);
}

int main(int argc, char *argv[])
{
	int n;
	switch (argc)
	{
	case 2:
		n = atoi(argv[1]);
		if (n == 0)
		{
			cout << "Invalid Input , n must be a positive integer" << endl;
			return -1;
		}
		break;
	default:
		cout << "Invalid Number of arguments " << endl;
		return -1;
		break;
	}

	// glfw window creation
	auto window = create_window();
	if (window == NULL)
	{
		return -1;
	}

	// build and compile our shader program
	Shader ourShader("../src/vertex.shader", "../src/fragment.shader");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// float vertices[] = {
	// 	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // left
	// 	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // right
	// 	0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f	  // top
	// };
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s),
	// and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), vertex.data(), GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_index.size() * sizeof(unsigned int), vertex_index.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered
	// VBO as the vertex attribute's bound vertex buffer object so afterwards we
	// can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally
	// modify this VAO, but this rarely happens. Modifying other VAOs requires a
	// call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
	// VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// render loop
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBuffer
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// make transformations
		//	glm::mat4 transform = glm::mat4(1.0f);
		//	transform = glm::rotate(transform, (float)glfwGetTime(),
		//		glm::vec3(0.0f, 0.0f, 1.0f));

		ourShader.use();
		//  unsigned int transformLoc =
		//  glGetUniformLocation(ourShader.ID, "transform");
		//	glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
		//  glm::value_ptr(transform));

		glBindVertexArray(VAO); // seeing as we only have a single VAO there's
		// no need to bind it every time, but we'll do
		// so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // no need to unbind it every time

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse
		// moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width
	// and height will be significantly larger than specified on retina
	// displays.
	glViewport(0, 0, width, height);
}
