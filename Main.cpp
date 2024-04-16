#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Camera.h"
#include "TextureLoader.h"
#include "Renderer.h"
#include "InputManager.h"
#include "BSPMap.h"
#include "NewRenderer.h"


int main(void) {
    GLFWwindow* window;

    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(640, 480, "Waves Engine", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Shader initialization. I wanted to do this in my renderer class but OpenGL didn't like that.
    Shader sceneShader("shader.vert", "shader.frag");
    Shader textureShader("texture.vert", "texture.frag");
    // Camera initialization (positioned at (0,0,3) looking down -Z axis in this case)
    Camera myCamera(glm::vec3(1.25f, 0.0f, 3.0f));
    unsigned int textureID = TextureLoader::LoadTexture("redtexture.jpg");
    unsigned int textureIDF = TextureLoader::LoadTexture("YellowTexture.jpg");
    // Renderer initialization
    NewRenderer myRenderer(sceneShader, textureShader, &myCamera, textureID, textureIDF);

    // Assuming camera has already been created
    InputManager inputManager(&myCamera);

    // After creating the GLFWwindow* window
    inputManager.SetupCallbacks(window);

    BSPMap myMap;
    myMap.LoadAllLumps("MYFIRSTMAP.bsp");

    float lastFrame = 0.0f; // Time of last frame
    float deltaTime = 0.0f; // Time between current frame and last frame

    glEnable(GL_DEPTH_TEST);
    // Render loop
    while (!glfwWindowShouldClose(window)) {

        float currentFrame = glfwGetTime(); // Get the current time as a float
        deltaTime = currentFrame - lastFrame; // Calculate deltaTime
        lastFrame = currentFrame; // Update lastFrame with the current time for the next iteration

        inputManager.ProcessKeyboard(window, deltaTime);
        // Clear the color buffer
        glClearColor(0.2f, 0.5f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        myRenderer.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
