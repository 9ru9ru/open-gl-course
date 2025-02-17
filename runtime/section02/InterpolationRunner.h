//
// Created by ypp06 on 2025-02-14.
//

#ifndef INTERPOLATIONRUNNER_H
#define INTERPOLATIONRUNNER_H

#include "../base/IBaseRunner.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class InterpolationRunner : public IBaseRunner
{
public:
    bool Run() override;
    ~InterpolationRunner() override = default;
private:
    GLFWwindow* ReadyGlfwGlewEnv();
    void CreateTriangle();
    void AddShader(GLuint targetProgram, const char* rawShaderCode, GLenum shaderType);
    void CompileShaders();

    static const GLint WIDTH, HEIGHT;
    static GLuint VAO, VBO, shaderProgram;

    static const char* vShader;
    static const char* fShader;
};


#endif //INTERPOLATIONRUNNER_H
