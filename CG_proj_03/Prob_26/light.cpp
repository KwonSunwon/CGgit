#include "light.h"

Light::Light()
{
    colors.push_back(1.0f);
    colors.push_back(1.0f);
    colors.push_back(1.0f);
    ambientLight = glm::vec3(0.5f, 0.5f, 0.5f);
    shininess = 32;

    pos = glm::vec3(0.0f, 0.f, 2.0f);
}

void Light::setLight(GLuint shaderProgramID, glm::vec3 viewPos)
{
    glUniform3f(glGetUniformLocation(shaderProgramID, "lightColor"), colors[0], colors[1], colors[2]);
    glUniform3f(glGetUniformLocation(shaderProgramID, "lightPos"), pos.x, pos.y, pos.z);
    glUniform3f(glGetUniformLocation(shaderProgramID, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
    glUniform3f(glGetUniformLocation(shaderProgramID, "ambientLight"), ambientLight.x, ambientLight.y, ambientLight.z);
    glUniform1i(glGetUniformLocation(shaderProgramID, "shininess"), shininess);
}

void Light::update()
{
    angle += 0.1f;

    pos.x = 4.0f * cos(glm::radians(angle));
    pos.z = 4.0f * sin(glm::radians(angle));
}