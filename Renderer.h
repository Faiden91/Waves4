#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Shader.h" // Include the Shader class header
#include "TextureLoader.h"
#include "BSPMap.h"
#include <vector>

class Renderer {
public:
    Shader sceneShader;
    Shader textureShader;
    Camera* camera;
    unsigned int textureID; // Store the texture ID


    // Constructor
    //Renderer(Shader shader, Camera* camera, unsigned int texture);
    Renderer(Shader sceneShader, Shader textureShader, Camera* camera, unsigned int texture);

    // Render function
    void Render();
    void RenderScene();
    void TextureRenderer(unsigned int texture);


private:
    void initTextureRenderData();
    GLuint quadVAO = 0, quadVBO = 0, quadEBO = 0; // VAO and VBO for the quad
};

#endif // RENDERER_H
