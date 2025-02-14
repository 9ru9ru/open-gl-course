//
// Created by ypp06 on 2025-02-14.
//

#ifndef TRIANGLETRANSLATIONRUNNER_H
#define TRIANGLETRANSLATIONRUNNER_H

#include "../base/IBaseRunner.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class TriangleTranslationRunner : public IBaseRunner
{
    public:
        bool Run() override;
        ~TriangleTranslationRunner() override = default;
    private:
        GLFWwindow* ReadyGlfwGlewEnv();
        void ControlMoveOffset();
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
};


#endif //TRIANGLETRANSLATIONRUNNER_H
