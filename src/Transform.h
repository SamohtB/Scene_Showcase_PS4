#pragma once
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

class Transform
{
public:
    Transform() : position(0.0f), rotation(0.0f), scaling(1.0f) {}

    void setPosition(float x, float y, float z);
    void setPosition(glm::vec3 vector);
    void move(float x, float y, float z);
    glm::vec3 getPosition();

    void setRotation(float pitch, float yaw, float roll);
    void setRotation(glm::vec3 vector);
    void rotate(float pitch, float yaw, float roll);
    glm::vec3 getRotation();

    void setScale(float x, float y, float z);
    void setScale(glm::vec3 vector);
    void scale(float scale);
    glm::vec3 getScale();

    glm::mat4 getTransformMatrix();

private:
    glm::vec3 position;
    glm::vec3 scaling;
    glm::vec3 rotation;
};