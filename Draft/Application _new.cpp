#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <unordered_map>

#include "graphics/Renderer.h"
#include "graphics/IndexBuffer.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBufferLayout.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Controls.h"
#include "graphics/VertexIndexBufferCouple.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "game/Chunk.hpp"
#include "game/Map.hpp"

#include "Minecraft.hpp"

constexpr auto W_WIDTH = 1024;
constexpr auto W_HEIGHT = 768;
constexpr auto FULLSCREEN = false;

int main(void)
{
    GLFWwindow* window;

    /* Initialize GLFW */
    if (!glfwInit())
    {
        std::cout << "[Initialisation error] Unable to initialise GLFW" << std::endl;
        ASSERT(false);
        return -1;
    }
        
    /* Setting profile to CORE to use vertex arrays objects */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    if (FULLSCREEN)
        window = glfwCreateWindow(1920, 1280, "Minecraft OpenGL", glfwGetPrimaryMonitor(), NULL); // 4:3
    else
        window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "Minecraft OpenGL", NULL, NULL); // 4:3
    glfwSetWindowPos(window, 500, 200);

    if (!window)
    {
        std::cout << "[Initialisation error] Unable to create a window" << std::endl;
        glfwTerminate();
        ASSERT(false);
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Set the swap interval to make the render smooth */
    glfwSwapInterval(1);

    /* Initialising GLEW */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "[Initialisation error] Unable to initialise GLEW" << std::endl;
        ASSERT(false);
        return -1;
    }

    {
        /* GL enables */
        // Enable depth test
        GLCall(glEnable(GL_DEPTH_TEST));
        // Accept fragment if it closer to the camera than the former one
        GLCall(glDepthFunc(GL_LESS));
        
        // Cull face if its BEHIND - GL_FRONT is here to fix a strange bug
        GLCall(glEnable(GL_CULL_FACE));
        GLCall(glCullFace(GL_FRONT));
        
        // Change clear color to a blue sky
        GLCall(glClearColor(0.53, 0.81, 0.92, 1.0));

        Minecraft minecraft(window);

        /* Map and camera creation (will be moved later) */
        glm::vec3 camera_position(8, 15, 8);
        Control control(window, camera_position);

        Map map;

        /* Graphics part */
        VertexArray vao;
        vao.Bind();

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform1i("u_Texture", 0); // 0 = slot, default value

        Texture texture("res/textures/default_mc_textures.png");
        texture.Bind(); // default slot is 0

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
        {
            //minecraft.Update(window);
            renderer.Clear();

            control.UpdateInput(window);
            map.UpdateChunkPlayerPosition(control.GetCameraPosition());
            
            map.GenerateOneChunk();

            glm::mat4 proj = control.getProjectionMatrix();
            glm::mat4 view = control.getViewMatrix();
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

            glm::mat4 MVP = proj * view * model;
            shader.SetUniformMat4f("u_MVP", MVP);

            map.RenderAllNeededChunks(vao);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();

        }

    }
    glfwTerminate();

    return 0;
}
