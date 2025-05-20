#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 Colour;

void main() {
    // Muliplication from right to left as MatrixSpace
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Colour = vec4(aColour, 1.0f);
}
