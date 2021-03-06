#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

class Engine
{
private:

	Engine() = default;

public:

	static void	Create(unsigned int width, unsigned int height, const char* title, class GLFWmonitor* monitor = nullptr, class GLFWwindow* share = nullptr, unsigned int antialiasing = 4);
	static void	Setup();
	static void	Update();
	static void	Render();
	static void	Destroy();

private:

	static int	InitializeGLFWCore();
	static int	InitializeGLFWWindowContext(unsigned int width, unsigned int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, unsigned int antialiasing);
	static int	InitializeGLEWCore();
	static void	InitializeVertexArrayObject();
	static int	InitializeShaders(const char* vertex_file_path, const char* fragment_file_path);

private:

	static void Draw2DTriangle();

public:

	static bool	IsRunning();

private:

	static GLFWwindow*	window;
	static GLuint		vertexArrayID;
	static GLuint		vertexBuffer;
	static GLuint		vertexShaderID;
	static GLuint		fragmentShaderID;
	static GLuint		programID;

};