#include <stdio.h>
#include <stdlib.h>
//OpenGL
#include <GL/glew.h>

#include <glfw3.h>

GLFWwindow* window;
GLfloat waveTime = 0.5f;
GLfloat waveWidth = 0.6f;
GLfloat waveHeight = 1.0f;
GLfloat waveFreq = 0.001f;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

//C++
#include <vector>
#include <iostream>
#include "tools.hh"
#include "Wave.hh"

#define WIDTH_MESH 100
#define HEIGHT_MESH 100

#define WINDOWS_WIDTH 1300
#define WINDOWS_HEIGHT 700

std::vector<WaveParams> getWavesParams(const std::vector<Wave>& waves)
{
    std::vector<WaveParams> res;
    for (const auto& e : waves)
        res.push_back(e.wp);
    return res;
}

std::vector<WaveDirection> getWavesDirections(const std::vector<Wave>& waves)
{
    std::vector<WaveDirection> res;
    for (const auto& e : waves)
        res.push_back(e.wd);
    return res;
}

std::vector<Wave> generateWaves()
{
    std::vector<Wave> res;
    
    res.push_back(Wave(1.0f, 0.01f, 4.0f, 1.0f, 0.0f,1.0f));
    res.push_back(Wave(0.5f, 0.02f, 3.0f, 1.0f, 0.0f, 0.0f));
    res.push_back(Wave(0.1f, 0.015f, 2.0f, -0.1f, 0.0f, -0.2f));
    res.push_back(Wave(1.1f, 0.008f, 1.0f, -0.2f, 0.f, -0.1f));

    return res;
}

std::vector<GLuint> bindBuffers(const std::vector<unsigned short>& indices, 
                                const std::vector<float>& vertexDatas,
                                const std::vector<Wave>& waves)
{
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint vertexBuffer;

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, WIDTH_MESH * HEIGHT_MESH * 3 * sizeof(float), &vertexDatas[0], GL_STATIC_DRAW);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    GLuint shaderID = LoadShaders("../../src/vertexS", "../../src/fragmentS");
    GLuint matrixID = glGetUniformLocation(shaderID, "MVP");
    GLuint waveTimeID = glGetUniformLocation(shaderID, "waveTime");
    GLuint waveWidthID = glGetUniformLocation(shaderID, "waveWidth");
    GLuint waveHeightID = glGetUniformLocation(shaderID, "waveHeight");
    GLuint waveParamsID = glGetUniformLocation(shaderID, "waveParams");
    GLuint waveDirectionID = glGetUniformLocation(shaderID, "waveDirection");

    std::vector<GLuint> res;
    res.push_back(VertexArrayID); // 0
    res.push_back(vertexBuffer); // 1
    res.push_back(indexBuffer); // 2
    res.push_back(shaderID); // 3
    res.push_back(matrixID); // 4
    res.push_back(waveTimeID); // 5
    res.push_back(waveWidthID); // 6
    res.push_back(waveHeightID); // 7
    res.push_back(waveParamsID); // 8
    res.push_back(waveDirectionID); // 9
    //Always this order

    return res;
}

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
    return convertToArray(vertexs);
}

void initWindow()
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Water Simulation", NULL, NULL);
    if (window == NULL)
        exit(-1);

    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        exit(-1);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0, 0, 0, 0);
}

void renderScene(std::vector<GLuint> ids, 
                 glm::mat4& MVP, Control& ctrl, 
                 const std::vector<unsigned short>& indices,
                 const std::vector<Wave>& waves)
{

    std::vector<WaveParams> wps = getWavesParams(waves);
    std::vector<WaveDirection> wds = getWavesDirections(waves);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(ids[3]);
    MVP = ctrl.getMVP();
    ctrl.computeMVP();

    glUniformMatrix4fv(ids[4], 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(ids[5], waveTime);
    glUniform1f(ids[6], waveWidth);
    glUniform1f(ids[7], waveHeight);
    glUniform4fv(ids[8], 4 * NUM_OF_WAVES, (GLfloat*)(&wps[0]));
    glUniform2fv(ids[9], 2 * NUM_OF_WAVES, (GLfloat*)(&wds[0]));

    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(WIDTH_MESH *HEIGHT_MESH);
    glEnableVertexAttribArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindBuffer(GL_ARRAY_BUFFER, ids[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ids[2]);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
        );
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, vertexDatas.size() / 3);
    glDrawElements(GL_TRIANGLE_STRIP, static_cast<int>(indices.size()), GL_UNSIGNED_SHORT, (void*)0);
    glDisableVertexAttribArray(0);
    glfwSwapBuffers(window);
    glfwPollEvents();
}

int main()
{
    // TODO : Generate vertex in generate(),
    // Draw the mesh

    if (!glfwInit())
        return -1;

    initWindow();
    std::vector<Wave> waves = generateWaves();
    std::vector<unsigned short> indices;
    std::vector<float> vertexDatas = generateQuads(64, indices, WIDTH_MESH, HEIGHT_MESH);

    std::vector<GLuint> ids = bindBuffers(indices, vertexDatas, waves);

    glm::mat4 MVP;

    Control ctrl(WINDOWS_WIDTH, WINDOWS_HEIGHT);

    do
    {   
        renderScene(ids, MVP, ctrl, indices, waves);
        waveTime += waveFreq;
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &ids[1]);
    glDeleteProgram(ids[3]);
    
    return 0;
}
