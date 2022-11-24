#version 330 core

in vec3 fColor;
out vec4 fragColor;

uniform float alpha;

void main()
{
    fragColor = vec4(fColor , alpha);
}