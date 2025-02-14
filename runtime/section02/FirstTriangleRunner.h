//
// Created by ypp06 on 2025-02-02.
//

#ifndef FIRSTTRIANGLERUNNER_H
#define FIRSTTRIANGLERUNNER_H

#include "../base/IBaseRunner.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class FirstTriangleRunner : public IBaseRunner
{
    public:
        bool Run() override;
        ~FirstTriangleRunner() override = default;
    private:
        GLFWwindow* ReadyGlfwGlewEnv();
        void ControlMoveOffset();
        void CreateTriangle();
        void AddShader(GLuint targetProgram, const char* rawShaderCode, GLenum shaderType);
        void CompileShaders();
};

#endif //FIRSTTRIANGLERUNNER_H
