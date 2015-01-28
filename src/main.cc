#include <stdio.h>
#include <stdlib.h>
//OpenGL
#include <GL/glew.h>

#include <glfw3.h>

GLFWwindow* window;
GLfloat waveTime = 0.5f;
GLfloat waveWidth = 0.6f;
GLfloat waveHeight = 1.0f;
GLfloat waveFreq = 0.003f;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hh"

//C++
#include <vector>
#include <iostream>
#include "control.hh"
#include "Wave.hh"

#define WIDTH_MESH 100
#define HEIGHT_MESH 100

#define WINDOWS_WIDTH 1300
#define WINDOWS_HEIGHT 700

const float g_water_plane_length = 128.0f;

void optionParser(bool& mesh, int& wave, int argc, char** argv)
{
    if (argc > 3)
    {
        std::cout << " Usage : ./syn mode typeOfWave" << std::endl;
        std::cout << " The option parser is really basic, do no try anything tricky" << std::endl;
        std::cout << " There are 3 types of waves" << std::endl;
        std::cout << " By defaut, mesh is enabled" << std::endl;
        std::cout << " Example displaying only mesh : ./syn mesh 1" << std::endl;
        std::cout << " Example display using color : ./syn nomesh 1" << std::endl;
        std::cout << " Example displaying only mesh : ./syn 1" << std::endl;
        exit(1);
    }
    if (argc == 3)
    {
        std::string argv1 = argv[1];
        std::string argv2 = argv[2];
        if (argv1 == "mesh")
            mesh = true;
        else if (argv1 == "nomesh")
            mesh = false;
        else
        {
            std::cout << " Usage : ./syn mode typeOfWave" << std::endl;
            std::cout << " The option parser is really basic, do no try anything tricky" << std::endl;
            std::cout << " There are 3 types of waves" << std::endl;
            std::cout << " By defaut, mesh is enabled" << std::endl;
            std::cout << " Example displaying only mesh : ./syn mesh 1" << std::endl;
            std::cout << " Example display using color : ./syn nomesh 1" << std::endl;
            std::cout << " Example displaying only mesh : ./syn 1" << std::endl;
            exit(1);
        }
        if (argv2 == "1")
            wave = 1;
        else if (argv2 == "2")
            wave = 2;
        else if (argv2 == "3")
            wave = 3;
        else
        {
            std::cout << " Usage : ./syn mode typeOfWave" << std::endl;
            std::cout << " The option parser is really basic, do no try anything tricky" << std::endl;
            std::cout << " There are 3 types of waves" << std::endl;
            std::cout << " By defaut, mesh is enabled" << std::endl;
            std::cout << " Example displaying only mesh : ./syn mesh 1" << std::endl;
            std::cout << " Example display using color : ./syn nomesh 1" << std::endl;
            std::cout << " Example displaying only mesh : ./syn 1" << std::endl;
            exit(1);
        }
    }
    if (argc == 2)
    {
        std::string argv2 = argv[1];
        if (argv2 == "1")
            wave = 1;
        else if (argv2 == "2")
            wave = 2;
        else if (argv2 == "3")
            wave = 3;
        else
        {
            std::cout << " Usage : ./syn mode typeOfWave" << std::endl;
            std::cout << " The option parser is really basic, do no try anything tricky" << std::endl;
            std::cout << " There are 3 types of waves" << std::endl;
            std::cout << " By defaut, mesh is enabled" << std::endl;
            std::cout << " Example displaying only mesh : ./syn mesh 1" << std::endl;
            std::cout << " Example display using color : ./syn nomesh 1" << std::endl;
            std::cout << " Example displaying only mesh : ./syn 1" << std::endl;
            exit(1);
        }
    }
    if (argc == 1)
    {
            std::cout << " Usage : ./syn mode typeOfWave" << std::endl;
            std::cout << " The option parser is really basic, do no try anything tricky" << std::endl;
            std::cout << " There are 3 types of waves" << std::endl;
            std::cout << " By defaut, mesh is enabled" << std::endl;
            std::cout << " Example displaying only mesh : ./syn mesh 1" << std::endl;
            std::cout << " Example display using color : ./syn nomesh 1" << std::endl;
            std::cout << " Example displaying only mesh : ./syn 1" << std::endl;
            exit(1);
    }
}

