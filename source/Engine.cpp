#include "Engine.h"





GLFWwindow* Engine::window;
GLuint		Engine::vertexArrayID;
GLuint		Engine::vertexBuffer;
GLuint		Engine::vertexShaderID;
GLuint		Engine::fragmentShaderID;
GLuint		Engine::programID;





#pragma region CREATE - SETUP - UPDATE - RENDER - DESTROY

void Engine::Create(unsigned int width, unsigned int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, unsigned int antialiasing)
{
	InitializeGLFWCore();
	InitializeGLFWWindowContext(width, height, title, monitor, share, antialiasing);
	InitializeGLEWCore();
	InitializeVertexArrayObject();
	InitializeShaders("resource/shaders/vertexShader.glsl", "resource/shaders/fragmentShader.glsl");
}

void Engine::Setup()
{
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);	// for grabbing user input
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);			// set window clear color
}

void Engine::Update()
{
	glfwPollEvents();
}

void Engine::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear frame

	glUseProgram(programID); // shader stuff
	
	// vvv draw frame vvv //

	Draw2DTriangle();

	// ^^^ draw frame ^^^ //

	glfwSwapBuffers(window); // display frame (swap buffer)
}

void Engine::Destroy()
{
	delete window; // what goes up must come down
}

#pragma endregion CREATE - SETUP - UPDATE - RENDER - DESTROY





#pragma region SPECIFIC CREATE FUNCTIONS

int Engine::InitializeGLFWCore()
{
	/* initialize/create glfw */

	glewExperimental = true; // needed for core profile // glewExperimental allows extension entry points to be loaded even if the extension isn’t present in the driver’s extensions string. <- source https://community.khronos.org/t/glewexperimental/72738

	if (!glfwInit())
	{
		fprintf(stderr, "GLFW Initialization - Failed\n");
		return -1;
	}
}

int Engine::InitializeGLFWWindowContext(unsigned int width, unsigned int height, const char* title, GLFWmonitor* monitor, GLFWwindow* share, unsigned int antialiasing)
{
	/* initialize/create OpenGL window and context */

	glfwWindowHint(GLFW_SAMPLES, antialiasing);			// set antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);			// we want openGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);			// ^
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);		// MacOS B.S. 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// say no to old OpenGL

	window = glfwCreateWindow(width, height, title, monitor, share);

	if (window == nullptr)
	{
		fprintf(stderr, "GLFW Window Initialization - Failed");
		glfwTerminate();
		return -1;
	}
}

int Engine::InitializeGLEWCore()
{
	/* initialize/create GLEW */

	glfwMakeContextCurrent(window);
	glewExperimental = true; // needed for core profile

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "GLEW Initialization - Failed\n");
		return -1;
	}
}

void Engine::InitializeVertexArrayObject()
{
	/* initialize/create VOA (Vertext Array Object) */

	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	GLfloat gVertexBufferData[]
	{
		-1.0,	-1.0,	0.0f,
		1.0,	-1.0,	0.0f,
		0.0,	1.0,	0.0f
	};

	glGenBuffers(1, &vertexBuffer);									// generate 1 buffer and put resulting identifier in vertexBuffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);							// ? the following commands will talk about our "vertextBuffer" buffer ?
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);
}

int Engine::InitializeShaders(const char* vertex_file_path, const char* fragment_file_path)
{
	/* initialize/create shaders */

	vertexShaderID		= glCreateShader(GL_VERTEX_SHADER);
	fragmentShaderID	= glCreateShader(GL_FRAGMENT_SHADER);





	/* read the vertex shader code from file */

	std::string vertexShaderCode;
	std::ifstream vertexShaderStream(vertex_file_path, std::ios::in);

	if (vertexShaderStream.is_open())
	{
		std::stringstream strStream;
		strStream << vertexShaderStream.rdbuf();
		vertexShaderCode = strStream.str();
		vertexShaderStream.close();
	}
	else
	{
		printf("Impossible to open %s. Are you in the right directory? Don't forget to read the FAQ!\n", vertex_file_path);
		getchar();
		return 0;
	}





	/* read the fragment shader code from file */

	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragment_file_path, std::ios::in);

	if (fragmentShaderStream.is_open())
	{
		std::stringstream strStream;
		strStream << fragmentShaderStream.rdbuf();
		fragmentShaderCode = strStream.str();
		fragmentShaderStream.close();
	}





	//
	GLint result = GL_FALSE;
	int infoLogLength;





	/* compile vertex shader */

	printf("Compliling shader : %s\n", vertex_file_path);
	char const* vertexSourcePointer = vertexShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vertexSourcePointer, nullptr);
	glCompileShader(vertexShaderID);





	/* check vertex shader */

	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(vertexShaderID, infoLogLength, nullptr, &vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
	}





	/* compile fragment shader */

	printf("Compliling shader : %s\n", fragment_file_path);
	char const* fragmentSourcePointer = fragmentShaderCode.c_str();
	glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, nullptr);
	glCompileShader(fragmentShaderID);





	/* check fragment shader */

	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{
		std::vector<char> fragmentShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, nullptr, &fragmentShaderErrorMessage[0]);
		printf("%s\n", &fragmentShaderErrorMessage[0]);
	}





	/* link the program */

	printf("Linking program\n");
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);





	/* check the program */
	glGetProgramiv(programID, GL_LINK_STATUS, &result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{
		std::vector<char> programErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(programID, infoLogLength, nullptr, &programErrorMessage[0]);
		printf("%s\n", &programErrorMessage);
	}

	glDetachShader(programID, vertexShaderID);
	glDetachShader(programID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}

#pragma endregion SPECIFIC CREATE FUNCTIONS





#pragma region SPECIFIC RENDER FUNCTIONS

void Engine::Draw2DTriangle()
{
	/* 1st attribute buffer : vertices */

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,		// attribute 0. No particular reason, but must match the layout in shader
		3,		// size
		GL_FLOAT,	// type
		GL_FALSE,	// normalized
		0,		// stride
		(void*)0	// array buffer offset
	);

	/* Draw Triangle */

	glDrawArrays(GL_TRIANGLES, 0, 3); // statring from vertex 0; 3 vertices total; 1 triangle;
	glDisableVertexArrayAttrib(0, 0);

}

#pragma endregion SPECIFIC RENDER FUNCTIONS





#pragma region RETURN FUNCTIONS

bool Engine::IsRunning()
{
	/* good enough of a check for now */

	if (
		(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) 	&& 
		(glfwWindowShouldClose(window) == 0)			)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#pragma endregion RETURN FUNCTIONS
