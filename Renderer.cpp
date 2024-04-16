#include "Renderer.h"


//Shader sceneShader("shader.vert", "shader.frag");
//Shader textureShader("texture.vert", "texture.frag");

// Constructor implementation
Renderer::Renderer(Shader sceneShader, Shader textureShader, Camera* camera, unsigned int texture)
    : sceneShader(sceneShader), textureShader(textureShader), camera(camera), textureID(texture) {
    // Initialization code...
}


// Render method implementation
void Renderer::Render() {
    RenderScene();
    TextureRenderer(textureID);
    FaceRenderer(textureID);
}

void Renderer::RenderScene() {
    //Shader sceneShader("shader.vert", "shader.frag");
    sceneShader.use(); // Use the shader program for 3D rendering
    sceneShader.setInt("texture1", 0);

    // Set the camera-related uniforms
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    sceneShader.setMat4("projection", projection);
    sceneShader.setMat4("view", view);

    // Assuming model matrix is part of your object's properties, this is a general set up.
    // Iterate over each object in your scene and set its model matrix.
    glm::mat4 model = glm::mat4(1.0f); // Example: Identity matrix for model, meaning no transformation
    sceneShader.setMat4("model", model);



    // Here you'd typically have your drawing calls, like glBindVertexArray, glDrawArrays/Elements, etc.
    // For each object in your scene.
}


void Renderer::TextureRenderer(unsigned int texture) {
    if (quadVAO == 0) {
        initTextureRenderData();
        //initFaceTRData();
    }
    //Shader textureShader("texture.vert", "texture.frag");
    // Use the texture-specific shader
    textureShader.use();

    // Corrected to use GL_TEXTURE0 for simplicity and consistency
    textureShader.setInt("texture1", 0);

    // Setting transformation uniforms
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix or your model transform
    glm::mat4 view = camera->GetViewMatrix(); // Camera view matrix
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)800 / (float)600, 0.1f, 200.0f); // Projection matrix

    // Send matrices to the shader
    textureShader.setMat4("model1", model);
    textureShader.setMat4("view1", view);
    textureShader.setMat4("projection1", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Render quad using indices
    glBindVertexArray(0);
}


void Renderer::initTextureRenderData() {
    float quadVertices[] = {
        // Positions       // Texture Coords
        1.75f,  0.75f, 0.0f, 1.0f, 1.0f, // Top Right
        1.75f,  0.25f, 0.0f, 1.0f, 0.0f, // Bottom Right
        1.25f,  0.25f, 0.0f, 0.0f, 0.0f, // Bottom Left
        1.25f,  0.75f, 0.0f, 0.0f, 1.0f  // Top Left
    };

    unsigned int indices[] = {
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Renderer::FaceRenderer(unsigned int texture) {
    if (quadVAO == 0) {
        //initTextureRenderData();
        initFaceTRData();
    }
    //Shader textureShader("texture.vert", "texture.frag");
    // Use the texture-specific shader
    textureShader.use();

    // Corrected to use GL_TEXTURE0 for simplicity and consistency
    textureShader.setInt("texture1", 0);

    // Setting transformation uniforms
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix or your model transform
    glm::mat4 view = camera->GetViewMatrix(); // Camera view matrix
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)800 / (float)600, 0.1f, 200.0f); // Projection matrix

    // Send matrices to the shader
    textureShader.setMat4("model1", model);
    textureShader.setMat4("view1", view);
    textureShader.setMat4("projection1", projection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Render quad using indices
    glBindVertexArray(0);
}

void Renderer::initFaceTRData() {
    // Using std::vector instead of a static array
    std::vector<float> quadVertices = {
        // Positions       // Texture Coords
        0.75f,  0.75f, 0.0f, 1.0f, 1.0f, // Top Right
        0.75f,  0.25f, 0.0f, 1.0f, 0.0f, // Bottom Right
        0.25f,  0.25f, 0.0f, 0.0f, 0.0f, // Bottom Left
        0.25f,  0.75f, 0.0f, 0.0f, 1.0f  // Top Left
    };

    unsigned int indices[] = {
        0, 1, 3, // First Triangle
        1, 2, 3  // Second Triangle
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glGenBuffers(1, &quadEBO);

    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    // Use vector's data pointer and size for buffer data
    glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), quadVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}