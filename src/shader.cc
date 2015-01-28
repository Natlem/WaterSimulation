#include "shader.hh"

GLuint LoadCompileLinkShader(std::string vertexPathSrc, std::string fragmentPathSrc)
{
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    std::string vertexSrc;
    std::ifstream vertexStream(vertexPathSrc, std::ios::in);
    if (vertexStream.is_open()) {
        std::string line = "";
        while (std::getline(vertexStream, line))
            vertexSrc += "\n" + line;
        vertexStream.close();
    }
    else {
        std::cout << "Cannot find verter shader source" << std::endl;
        return 0;
    }

    GLint compileResult = GL_FALSE;
    int logLength;

    // Compile Vertex Shader
    std::cout << "Compiling Vertex Shader" << std::endl;
    const char* vertexSrcC = vertexSrc.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSrcC, NULL);
    glCompileShader(vertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &compileResult);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::vector<char> errorMessage(logLength + 1);
        glGetShaderInfoLog(vertexShaderID, logLength, NULL, &errorMessage[0]);
        for (const auto& e : errorMessage)
            std::cout << e;
        std::cout << std::endl;
    }

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentSrc;
    std::ifstream fragStream(fragmentPathSrc, std::ios::in);
    if (fragStream.is_open()) {
        std::string line = "";
        while (std::getline(fragStream, line))
            fragmentSrc += "\n" + line;
        fragStream.close();
    }

    std::cout << "Compiling Fragment Shader" << std::endl;
    const char * fragSourceC = fragmentSrc.c_str();
    glShaderSource(fragmentShaderID, 1, &fragSourceC, NULL);
    glCompileShader(fragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &compileResult);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::vector<char> errorMessage(logLength + 1);
        glGetShaderInfoLog(fragmentShaderID, logLength, NULL, &errorMessage[0]);
        for (const auto& e : errorMessage)
            std::cout << e;
        std::cout << std::endl;
    }

    // Link the program
    std::cout << "Linking vertex shader with fragment shader" << std::endl;
    GLuint programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    // Check the program
    glGetProgramiv(programID, GL_LINK_STATUS, &compileResult);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        std::vector<char> errorMessage(logLength + 1);
        glGetProgramInfoLog(programID, logLength, NULL, &errorMessage[0]);
        for (const auto& e : errorMessage)
            std::cout << e;
        std::cout << std::endl;
    }

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}





