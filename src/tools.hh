#pragma once
//OpenGL
#include <GL/glew.h>

#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex
{
    float x;
    float y;
    float z;
    Vertex(float x, float y, float z) : x(x), y(y), z(z) {};
    Vertex() : x(0), y(0), z(0) {};
    Vertex& operator=(const Vertex& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    }
    Vertex& operator=(const Vertex&& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
        return *this;
    }
    Vertex::Vertex(const Vertex& v) = default;
};

class Control
{
    public:
        void computeMVP();
        glm::mat4 getProjMatrix();
        glm::mat4 getViewMatrix();
        glm::mat4 getModMatrix();
    private:
        glm::mat4 proj;
        glm::mat4 view;
        glm::mat4 mod;
        glm::vec3 position = glm::vec3(14, 10, 3);
        // Initial horizontal angle : toward -Z
        float horizontalAngle = 3.14f;
        // Initial vertical angle : none
        float verticalAngle = 0.0f;
        // Initial Field of View
        float initialFoV = 45.0f;

        float speed = 3.0f; // 3 units / second
        float mouseSpeed = 0.005f;
};