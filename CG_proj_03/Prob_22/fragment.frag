#version 330 core

in vec3 fColor;
out vec4 fragColor;

in vec3 fNormal;
in vec3 fPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform vec3 ambientLight;
uniform int shininess;

uniform int isOn;

void main()
{
    vec3 ambient = ambientLight * lightColor;

    vec3 norm = normalize(fNormal);
    vec3 lightDir = normalize(lightPos - fPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specStrength = 1;
    vec3 viewDir = normalize(viewPos - fPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specStrength * spec * lightColor;

    vec3 result = ambient * fColor;
    
    if (isOn == 1)
        result = (ambient + diffuse + specular) * fColor;

    fragColor = vec4(result , 1.0);
}