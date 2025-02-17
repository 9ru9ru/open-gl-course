//
// Created by ypp06 on 2025-02-02.
//

#include "IndexedDrawRunner.h"
#include <iostream>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// 전역 변수 정의
const GLint IndexedDrawRunner::WIDTH = 800;
const GLint IndexedDrawRunner::HEIGHT = 600;
GLuint IndexedDrawRunner::VAO = 0;
GLuint IndexedDrawRunner::VBO = 0;
GLuint IndexedDrawRunner::IBO = 0;
GLuint IndexedDrawRunner::uniformModel = 0;
GLuint IndexedDrawRunner::shaderProgram = 0;

// Shader 소스 코드 정의
const char* IndexedDrawRunner::vShader =
    "#version 330\n"
    "layout (location = 0) in vec3 pos;\n"
    "uniform mat4 model;\n"
    "out vec4 vCol;\n"
    "void main()\n"
    "{\n"
    "   vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);\n"
    "   gl_Position = model * vec4(pos * 0.4f, 1.0);\n"
    "}\n";

const char* IndexedDrawRunner::fShader =
    "#version 330\n"
    "out vec4 color;\n"
    "in vec4 vCol;\n"
    "void main()\n"
    "{\n"
    "   color = vCol;\n"
    "}\n";

const float IndexedDrawRunner::toRads = 3.14159265f / 180.0f;

// make currentAngle = 0;
IndexedDrawRunner::IndexedDrawRunner()
{
    currentAngle = 0.0f;
}

// Add Shader to Program.
// Shader에 대해 1. source 지정, 2. compile, 3. targetProgram에 attach.
void IndexedDrawRunner::AddShader(GLuint targetProgram, const char* rawShaderCode, GLenum shaderType)
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

void IndexedDrawRunner::CompileShaders()
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

void IndexedDrawRunner::CreateTriangle()
{

    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };
    // NDC 좌표계에서 삼각형을 구성하는 vertex들의 위치.
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    // VAO 생성 및 Bind - 이 밑에 생성되는 VBO들을 VAO에 자동으로 Bind.
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Index Buffer Object 생성 및 Bind.
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // VBO 생성 및 Bind.
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Attribute 선언 및 활성화
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Index Buffer Object binding 다시 정리해주기
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    // VBO binding 다시 정리해주기
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // VAO binding 다시 정리해주기
    glBindVertexArray(0);
}

// RotateTriangle
void IndexedDrawRunner::RotateTriangle()
{
    currentAngle += 0.50f;
    if (currentAngle >= 360.0f)
    {
        currentAngle -= 360.0f;
    }
    glm::mat4 translateMatrix(1.0f); // identity matrix.
    translateMatrix = glm::rotate(translateMatrix, currentAngle * toRads, glm::vec3(0.0f, 1.0f, 0.0f)); // 세 번째 파라미터는 회전축. y-axis
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(translateMatrix));
}

// return NULL if failed.
GLFWwindow* IndexedDrawRunner::ReadyGlfwGlewEnv()
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

    glEnable(GL_DEPTH_TEST); // enable depth testing.

    // Setup Viewport size.
    glViewport(0, 0, bufferWidth, bufferHeight);

    return mainWindow;
}

bool IndexedDrawRunner::Run()
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    // loop til window closed.
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get + handle user input events.
        glfwPollEvents();

        // Clear window.
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear color buffer and depth buffer.

        RotateTriangle();

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 3 * 4, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(mainWindow);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(0);
    return true;
}
