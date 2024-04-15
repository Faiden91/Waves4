#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <GL/glew.h>

class TextureLoader {
public:
    static unsigned int LoadTexture(const char* filePath);
};

#endif // TEXTURELOADER_H
