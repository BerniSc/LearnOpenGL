#version 330 core
out vec4 FragColor;
in vec4 Colour;
void main() {
    gl_FragColor = Colour;
} 
