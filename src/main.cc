#include <stdio.h>
#include <stdlib.h>
//OpenGL
#include <GL/glew.h>

#include <glfw3.h>

GLFWwindow* window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

//C++
#include <vector>
#include <iostream>
#include "tools.hh"

#define WIDTH_MESH 10
#define HEIGHT_MESH 10

std::vector<float> convertToArray(const std::vector<Vertex>& v)
{
    std::vector<float> res;
    for (const auto& e : v)
    {
        res.push_back(e.x);
        res.push_back(e.y);
        res.push_back(e.z);
    }
    return res;
}

void fillIndices(std::vector<unsigned short>& v, int width, int height, size_t size)
{
    for (unsigned short i = 0; i < size; ++i)
    {
        if (!(i + width >= size))
        {

                v.push_back(i);
                v.push_back(i + width);
                if ((i+1) % width == 0 && i + width + 1 != size)
                    v.push_back(width * height);
        }
    }
}

std::vector<float> generateQuads(int number, std::vector<unsigned short>& indices, int width, int height)
{
    std::vector<Vertex> vertexs;
    float startX = 0.0f;
    float startZ = 0.0f;
    float x1 = -1.0f;
    float y1 = 0.0f;
    float z1 = 0.0f;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            x1 += 1.0f;
            vertexs.push_back(Vertex(x1,y1,z1));
        }
        x1 = -1.0f;
        z1 += 1.0f;
    }
    fillIndices(indices, width, height, vertexs.size());
    for (unsigned int i = 0; i < indices.size(); ++i)
    {
        std::cout << indices[i] << "|";
        if (indices[i] == height * width)
            std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << " VERTICES " << std::endl;
    for (unsigned int i = 0; i < vertexs.size(); ++i)
    {
        std::cout << vertexs[i].x <<  " " << vertexs[i].z << "    ";
        if ((i + 1) % width == 0)
            std::cout << std::endl;
    }
    return convertToArray(vertexs);
}

int main()
{
    // TODO : Generate vertex in generate(),
    // Draw the mesh

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "Water Simulation", NULL, NULL);
    if (window == NULL)
        return -1;

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }


    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0, 0, 0, 0);


    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    std::vector<unsigned short> indices;
    std::vector<float> vertexDatas = generateQuads(64, indices, WIDTH_MESH, HEIGHT_MESH);
    std::cout << vertexDatas.size() << std::endl;
    std::cout << indices.size() << std::endl;

    GLuint vertexBuffer;

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, WIDTH_MESH * HEIGHT_MESH *3 * sizeof(float), &vertexDatas[0], GL_STATIC_DRAW);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);



    GLuint shaderID = LoadShaders("vertexS", "fragmentS");
    GLuint matrixID = glGetUniformLocation(shaderID, "MVP");
    glm::mat4 proj = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

    glm::mat4 view = glm::lookAt(
        glm::vec3(14, 10, 3),
        glm::vec3(10, 0, 0),
        glm::vec3(0, 1, 0)
        );

    glm::mat4 mod = glm::mat4(1.0f);

    glm::mat4 MVP = proj * view * mod;

    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderID);
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
        glEnable(GL_PRIMITIVE_RESTART);
        glPrimitiveRestartIndex(WIDTH_MESH *HEIGHT_MESH);
        glEnableVertexAttribArray(0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
            );  
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexDatas.size() / 3);
        glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_SHORT, (void*)0);
        glDisableVertexAttribArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteProgram(shaderID);

    return 0;
}