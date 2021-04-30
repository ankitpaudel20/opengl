#pragma once

#include <filesystem>
namespace fs = std::filesystem;

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "core.h"

Camera cam(glm::vec3(-2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0, -45.0, -45.0);
double mx, my;
bool captured = false;
float aspect_ratio;
double deltatime = 0;
float sensitivity = 2.5;
float perspective_fov(90);
vec3 lightPos(2.0f, 2.0f, 2.0f);
//pointLight light(lightPos, 1 );

#define NR_POINT_LIGHTS 1
std::vector<pointLight> pointLights;
DirLight directionLight(1, 0.31);
vec3 AmbientLight(1);
unsigned selected_light = 0;

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
		return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:
		std::cout << "Source: API";
		break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		std::cout << "Source: Window System";
		break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		std::cout << "Source: Shader Compiler";
		break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		std::cout << "Source: Third Party";
		break;
	case GL_DEBUG_SOURCE_APPLICATION:
		std::cout << "Source: Application";
		break;
	case GL_DEBUG_SOURCE_OTHER:
		std::cout << "Source: Other";
		break;
	}
	std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:
		std::cout << "Type: Error";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		std::cout << "Type: Deprecated Behaviour";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		std::cout << "Type: Undefined Behaviour";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		std::cout << "Type: Portability";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		std::cout << "Type: Performance";
		break;
	case GL_DEBUG_TYPE_MARKER:
		std::cout << "Type: Marker";
		break;
	case GL_DEBUG_TYPE_PUSH_GROUP:
		std::cout << "Type: Push Group";
		break;
	case GL_DEBUG_TYPE_POP_GROUP:
		std::cout << "Type: Pop Group";
		break;
	case GL_DEBUG_TYPE_OTHER:
		std::cout << "Type: Other";
		break;
	}
	std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		std::cout << "Severity: high";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		std::cout << "Severity: medium";
		break;
	case GL_DEBUG_SEVERITY_LOW:
		std::cout << "Severity: low";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		std::cout << "Severity: notification";
		break;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	DEBUG_BREAK;
}

static void cursor_position_callback(GLFWwindow* window, double x, double y)
{
	if (captured)
	{
		float xoffset = mx - x;
		float yoffset = my - y;
		mx = x;
		my = y;

		float sensitivity = 0.09;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		cam.DelYaw(xoffset);
		cam.DelPitch(yoffset);
	}
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	if (height != 0)
		aspect_ratio = (float)width / height;
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void scroll_callback(GLFWwindow* window, double xoff, double yoff)
{
	perspective_fov += (sensitivity * yoff);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

	switch (action)
	{
	case GLFW_PRESS:
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;

		case GLFW_KEY_1:
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			captured = false;
			break;
		case GLFW_KEY_2:
			glfwSetCursorPos(window, mx, my);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			captured = true;
			break;
		case GLFW_KEY_KP_4:
			if (selected_light == 0)
			{
				selected_light = NR_POINT_LIGHTS - 1;
				break;
			}
			selected_light--;
			break;
		case GLFW_KEY_KP_6:
			if (selected_light == NR_POINT_LIGHTS - 1)
			{
				selected_light = 0;
				break;
			}
			selected_light++;
			break;
		default:
			break;
		}
		break;
	}
}

