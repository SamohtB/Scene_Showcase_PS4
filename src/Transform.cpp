#include "Transform.h"


void Transform::setPosition(float x, float y, float z) 
{
    position = glm::vec3(x, y, z);
}

void Transform::setPosition(glm::vec3 vector) 
{
    position = vector;
}

void Transform::move(float x, float y, float z) 
{
    position += glm::vec3(x, y, z);
}

glm::vec3 Transform::getPosition() 
{
    return position;
}

void Transform::setRotation(float pitch, float yaw, float roll) 
{
    rotation = glm::vec3(pitch, yaw, roll);
}

void Transform::setRotation(glm::vec3 vector) 
{
    rotation = vector;
}

void Transform::rotate(float pitch, float yaw, float roll) 
{
    rotation += glm::vec3(pitch, yaw, roll);
}

glm::vec3 Transform::getRotation() 
{
    return rotation;
}

void Transform::setScale(float x, float y, float z) 
{
    scaling = glm::vec3(x, y, z);
}

void Transform::setScale(glm::vec3 vector) 
{
    scaling = vector;
}

void Transform::scale(float scale) 
{
    scaling *= scale;
}

glm::vec3 Transform::getScale() 
{
    return scaling;
}

glm::mat4 Transform::getTransformMatrix() 
{
    glm::mat4 transform = glm::mat4(1.0f);

    transform = glm::translate(transform, position);

    transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1, 0, 0));
    transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0, 1, 0));
    transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0, 0, 1));

    transform = glm::scale(transform, scaling);

    return transform;
}
