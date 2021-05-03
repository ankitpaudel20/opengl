

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "buffer.h"
#include "Shader.h"
#include "Vertexarray.h"
#include "Camera.h"
#include "glm/gtx/projection.hpp"
#include "callback.h"
#include "Drawable.h"
#include "Shapes.h"
#include "Texture.h"

#include "imgui/imgui.h"

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

int main()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *mainWin;
    GLFWwindow *imguiWin;

    imguiWin = glfwCreateWindow(800, 600, "imgui", NULL, NULL);
    mainWin = glfwCreateWindow(1000, 750, "main", NULL, NULL);

    if (!(mainWin && imguiWin))
    {
        glfwTerminate();
        return -1;
    }



#pragma region imgui initialization

    glfwMakeContextCurrent(imguiWin);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(imguiWin, true);
    ImGui_ImplOpenGL3_Init("#version 440");

#pragma endregion

    glfwMakeContextCurrent(mainWin);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(mainWin, framebuffer_size_callback);
    glfwSetCursorPosCallback(mainWin, cursor_position_callback);
    glfwSetKeyCallback(mainWin, key_callback);

    glfwSetInputMode(mainWin, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    int height, width;
    glfwGetFramebufferSize(mainWin, &width, &height);

    aspect_ratio = width / height;

   

    std::cout << glGetString(GL_VERSION) << std::endl;
    auto glversion = glGetString(GL_VERSION);
    auto glrenderer = glGetString(GL_RENDERER);
    auto glvendor = glGetString(GL_VENDOR);
    glEnable(GL_MULTISAMPLE); 
    //glEnable(GL_CULL_FACE);
    GLcall(glEnable(GL_DEPTH_TEST ));

    {

        //init shaders
        std::string resPath = searchRes();
        Shader lampShader(resPath + "/shaders/lamp.vert", resPath + "/shaders/lamp.frag");
        Shader cubeShader(resPath + "/shaders/cube_final.vert", resPath + "/shaders/cube_final.frag");

        //init textures
        Texture t1(resPath + "/textures/container2.png");
        Texture t2(resPath + "/textures/container2_specular_map.png");

        Drawable<Vertex> cube(cube::pos, cube::indices);
        Drawable<Vertex> lamp(cube::pos, cube::indices);
        pointLights.emplace_back(pointLight(lightPos, 1));
        pointLights[0].constant = 1;
        pointLights[0].linear = 0.09;
        pointLights[0].quadratic = 0.032;

        cube.addTexture(t1);
        cube.addTexture(t2);
        cube.matModel = glm::mat4(1.0);
        lamp.matModel = glm::translate(glm::scale(glm::mat4(1.0), glm::vec3(0.05)),(glm::vec3)pointLights[0].position);
        Material cubeMaterial(32);


        glm::mat4 view(1.0);
        glm::mat4 projpersp(1.0);

        glm::vec3 translate(0, 0, 0);
        glm::vec3 scale(0.5, 0.5, 0.5);
        glm::vec3 rotate(0, 0, 0);
        glm::mat4 translation(1.0);
        glm::mat4 rotation(1.0);        
        glm::mat4 scaling(1.0);

        float near_point{0.1f}, far_point(100);

        double LastFrame = 0;
        //        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        while (!(glfwWindowShouldClose(mainWin) || glfwWindowShouldClose(imguiWin)))
        {
            glfwMakeContextCurrent(mainWin);
            glfwPollEvents();
            deltatime = glfwGetTime() - LastFrame;
            LastFrame = glfwGetTime();

            processInput(mainWin);

            GLcall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            view = glm::lookAt(cam.Camera_Position, cam.Camera_Position + cam.Camera_Facing_Direction * cam.Camera_Target_distance, cam.Camera_Up);

            projpersp = glm::perspective(glm::radians(perspective_fov), aspect_ratio, near_point, far_point);

            lampShader.Bind();           
            lamp.scaling = glm::scale(glm::mat4(1.0), glm::vec3(0.05, 0.05, 0.05));
            lamp.translation = glm::translate(glm::mat4(1.0), (glm::vec3)pointLights[0].position);
            lampShader.SetUniform<glm::mat4 *>("model", &lamp.matModel);
            lampShader.SetUniform<glm::mat4 *>("view", &view);
            lampShader.SetUniform<glm::mat4 *>("proj", &projpersp);
            lampShader.SetUniform<vec3>("color", pointLights[0].diffusecolor);


            lamp.Draw(lampShader);

            rotation = {
                glm::rotate(
                    glm::rotate(
                        glm::rotate(
                            glm::mat4(1.0), glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f)),
                        glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f)),
                    glm::radians(rotate.x), glm::vec3(1.0f, 0.0f, 0.0f)) };

            scaling = glm::scale(glm::mat4(1.0), scale);
            translation = glm::translate(glm::mat4(1.0), translate);

            cube.scaling = scaling;
            cube.rotation = rotation;
            cube.translation = translation;
            view = glm::lookAt(cam.Camera_Position, cam.Camera_Position + cam.Camera_Facing_Direction * cam.Camera_Target_distance, cam.Camera_Up);
            projpersp = glm::perspective(glm::radians(perspective_fov), aspect_ratio, near_point, far_point);

            cubeShader.Bind();
            cubeShader.SetUniform<glm::mat4*>("model", &cube.matModel);
            cubeShader.SetUniform<glm::mat4*>("view", &view);
            cubeShader.SetUniform<glm::mat4*>("proj", &projpersp);
            cubeShader.SetUniform<vec3>("camPos", (vec3)cam.Camera_Position);
            cubeShader.SetUniform<vec3>("ambientLight", AmbientLight);


            cubeShader.SetUniform<vec3>("dirLight.direction", directionLight.direction);
            cubeShader.SetUniform<float>("dirLight.intensity", directionLight.intensity);
            cubeShader.SetUniform<vec3>("dirLight.diffusecolor", directionLight.diffusecolor);
            cubeShader.SetUniform<vec3>("dirLight.specularcolor", directionLight.specularcolor);

            cubeShader.SetUniform<vec3>("pointLights[0].position", pointLights[0].position);
            cubeShader.SetUniform<float>("pointLights[0].intensity", pointLights[0].intensity);
            cubeShader.SetUniform<vec3>("pointLights[0].diffusecolor", pointLights[0].diffusecolor);
            cubeShader.SetUniform<vec3>("pointLights[0].specularcolor", pointLights[0].specularcolor);
            cubeShader.SetUniform<float>("pointLights[0].constant", pointLights[0].constant);
            cubeShader.SetUniform<float>("pointLights[0].linear", pointLights[0].linear);
            cubeShader.SetUniform<float>("pointLights[0].quadratic", pointLights[0].quadratic);

            cubeShader.SetUniform<float>("material.shininess", cubeMaterial.shininess);
            cubeShader.SetUniform<vec3>("material.specularColor", cubeMaterial.specularColor);
            cubeShader.SetUniform<float>("material.ambientStrength", cubeMaterial.AmbientStrength);
            cubeShader.SetUniform<float>("material.diffuseStrength", cubeMaterial.DiffuseStrength);
            cubeShader.SetUniform<float>("material.specularStrength", cubeMaterial.SpecularStrength);

            cube.Draw(cubeShader);


            glfwSwapBuffers(mainWin);

            glfwMakeContextCurrent(imguiWin);
            {
                glClear(GL_COLOR_BUFFER_BIT);

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                {
                    static float f = 0.0f;
                    static int counter = 0;

                    ImGui::Begin("Controls");

                    ImGui::SliderFloat3("translate", &translate.x, -5.0f, 2.0f);

                    ImGui::SliderFloat3("scale", &scale.x, -3.0f, 3.0f);
                    ImGui::SliderFloat3("rotate", &rotate.x, -180.0f, 180.0f);
                    //ImGui::SliderFloat3("color", &lightColor.x, 0.0f, 1.0f);
                    //ImGui::SliderFloat("near point", &near_point, 0.0f, 5.0f);
                    ImGui::SliderFloat3("pointLight color ambient", &AmbientLight.x, 0.0f, 1.0f);
                    ImGui::SliderFloat3("pointLight color diffused", &pointLights[selected_light].diffusecolor.x, 0.0f, 1.0f);
                    ImGui::SliderFloat3("pointLight color specular", &pointLights[selected_light].specularcolor.x, 0.0f, 1.0f);
                    ImGui::Text("selected Lamp :%d", selected_light);
                    ImGui::Text("position of light is %.3f %.3f %.3f", pointLights[selected_light].position.x, pointLights[selected_light].position.y, pointLights[selected_light].position.z);

                    //ImGui::SliderFloat3("rotate", &light.direction.x, 0.0f, 1.0f);
                    //ImGui::SliderFloat3("Specular Reflectivity", &material.specular.x, 0.0f, 1.0f);
                    ImGui::SliderFloat("Ambient Strength", &cubeMaterial.AmbientStrength, 0.0f, 1.0f);
                    ImGui::SliderFloat("Diffuse Strength", &cubeMaterial.DiffuseStrength, 0.0f, 1.0f);
                    ImGui::SliderFloat("Specular Strength", &cubeMaterial.SpecularStrength, 0.0f, 1.0f);
                    ImGui::SliderFloat("liear term", &pointLights[selected_light].linear, 0.001f, 0.7f);
                    ImGui::SliderFloat("quadratic term", &pointLights[selected_light].quadratic, 0.000007f, 1.8f);
                    ImGui::Text("directed light control terms:");
                    ImGui::SliderFloat3("dirlight direction", &directionLight.direction.x, 0.001f, 0.7f);

                    //ImGui::SliderFloat("Shineness", &material.shininess, 0.0f, 512.0f);

                    ImGui::SliderFloat("far point", &far_point, 0.0f, 200.0f);
                    ImGui::SliderFloat("POV", &perspective_fov, 0.0f, 180.0f);

                    ImGui::Text("aspect Ratio : %f", aspect_ratio);
                    ImGui::Text("value of facing vector is %.3f %.3f %.3f", cam.Camera_Facing_Direction.x, cam.Camera_Facing_Direction.y, cam.Camera_Facing_Direction.z);
                    ImGui::Text("value of pitch = %.3f yaw = %.3f roll = %.3f", glm::degrees(cam.getAngles().m_pitch), glm::degrees(cam.getAngles().m_yaw), glm::degrees(cam.getAngles().m_roll));
                    ImGui::Text("value of up vector is %.3f %.3f %.3f", cam.Camera_Up.x, cam.Camera_Up.y, cam.Camera_Up.z);
                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                    ImGui::End();

                    ImGui::Begin("Information");
                    ImGui::Text("Model matrix: ");
                    ImGui::Text("glrenderer %s", glrenderer);
                    ImGui::Text("glversion %s", glversion);
                    ImGui::Text("glvendor %s", glvendor);

                    ImGui::Text("( %.3f ,%.3f, %.3f, %.3f )\n( %.3f ,%.3f, %.3f, %.3f )\n( %.3f, %.3f, %.3f, %.3f )\n( %.3f, %.3f, %.3f, %.3f )",
                                cube.matModel[0][0], cube.matModel[0][1], cube.matModel[0][2], cube.matModel[0][3],
                                cube.matModel[1][0], cube.matModel[1][1], cube.matModel[1][2], cube.matModel[1][3],
                                cube.matModel[2][0], cube.matModel[2][1], cube.matModel[2][2], cube.matModel[2][3],
                                cube.matModel[3][0], cube.matModel[3][1], cube.matModel[3][2], cube.matModel[3][3]);

                    ImGui::Text("View matrix: ");
                    ImGui::Text("( %.3f ,%.3f, %.3f, %.3f )\n( %.3f ,%.3f, %.3f, %.3f )\n( %.3f, %.3f, %.3f, %.3f )\n( %.3f, %.3f, %.3f, %.3f )",
                                view[0][0], view[0][1], view[0][2], view[0][3],
                                view[1][0], view[1][1], view[1][2], view[1][3],
                                view[2][0], view[2][1], view[2][2], view[2][3],
                                view[3][0], view[3][1], view[3][2], view[3][3]);

                    ImGui::Text("Projection matrix: ");
                    ImGui::Text("( %.3f ,%.3f, %.3f, %.3f )\n( %.3f ,%.3f, %.3f, %.3f )\n( %.3f, %.3f, %.3f, %.3f )\n( %.3f, %.3f, %.3f, %.3f )",
                                projpersp[0][0], projpersp[0][1], projpersp[0][2], projpersp[0][3],
                                projpersp[1][0], projpersp[1][1], projpersp[1][2], projpersp[1][3],
                                projpersp[2][0], projpersp[2][1], projpersp[2][2], projpersp[2][3],
                                projpersp[3][0], projpersp[3][1], projpersp[3][2], projpersp[3][3]);

                    ImGui::Text("cursor pos: %f , %f", mx, my);
                    ImGui::Text("captured: %d", captured);
                    ImGui::Text("ambient strength: %f", cubeMaterial.AmbientStrength);
                    ImGui::Text("diffuse strength: %f", cubeMaterial.DiffuseStrength);
                    ImGui::Text("specular strength: %f", cubeMaterial.SpecularStrength);

                      
                    //					ImGui::Text("Direction: %f, %f, %f", light.direction.x, light.direction.y, light.direction.z);
                    ImGui::End();                   
                }

                // Rendering
                ImGui::Render();
                int display_w, display_h;
                glfwGetFramebufferSize(imguiWin, &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                glfwSwapBuffers(imguiWin);
            }

        }
    }
    return 0;
}
