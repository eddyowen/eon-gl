#include "Shader.h"
#include "Log.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Callback executed whenever the user resizes the window, so thw viewport gets adjusted
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else if ((glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void OpenGLLogMessage(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		EON_ERROR("[OPENGL::ERROR] {0}", message);
		ASSERT(false, "GL_DEBUG_SEVERITY_HIGH");
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		EON_WARN("[OPENGL::WARNING] {0}", message);
		break;
	case GL_DEBUG_SEVERITY_LOW:
		EON_INFO("[OPENGL::INFO] {0}", message);
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		EON_TRACE("[OPENGL::TRACE] {0}", message);
		break;
	}
}

void EnableGLDebugging()
{
	glDebugMessageCallback(OpenGLLogMessage, nullptr);
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
}

int main()
{
	// Initialize Logging System
	eon::Log::Init();

	// Initialize GLFW.
	glfwInit();
	// Configures the OpenGL version for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Configures the OpenGL Profile to be used
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow * window = glfwCreateWindow(800, 600, "EON GL", NULL, NULL);
	if (window == NULL)
	{
		EON_ERROR("Failed to create GLFW window.");
		glfwTerminate();
		return -1;
	}

	// Make the context of the current Window, the main context
	glfwMakeContextCurrent(window);

	// Try to initialize GLAD (passing to GLAD the function to load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		EON_ERROR("Failed to initialize GLAD");
		return -1;
	}
	else
	{
		EON_INFO("GLAD succesfully created.");
		EON_INFO("OpenGL Info:");
		EON_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		EON_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		EON_INFO("  Version: {0}", glGetString(GL_VERSION));
	}

	EnableGLDebugging();

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	eon::Shader shader("resource/shaders/Basic.shader");

	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	// Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex Buffer Object
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Element Buffer Object
	//unsigned int EBO;
	//glGenBuffers(1, &EBO));
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	// Configure the Position Attribute for its use in the VertexShader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbing everything to test how the VAO stores the current state
	// - "A VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
	// This also means it stores its unbind calls so make sure you don't unbind the element array buffer
	// before unbinding your VAO, otherwise it doesn't have an EBO configured"
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		// Input Handling
		processInput(window);

		// Render here
		// =========================================
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.Use();

		glBindVertexArray(VAO);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//==========================================

		// Swap the front and back buffers to render
		glfwSwapBuffers(window);
		// Process triggered events
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}