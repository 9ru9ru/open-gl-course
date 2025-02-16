//
// Created by ypp06 on 2025-02-14.
//

#ifndef TRIANGLEROTATIONRUNNER_H
#define TRIANGLEROTATIONRUNNER_H

#include "../base/IBaseRunner.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class TriangleRotationRunner : public IBaseRunner
{
public:
    bool Run() override;
    TriangleRotationRunner();
    ~TriangleRotationRunner() override = default;
private:
    GLFWwindow* ReadyGlfwGlewEnv();
    void MoveTriangle();
    void CreateTriangle();

    void AddShader(GLuint targetProgram, const char* rawShaderCode, GLenum shaderType);
    void CompileShaders();

    static const GLint WIDTH, HEIGHT;
    static GLuint VAO, VBO, shaderProgram;
    static GLuint uniformModel;
    static bool direction;
    static float triOffset, triMaxOffset, triIncrement;

    static const char* vShader;
    static const char* fShader;
    static const float toRads;

    float currentAngle;
};


#endif //TRIANGLEROTATIONRUNNER_H
