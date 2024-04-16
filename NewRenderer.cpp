#include "NewRenderer.h"

NewRenderer::NewRenderer(Shader sceneShader, Shader textureShader, Camera* camera, unsigned int texture, unsigned int faceTexture)
    : sceneShader(sceneShader), textureShader(textureShader), camera(camera), textureID(texture), textureIDF(faceTexture) {
    initTextureRenderData();
    std::vector<float> faceVertices = {
        0.75f, 0.75f, 0.0f, 1.0f, 1.0f,  // Top Right
        0.75f, 0.25f, 0.0f, 1.0f, 0.0f,  // Bottom Right
        0.25f, 0.25f, 0.0f, 0.0f, 0.0f,  // Bottom Left
        0.25f, 0.75f, 0.0f, 0.0f, 1.0f   // Top Left
    };
    std::vector<unsigned int> faceIndices = { 0, 1, 3, 1, 2, 3 };
    setupFaceVAO(faceVertices, faceIndices);
}

NewRenderer::~NewRenderer() {
    glDeleteVertexArrays(1, &quadVAO_Texture);
    glDeleteBuffers(1, &quadVBO_Texture);
    glDeleteBuffers(1, &quadEBO_Texture);
    glDeleteVertexArrays(1, &quadVAO_Face);
    glDeleteBuffers(1, &quadVBO_Face);
    glDeleteBuffers(1, &quadEBO_Face);
}

void NewRenderer::Render() {
    RenderScene();
    TextureRenderer(textureID);
    FaceRenderer(textureIDF);
}

void NewRenderer::RenderScene() {
    sceneShader.use();
    sceneShader.setInt("texture1", 0);
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    sceneShader.setMat4("projection", projection);
    sceneShader.setMat4("view", view);
    glm::mat4 model = glm::mat4(1.0f);
    sceneShader.setMat4("model", model);
}

void NewRenderer::TextureRenderer(unsigned int texture) {
    textureShader.use();
    textureShader.setInt("texture1", 0);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)800 / (float)600, 0.1f, 200.0f);
    textureShader.setMat4("model1", model);
    textureShader.setMat4("view1", view);
    textureShader.setMat4("projection1", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(quadVAO_Texture);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void NewRenderer::FaceRenderer(unsigned int texture) {
    textureShader.use();
    textureShader.setInt("texture1", 0);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)800 / (float)600, 0.1f, 200.0f);
    textureShader.setMat4("model1", model);
    textureShader.setMat4("view1", view);
    textureShader.setMat4("projection1", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(quadVAO_Face);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void NewRenderer::initTextureRenderData() {
    std::vector<float> quadVertices = {
        2.75f, 0.75f, 0.0f, 1.0f, 1.0f,  // Top Right
        2.75f, 0.25f, 0.0f, 1.0f, 0.0f,  // Bottom Right
        2.25f, 0.25f, 0.0f, 0.0f, 0.0f,  // Bottom Left
        2.25f, 0.75f, 0.0f, 0.0f, 1.0f   // Top Left
    };
    std::vector<unsigned int> indices = { 0, 1, 3, 1, 2, 3 };
    glGenVertexArrays(1, &quadVAO_Texture);
    glGenBuffers(1, &quadVBO_Texture);
    glGenBuffers(1, &quadEBO_Texture);
    glBindVertexArray(quadVAO_Texture);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO_Texture);
    glBufferData(GL_ARRAY_BUFFER, quadVertices.size() * sizeof(float), quadVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO_Texture);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

void NewRenderer::setupFaceVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &quadVAO_Face);
    glGenBuffers(1, &quadVBO_Face);
    glGenBuffers(1, &quadEBO_Face);
    glBindVertexArray(quadVAO_Face);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO_Face);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO_Face);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}
