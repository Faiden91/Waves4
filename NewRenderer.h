#ifndef NEWRENDERER_H
#define NEWRENDERER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Shader.h"
#include "TextureLoader.h"
#include <vector>

class NewRenderer {
public:
    Shader sceneShader;
    Shader textureShader;
    Camera* camera;
    unsigned int textureID;
    unsigned int textureIDF;

    NewRenderer(Shader sceneShader, Shader textureShader, Camera* camera, unsigned int texture, unsigned int faceTexture);
    ~NewRenderer();

    void Render();
    void RenderScene();
    void TextureRenderer(unsigned int texture);
    void FaceRenderer(unsigned int faceTexture);

private:
    void initTextureRenderData();
    void setupFaceVAO(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    GLuint quadVAO_Texture = 0, quadVBO_Texture = 0, quadEBO_Texture = 0;
    GLuint quadVAO_Face = 0, quadVBO_Face = 0, quadEBO_Face = 0;
};

#endif // NEWRENDERER_H
