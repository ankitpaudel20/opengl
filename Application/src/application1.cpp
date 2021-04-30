
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Shader.h"
#include "buffer.h"
#include "Vertexarray.h"
#include "Camera.h"
#include "glm/gtx/projection.hpp"
#include "callback.h"
#include "Drawable.h"
#include "Shapes.h"

int main()
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	{
		//GLcall(glEnable(GL_BLEND));
		GLcall(glEnable(GL_DEPTH_TEST));

		std::string resPath = searchRes();
		//Shader basic_shader("../../opengl/res/shaders/line.vert", "../../opengl/res/shaders/line.frag");
		Shader basic_shader((resPath + "/shaders/line.vert").c_str(), (resPath + "/shaders/line.frag").c_str());
		basic_shader.Bind();

		glm::mat4 model(1.0);
		glm::mat4 view(1.0);
		glm::mat4 projpersp(1.0);

		/*				float vertices[] = {
						0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
						0.0f, 0.5f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, -0.5, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
						0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
						-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
				};*/

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

		/*float vertices2[] = {
				  0.0f, 0.0f, 3.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				  0.0f, 0.0f, 2.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				  0.0f, 0.5, 2.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				  0.5f, 0.0f, 2.0f, 1.0f, 1.0f, 0.0f, 1.0f,
				  -0.5f, 0.0f, 2.0f, 0.0f, 1.0f, 1.0f, 1.0f,
				  0.0f, 0.5f, 2.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		  };*/

		Vertexarray linevao, vao1, vao2, vao3;
		buffer<Vertex, GL_ARRAY_BUFFER> linevbo, vbo1, vbo2, vbo3;
		buffer<uint32_t, GL_ELEMENT_ARRAY_BUFFER> lineibo, ibo1, ibo2, ibo3;

		std::vector<unsigned> lineindices = {0, 1};

		linevbo.AddData(std::vector<Vertex>{
			Vertex(vec3(-1.0f, -1.0f, -1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)),
			Vertex(vec3(1.0f, 1.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f))});
		linevao.AddBuffer(linevbo, lineibo);
		lineibo.AddData(lineindices);

		Drawable<Vertex> testdraw(tris, indices);
		//Drawable<Vertex> testdraw1(cube::pos, cube::indices);
		//testdraw1(testdraw);

		/*	vbo3.AddData(std::vector<Vertex>{
			Vertex(vec3(-1.0f, -1.0f, -1.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f)),
			Vertex(vec3(1.0f, 1.0f, 1.0f), vec4(0.0f, 1.0f, 0.0f, 1.0f))});*/

		vbo1.AddData(tris);
		vao1.AddBuffer(vbo1, ibo1);
		ibo1.AddData(indices);

		for (int i = 0; i < tris.size(); i++)
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

		vbo2.AddData(tris);
		vao2.AddBuffer(vbo2, ibo2);
		ibo2.AddData(indices);

		std::vector<vec3> trisPos;
		for (auto &i : tris)
			trisPos.push_back(i.position);

		const std::vector<Indexdata> trisIndices{
			{{0, 1, 2}, vec4(1, 0, 0), {vec2(0, 0), vec2(1, 0), vec2(1, 1)}, 1},
			{{3, 4, 5}, vec4(1, 1, 0), {vec2(0, 0), vec2(1, 0), vec2(1, 1)}, 1}};

		Drawable<Vertex> testdraw2(cube::pos, cube::indices);

		float near_point{0.1f}, far_point(100);

		double LastFrame = 0;
		//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

			testdraw.Draw(basic_shader);
			testdraw2.Draw(basic_shader);

			/*vao1.Bind();
			GLcall(glDrawElements(GL_TRIANGLES, ibo1.get_count(), GL_UNSIGNED_INT, nullptr));

			vao2.Bind();
			GLcall(glDrawElements(GL_TRIANGLES, ibo2.get_count(), GL_UNSIGNED_INT, nullptr));*/

			GLcall(glLineWidth(2));

			linevao.Bind();
			lineibo.Bind();
			auto count = lineibo.get_count();
			GLcall(glDrawElements(GL_LINES, count, GL_UNSIGNED_INT, nullptr));

			GLcall(glfwSwapBuffers(window));
			GLcall(glfwPollEvents());
		}
	}
	return 0;
}
