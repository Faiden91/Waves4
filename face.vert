#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model1;
uniform mat4 view1;
uniform mat4 projection1;

out vec2 TexCoord;

void main() {
    // Transform the position from model space to clip space
    gl_Position = projection1 * view1 * model1 * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
