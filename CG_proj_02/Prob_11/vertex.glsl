#version 330 core

in vec3 vPos;

uniform mat4 model;

void main() 
{
    gl_Position = vec4(vPos, 1.0);
}