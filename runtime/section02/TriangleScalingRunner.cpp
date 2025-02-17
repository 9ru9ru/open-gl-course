//
// Created by ypp06 on 2025-02-02.
//

#include "TriangleScalingRunner.h"
#include <iostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 전역 변수 정의
const GLint TriangleScalingRunner::WIDTH = 800;
const GLint TriangleScalingRunner::HEIGHT = 600;
GLuint TriangleScalingRunner::VAO = 0;
GLuint TriangleScalingRunner::VBO = 0;
GLuint TriangleScalingRunner::shaderProgram = 0;
GLuint TriangleScalingRunner::uniformModel = 0;
bool TriangleScalingRunner::direction = true;
float TriangleScalingRunner::triOffset = 0.0f;
float TriangleScalingRunner::triMaxOffset = 0.7f;
float TriangleScalingRunner::triIncrement = 0.005f;
const float TriangleScalingRunner::scaleMax = 2.0f;
const float TriangleScalingRunner::scaleIncrement = 0.025f;

// Shader 소스 코드 정의
const char* TriangleScalingRunner::vShader =
    "#version 330\n"
    "layout (location = 0) in vec3 pos;\n"
    "uniform mat4 model;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = model * vec4(pos, 1.0);\n"
    "}\n";

const char* TriangleScalingRunner::fShader =
    "#version 330\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "   color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

TriangleScalingRunner::TriangleScalingRunner()
{
    currentScale = 0.0f;
    sizeDirection = true;
}

// Add Shader to Program.
// Shader에 대해 1. source 지정, 2. compile, 3. targetProgram에 attach.
void TriangleScalingRunner::AddShader(GLuint targetProgram, const char* rawShaderCode, GLenum shaderType)
{
    GLuint targetShader = glCreateShader(shaderType);

    const GLchar* code[1];
    code[0] = rawShaderCode;

    GLint codeLength[1];
    codeLength[0] = strlen(rawShaderCode);

    glShaderSource(targetShader, 1, code, codeLength);
    glCompileShader(targetShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(targetShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(targetShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(targetProgram, targetShader);
    return;
}

void TriangleScalingRunner::CompileShaders()
{
    shaderProgram = glCreateProgram();
    if (!shaderProgram)
    {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shaderProgram, vShader, GL_VERTEX_SHADER);
    AddShader(shaderProgram, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    // 제대로 링크되지 않았다면.
    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    // Validate program.
	glValidateProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
    uniformModel = glGetUniformLocation(shaderProgram, "model");
}

void TriangleScalingRunner::CreateTriangle()
{
    // NDC 좌표계에서 삼각형을 구성하는 vertex들의 위치.
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // VAO 생성 및 Bind - 이 밑에 생성되는 VBO들을 VAO에 자동으로 Bind.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO 생성 및 Bind.
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attribute 선언 및 활성화
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // VBO binding 다시 정리해주기
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // VAO binding 다시 정리해주기
    glBindVertexArray(0);
}

// return NULL if failed.
GLFWwindow* TriangleScalingRunner::ReadyGlfwGlewEnv()
{
    // init GLFW.
    if(!glfwInit())
    {
        printf("GLFW init fail");
        glfwTerminate();
        return NULL;
    }

    // Setup GLFW window properties.
    // OpenGL Version.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core profile = No Backwards Compatible.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatible.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Hello Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("glfwCreateWindow failed!");
        glfwTerminate();
        return NULL;
    }

    // Get Buffer Size Info.
    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    // Set Context for GLEW to use.
    glfwMakeContextCurrent(mainWindow);

    // Allow modern extension feats.
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("glew init failed");
        glfwDestroyWindow(mainWindow); // 이 시점에서 window가 생성되었으니 지워야 한다.
        glfwTerminate();
        return NULL;
    }

    // Setup Viewport size.
    glViewport(0, 0, bufferWidth, bufferHeight);

    return mainWindow;
}

void TriangleScalingRunner::MoveTriangle()
{
    if (direction)
    {
        triOffset += triIncrement;
    }
    else
    {
        triOffset -= triIncrement;
    }
    if (abs(triOffset) >= triMaxOffset)
    {
        direction = !direction;
    }

    if (sizeDirection)
    {
        currentScale += scaleIncrement;
    }
    else
    {
        currentScale -= scaleIncrement;
    }

    if (currentScale >= scaleMax)
    {
        sizeDirection = false;
    }
    else if (currentScale <= 0.0f)
    {
        sizeDirection = true;
    }


    glm::mat4 scaleMatrix(1.0f); // identity matrix.
    scaleMatrix = glm::scale(scaleMatrix, currentScale * glm::vec3(1.0f, 1.0f, 1.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(scaleMatrix));
}

bool TriangleScalingRunner::Run()
{
    GLFWwindow* mainWindow = ReadyGlfwGlewEnv();
    if (!mainWindow)
    {
        return false;
    }

    CreateTriangle();
    CompileShaders();
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    // loop til window closed.
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get + handle user input events.
        glfwPollEvents();

        // Clear window.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        MoveTriangle();

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(mainWindow);
    }
    glBindVertexArray(0);
    glUseProgram(0);
    return true;
}
