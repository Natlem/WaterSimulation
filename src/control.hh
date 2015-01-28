#pragma once
//OpenGL
#include <GL/glew.h>

#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

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
    Vertex(const Vertex& v) = default;
};

class Control
{
    public:
        Control(int widthPixel, int heightPixel) : widthPixel(widthPixel), heightPixel(heightPixel){};
        void computeMVP(bool debug = false);
        glm::mat4 getProjMatrix();
        glm::mat4 getViewMatrix();
        glm::mat4 getModMatrix();
        glm::mat4 getMVP();
        glm::vec3 position = glm::vec3(14, 10, 3);
    private:
        int widthPixel;
        int heightPixel;
        glm::mat4 proj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(
                    glm::vec3(14, 10, 3),
                    glm::vec3(10, 0, 0),
                    glm::vec3(0, 1, 0)
                    );
        glm::mat4 mod = glm::mat4(1.0f);

        float horizontalAngle = 3.14f;

        float verticalAngle = 3.14f;

        float initialFoV = 45.0f;

        float speed = 3.0f; // 3 units / second
        float mouseSpeed = 0.005f;
};