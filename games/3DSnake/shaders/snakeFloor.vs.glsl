#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aTexColour;

out vec2 TexCoord;
out vec4 TexColour;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 colour;

void main() {
    // Muliplication from right to left as MatrixSpace
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    TexColour = vec4(colour, 0.1f);
}
