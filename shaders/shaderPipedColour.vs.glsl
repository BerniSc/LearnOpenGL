#version 330 core
layout (location = 0) in vec3 aPos;
out vec4 vertexColor;                   // specify a color that gets output#ed to the fragment shader -> Shader-Shader communication
void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexColor = vec4(aPos, 1.0);
}