void processInput(GLFWwindow* window)
{
	const float cameraSpeed = 2.5f * deltatime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cam.Camera_Position += cameraSpeed * cam.Camera_Facing_Direction;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cam.Camera_Position -= cameraSpeed * glm::normalize(glm::cross(cam.Camera_Facing_Direction, cam.Camera_Up));

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cam.Camera_Position -= cameraSpeed * cam.Camera_Facing_Direction;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cam.Camera_Position += cameraSpeed * glm::normalize(glm::cross(cam.Camera_Facing_Direction, cam.Camera_Up));

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cam.Camera_Position += cameraSpeed * cam.Camera_Up;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cam.Camera_Position -= cameraSpeed * cam.Camera_Up;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		pointLights[selected_light].position.x +=(0.05 * cam.Camera_Facing_Direction.x);
		pointLights[selected_light].position.y += (0.05 * cam.Camera_Facing_Direction.y);
		pointLights[selected_light].position.z +=(0.05 * cam.Camera_Facing_Direction.z);
		//pointLights[selected_light].position += (0.05 * vec3(cam.Camera_Facing_Direction));
		//pointLights[selected_light].position.z -= cameraSpeed * 5;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		pointLights[selected_light].position.x -= (0.05 * cam.Camera_Facing_Direction.x);
		pointLights[selected_light].position.y -= (0.05 * cam.Camera_Facing_Direction.y);
		pointLights[selected_light].position.z -= (0.05 * cam.Camera_Facing_Direction.z);
		//pointLights[selected_light].position -= (0.05 * vec3(cam.Camera_Facing_Direction));
		//pointLights[selected_light].position.z += cameraSpeed * 5; 
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { 
		vec3 a = glm::cross(cam.Camera_Up, cam.Camera_Facing_Direction);
		pointLights[selected_light].position.x += (0.05 * a.x);
		pointLights[selected_light].position.y += (0.05 * a.y);
		pointLights[selected_light].position.z += (0.05 * a.z);

		//pointLights[selected_light].position += 0.05 * vec3(glm::cross(cam.Camera_Up,cam.Camera_Facing_Direction));

		//pointLights[selected_light].position.x -= cameraSpeed * 5; 
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		vec3 a = glm::cross(cam.Camera_Up, cam.Camera_Facing_Direction);
		pointLights[selected_light].position.x -= (0.05 * a.x);
		pointLights[selected_light].position.y -= (0.05 * a.y);
		pointLights[selected_light].position.z -= (0.05 * a.z);

		//pointLights[selected_light].position.x += cameraSpeed * 5; 
	}

	if (glfwGetKey(window, GLFW_KEY_KP_0) == GLFW_PRESS) {
		pointLights[selected_light].position.x += (0.05 * cam.Camera_Up.x);
		pointLights[selected_light].position.y += (0.05 * cam.Camera_Up.y);
		pointLights[selected_light].position.z += (0.05 * cam.Camera_Up.z);
		//pointLights[selected_light].position.y -= cameraSpeed * 5;
	}

	if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
		pointLights[selected_light].position.x -= (0.05 * cam.Camera_Up.x);
		pointLights[selected_light].position.y -= (0.05 * cam.Camera_Up.y);
		pointLights[selected_light].position.z -= (0.05 * cam.Camera_Up.z);
		//pointLights[selected_light].position.y += +cameraSpeed * 5;
	}
}

std::string searchRes()
{
	auto currentPath = fs::current_path();
	auto orgiPath = currentPath;
	bool resPathFound = false, appPathFound = false;

	while (!resPathFound)
	{
		if (!resPathFound && !appPathFound && currentPath.has_parent_path())
		{
			fs::current_path(currentPath.parent_path());
			currentPath = fs::current_path();
		}
		else if (appPathFound)
		{
			try
			{
#ifdef _WIN_32
				auto a = currentPath.string().append("\\Application\\res");
#else
				auto a = currentPath.string().append("/Application/res");
#endif
				fs::current_path(a);
				fs::current_path(orgiPath);
				return a;
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << "\n \"res\" folder not found inside \"Application\" folder \n";
				return std::string();
			}
			resPathFound = true;
			break;
		}
		else
			break;

		//std::cout << "currently in path: " << currentPath.string() << std::endl;

		for (auto& dirs : fs::directory_iterator(currentPath))
		{
			if (dirs.is_directory() && dirs.path().filename().string() == "Application")
			{
				appPathFound = true;
				break;
			}
		}
	}

	std::cout << "folder \"Application\" not found in 4 back iterations" << std::endl;
	return std::string();
}