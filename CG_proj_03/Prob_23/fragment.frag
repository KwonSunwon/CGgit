#version 330 core

in vec3 fColor;

in vec3 fNormal;
in vec3 fPos;

out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientLight = 0.5;
    vec3 ambient = ambientLight * lightColor;

    vec3 normalVector = normalize(fNormal);
    vec3 lightDir = normalize(lightPos - fPos);
    float diffuseLight = max(dot(normalVector, lightDir), 0.0);
    vec3 diffuse = diffuseLight * lightColor;

    int specularStrength = 128;
    vec3 viewDir = normalize(viewPos - fPos);
    vec3 reflectDir = reflect(-lightDir, normalVector);
    float specularLight = pow(max(dot(viewDir, reflectDir), 0.0), specularStrength);
    vec3 specular = specularLight * lightColor;

    vec3 result = (ambient + diffuse + specular) * fColor;
    
    fragColor = vec4(result , 1.0);
}