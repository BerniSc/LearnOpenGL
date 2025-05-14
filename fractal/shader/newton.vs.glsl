// newton.vs.glsl

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aComplex;  // Complex plane coordinates

out vec2 complexCoord;

void main() {
    gl_Position = vec4(aPos, 1.0);
    complexCoord = aComplex;  // Pass complex coordinates to fragment shader
}