std::vector<float> getWavesSpeeds(const std::vector<Wave>& waves)
{
    std::vector<float> res;
    for (const auto& e : waves)
        res.push_back(e.wp.speed);
    return res;
}

std::vector<float> getWavesAmplitude(const std::vector<Wave>& waves)
{
    std::vector<float> res;
    for (const auto& e : waves)
        res.push_back(e.wp.amplitude);
    return res;
}

std::vector<float> getWavesWaveLength(const std::vector<Wave>& waves)
{
    std::vector<float> res;
    for (const auto& e : waves)
        res.push_back(e.wp.waveLength);
    return res;
}

std::vector<float> getWavesSteepness(const std::vector<Wave>& waves)
{
    std::vector<float> res;
    for (const auto& e : waves)
        res.push_back(e.wp.steepness);
    return res;
}

std::vector<float> getWavesDirections(const std::vector<Wave>& waves)
{
    std::vector<float> res;
    for (const auto& e : waves)
    {
        res.push_back(e.wd.x);
        res.push_back(e.wd.z);
    }
    return res;
}


std::vector<Wave> generateWaves()
{
    std::vector<Wave> res;
    
    res.push_back(Wave(1.0f, 0.05f, 4.0f, 1.0f, 0.0f,1.0f));
    res.push_back(Wave(0.5f, 0.2f, 3.0f, 1.0f, 0.0f, 0.0f));
    res.push_back(Wave(0.1f, 0.02f, 2.0f, -0.1f, 0.0f, -0.2f));
    res.push_back(Wave(1.1f, 0.5f, 1.0f, -0.2f, 0.f, -0.1f));

    return res;
}



std::vector<GLuint> bindBuffers(const std::vector<unsigned short>& indices, 
                                const std::vector<float>& vertexDatas)
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

    //GLuint shaderID = LoadShaders("../../src/vertexS", "../../src/fragmentS");
    GLuint shaderID = LoadCompileLinkShader2("../../src/vertexS", "../../src/fragmentS");
    GLuint matrixID = glGetUniformLocation(shaderID, "MVP");
    GLuint waveTimeID = glGetUniformLocation(shaderID, "waveTime");
    GLuint waveNumberID = glGetUniformLocation(shaderID, "waveNumbers");
    GLuint waveSpeedID = glGetUniformLocation(shaderID, "waveSpeeds");
    GLuint waveAmpID = glGetUniformLocation(shaderID, "waveAmplitudes");
    GLuint waveLengthID = glGetUniformLocation(shaderID, "waveLengths");
    GLuint waveSteepID = glGetUniformLocation(shaderID, "waveSteepnesss");
    GLuint waveDirectionID = glGetUniformLocation(shaderID, "waveDirection");
    GLuint wavePlaneLengthID = glGetUniformLocation(shaderID, "wavePlaneLength");
    GLuint waveCenter = glGetUniformLocation(shaderID, "waveCenter");
    GLuint MID = glGetUniformLocation(shaderID, "M");
    GLuint VID = glGetUniformLocation(shaderID, "V");
    GLuint PID = glGetUniformLocation(shaderID, "P");
    GLuint lightPosID = glGetUniformLocation(shaderID, "lightPos");
    GLuint lightColorID = glGetUniformLocation(shaderID, "lightColor");
    GLuint cameraPosId = glGetUniformLocation(shaderID, "cameraPos");
    GLuint typeofWaveID = glGetUniformLocation(shaderID, "typeOfWave");

    std::vector<GLuint> res;
    res.push_back(VertexArrayID); // 0
    res.push_back(vertexBuffer); // 1
    res.push_back(indexBuffer); // 2
    res.push_back(shaderID); // 3
    res.push_back(matrixID); // 4
    res.push_back(waveTimeID); // 5
    res.push_back(waveNumberID); // 6
    res.push_back(waveSpeedID); // 7
    res.push_back(waveAmpID); // 8
    res.push_back(waveLengthID); // 9
    res.push_back(waveSteepID); // 10
    res.push_back(waveDirectionID); // 11
    res.push_back(wavePlaneLengthID); // 12
    res.push_back(waveCenter); // 13
    res.push_back(MID); // 14
    res.push_back(VID); // 15
    res.push_back(PID); // 16
    res.push_back(lightPosID); // 17
    res.push_back(lightColorID); // 18
    res.push_back(cameraPosId); // 19
    res.push_back(typeofWaveID); // 20


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

