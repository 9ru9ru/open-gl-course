//
// Created by ypp06 on 2025-02-14.
//

#ifndef TRIANGLESCALINGRUNNER_H
#define TRIANGLESCALINGRUNNER_H

#include "../base/IBaseRunner.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class TriangleScalingRunner : public IBaseRunner
{
public:
    bool Run() override;
    ~TriangleScalingRunner() override = default;
    TriangleScalingRunner();
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
    static const float scaleMax;
    static const float scaleIncrement;
    float currentScale;
    bool sizeDirection;
};


#endif //TRIANGLESCALINGRUNNER_H
