
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Camera.h"
#include "Shader.h"
#include "glm/gtx/projection.hpp"
#include "callback.h"
#include "Vertexarray.h"

int main()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	GLFWwindow *window = glfwCreateWindow(1000, 750, "Hello World", nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	int height, width;
	glfwGetFramebufferSize(window, &width, &height);

	aspect_ratio = width / height;

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	{
		//GLcall(glEnable(GL_BLEND));
		GLcall(glEnable(GL_DEPTH_TEST));

		std::string resPath = searchRes();
		Shader basic_shader((resPath + "/shaders/line.vert").c_str(), (resPath + "/shaders/line.frag").c_str());
		basic_shader.Bind();

		glm::mat4 model(1.0);
		glm::mat4 view(1.0);
		glm::mat4 projpersp(1.0);

		std::vector<Vertex> tris = {
			Vertex(vec3(0.0f, 0.5f, 1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)),
			Vertex(vec3(0.0f, 0.5f, -1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f)),
			Vertex(vec3(0.0f, -0.5, 0.0f), vec4(0.0f, 0.0f, 1.0f, 1.0f)),
			Vertex(vec3(0.5f, 0.0f, 0.0f), vec4(1.0f, 1.0f, 0.0f, 0.0f)),
			Vertex(vec3(-0.5f, 0.0f, 0.0f), vec4(0.0f, 1.0f, 1.0f, 0.0f)),
			Vertex(vec3(0.0f, 1.0f, 0.0f), vec4(1.0f, 0.0f, 1.0f, 0.0f)),
		};


		std::vector<unsigned int> indices = {
			0, 1, 2,
			3, 4, 5};

		std::vector<unsigned> lineindices = {0, 1};

		Vertexarray vao, vao1;
		buffer<Vertex, GL_ARRAY_BUFFER> vbo, vbo1;
		buffer<uint32_t, GL_ELEMENT_ARRAY_BUFFER> ibo, ibo1;


		vbo.AddData(tris);
		ibo.AddData(indices);
		vao.AddBuffer(vbo, ibo);

		

		for (int i = 0; i < 6; i++)
		{
			tris[i].position.x += 2.0f;
			tris[i].position.z += 2.0f;

			if (tris[i].color.x == 1.0f)
				tris[i].color.x = 0.0;
			else
				tris[i].color.x = 1.0;

			if (tris[i].color.y == 1.0f)
				tris[i].color.y = 0.0;
			else
				tris[i].color.y = 1.0;

			if (tris[i].color.z == 1.0f)
				tris[i].color.z = 0.0;
			else
				tris[i].color.z = 1.0;
		}

		

		vbo1.AddData(tris);
		ibo1.AddData(indices);
		vao1.AddBuffer(vbo1, ibo1);

		float near_point{0.1f}, far_point(100);

		double LastFrame = 0;
		    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		while (!glfwWindowShouldClose(window))
		{
			deltatime = glfwGetTime() - LastFrame;
			LastFrame = glfwGetTime();

			processInput(window);

			GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

			view = glm::lookAt(cam.Camera_Position, cam.Camera_Position + cam.Camera_Facing_Direction * cam.Camera_Target_distance, cam.Camera_Up);

			projpersp = glm::perspective(glm::radians(perspective_fov), aspect_ratio, near_point, far_point);

			basic_shader.Bind();
			basic_shader.SetUniform<glm::mat4 *>("model", &model);
			basic_shader.SetUniform<glm::mat4 *>("view", &view);
			basic_shader.SetUniform<glm::mat4 *>("proj", &projpersp);

			vao.Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			vao1.Bind();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// GLcall(glLineWidth(2));
			// linevao.Bind();
			// lineibo.Bind();
			// auto count = lineibo.get_count();
			// GLcall(glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, nullptr));

			GLcall(glfwSwapBuffers(window));
			GLcall(glfwPollEvents());
		}
	}
	return 0;
}
