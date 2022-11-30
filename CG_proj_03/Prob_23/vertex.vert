#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vColor;

out vec3 fColor;

out vec3  fPos;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() 
{
    gl_Position = projection * view * model * vec4(vPos, 1.0);

    fPos = vec3(model * vec4(vPos, 1.0));
    fNormal = vNormal;

    fColor = vColor;
}