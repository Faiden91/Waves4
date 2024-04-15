#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
uniform sampler2D texture2; // Updated uniform name to match the method

void main() {
    FragColor = texture(texture2, TexCoord);
}
