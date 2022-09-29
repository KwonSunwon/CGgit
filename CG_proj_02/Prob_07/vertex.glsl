#version 330 core

in vec3 vPos;
in vec3 vColor;
out vec3 passColor;

uniform mat4 modelTransform;

void main() 
{
    gl_Position = modelTransform * vec4(vPos, 1.0);
    passColor = vColor;
}