std::vector<float> generateQuads(int number, std::vector<unsigned short>& indices, int width, int height, std::vector<float>& p)
{
    std::vector<Vertex> vertexs;
    float startX = 0.0f;
    float startZ = 0.0f;
    float x1 = -1.0f;
    float y1 = 0.0f;
    float z1 = 0.0f;
    float step = 1.0f;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            x1 += step;
            vertexs.push_back(Vertex(x1,y1,z1));
        }
        x1 = 0 - step;
        z1 += step;
    }
    fillIndices(indices, width, height, vertexs.size());
    p.push_back(((width - 1) * step) / 2);
    p.push_back(((height - 1) * step) / 2);
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
                 const std::vector<Wave>& waves, const std::vector<float>& center,
                 const bool& mesh, const int typeOfWave)
{

    //std::vector<WaveParams> wps = getWavesParams(waves);
    //std::vector<WaveDirection> wds = getWavesDirections(waves);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(ids[3]);
    MVP = ctrl.getMVP();
    glm::mat4 M = ctrl.getModMatrix();
    glm::mat4 V = ctrl.getViewMatrix();
    glm::mat4 P = ctrl.getProjMatrix();
    ctrl.computeMVP();

    std::vector<float> speeds = getWavesSpeeds(waves);
    std::vector<float> amps = getWavesAmplitude(waves);
    std::vector<float> waveL = getWavesWaveLength(waves);
    std::vector<float> waveS = getWavesSteepness(waves);
    std::vector<float> waveD = getWavesDirections(waves);

    glUniformMatrix4fv(ids[4], 1, GL_FALSE, &MVP[0][0]);
    glUniform1f(ids[5], waveTime);
    glUniform1f(ids[6], static_cast<float>(waves.size()));
    glUniform1fv(ids[7], static_cast<int>(waves.size()), &speeds[0]);
    glUniform1fv(ids[8], static_cast<int>(waves.size()), &amps[0]);
    glUniform1fv(ids[9], static_cast<int>(waves.size()), &waveL[0]);
    glUniform1fv(ids[10], static_cast<int>(waves.size()), &waveS[0]);
    glUniform1fv(ids[11], static_cast<int>(waveD.size()), &waveD[0]);
    glUniform1f(ids[12], g_water_plane_length);
    glUniform1fv(ids[13], 2, &center[0]);
    glUniformMatrix4fv(ids[14], 1, GL_FALSE, &M[0][0]);
    glUniformMatrix4fv(ids[15], 1, GL_FALSE, &V[0][0]);
    glUniformMatrix4fv(ids[16], 1, GL_FALSE, &P[0][0]);

    //LIGHT Pos
    glUniform3f(ids[17], center[0], 50, center[1]);

    //LIGHT Color
    glUniform3f(ids[18], 1, 1, 1);

    //Camera Pos
    glUniform3f(ids[19], ctrl.position.x, ctrl.position.y, ctrl.position.z);

    glUniform1i(ids[20], typeOfWave);


    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(WIDTH_MESH *HEIGHT_MESH);
    glEnableVertexAttribArray(0);
    if (mesh)
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

int main(int argc, char** argv)
{
    // TODO : Generate vertex in generate(),
    // Draw the mesh

    if (!glfwInit())
        return -1;

    bool meshM = true;
    int typeOfWave = 1;

    optionParser(meshM, typeOfWave, argc, argv);

    initWindow();
    std::vector<Wave> waves = generateWaves();
    std::vector<unsigned short> indices;
    std::vector<float> center;
    std::vector<float> vertexDatas = generateQuads(64, indices, WIDTH_MESH, HEIGHT_MESH, center);

    std::vector<GLuint> ids = bindBuffers(indices, vertexDatas);

    glm::mat4 MVP;

    Control ctrl(WINDOWS_WIDTH, WINDOWS_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    do
    {   
        renderScene(ids, MVP, ctrl, indices, waves,center, meshM, typeOfWave);
        waveTime += waveFreq;
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &ids[1]);
    glDeleteBuffers(1, &ids[2]);
    glDeleteProgram(ids[3]);
    
    return 0;
}
