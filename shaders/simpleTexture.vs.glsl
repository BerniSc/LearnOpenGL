#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTexCoord;

out vec4 colour;
out vec2 TexCoord;

uniform mat4 transformM;

void main() {
    gl_Position = transformM * vec4(aPos, 1.0);
    colour = vec4(aColour, 1.0);
    TexCoord = aTexCoord;
}
