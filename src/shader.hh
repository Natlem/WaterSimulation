#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
GLuint LoadCompileLinkShaders(std::string vertexPathSrc, std::string fragmentPathSrc);
GLuint LoadCompileLinkShader2(std::string vertexPathSrc, std::string fragmentPathSrc);

