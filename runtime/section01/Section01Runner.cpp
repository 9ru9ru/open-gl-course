//
// Created by ypp06 on 2025-01-31.
//
#define GLEW_STATIC
#include "Section01Runner.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

bool Section01Runner::Run()
{
    // init GLFW.
    if(!glfwInit())
    {
        printf("GLFW init fail");
        glfwTerminate();
        return false;
    }

    // Setup GLFW window properties.
    // OpenGL Version.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Core profile = No Backwards Compatible.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatible.
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(windowWidth, windowHeight, "Hello Window", NULL, NULL);
    if (!mainWindow)
    {
        printf("glfwCreateWindow failed!");
        glfwTerminate();
        return false;
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
        return false;
    }

    // Setup Viewport size.
    glViewport(0, 0, bufferWidth, bufferHeight);

    // loop til window closed.
    while (!glfwWindowShouldClose(mainWindow))
    {
        // Get + handle user input events.
        glfwPollEvents();

        // Clear window.
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mainWindow);
    }

    return true;
}