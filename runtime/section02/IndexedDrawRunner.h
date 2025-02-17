//
// Created by ypp06 on 2025-02-14.
//

#ifndef INDEXEDDRAWRUNNER_H
#define INDEXEDDRAWRUNNER_H

#include "../base/IBaseRunner.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class IndexedDrawRunner : public IBaseRunner
{
public:
    bool Run() override;
    IndexedDrawRunner();
    ~IndexedDrawRunner() override = default;
private:
    GLFWwindow* ReadyGlfwGlewEnv();
    void CreateTriangle();
    void RotateTriangle();
    void AddShader(GLuint targetProgram, const char* rawShaderCode, GLenum shaderType);
    void CompileShaders();

    static const GLint WIDTH, HEIGHT;
    static GLuint uniformModel;
    static GLuint VAO, VBO, shaderProgram, IBO;

    static const char* vShader;
    static const char* fShader;

    static const float toRads;
    float currentAngle;
};


#endif //INDEXEDDRAWRUNNER_H
