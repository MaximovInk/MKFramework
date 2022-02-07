#include "transform.h"
#include <glm/gtx/transform.hpp>

void MKEngine::transform::updateLocalMatrix()
{
    const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f),
        glm::radians(eulerRot.x),
        glm::vec3(1.0f, 0.0f, 0.0f));
    const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f),
        glm::radians(eulerRot.y),
        glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f),
        glm::radians(eulerRot.z),
        glm::vec3(0.0f, 0.0f, 1.0f));

    // Y * X * Z
    const glm::mat4 rotationMatrix = transformY * transformX * transformZ;

    modelMatrix = glm::translate(glm::mat4(1.0f), pos) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
    for (size_t i = 0; i < children.size(); i++)
    {
        children[i]->updateLocalMatrix();
    }
}

glm::mat4 MKEngine::transform::getGlobalMatrix()
{
    if (parent != NULL)
       return parent->getLocalMatrix() * getLocalMatrix();
    return getLocalMatrix();
}

glm::mat4 MKEngine::transform::getLocalMatrix()
{
    return modelMatrix;
}

MKEngine::transform* MKEngine::transform::getRoot()
{
    if (parent != NULL)
        return parent->getRoot();

    return this;
}
