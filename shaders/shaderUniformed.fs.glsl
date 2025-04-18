#version 330 core
out vec4 FragColor;
// Declared but not used uniform will SILENTLY be removed!!!! PLEEEEASE Remeber this. I see horrible debugging coming ;-(
uniform vec4 uniformedColor; // Set by OpenGL CPU code
void main() {
    FragColor = uniformedColor;
}   

