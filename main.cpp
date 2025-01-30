#define GLEW_STATIC
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// window dimensions.
const GLint WIDTH = 800, HEIGHT = 600;

int main()
{
    // init GLFW.
    if(!glfwInit())
    {
        printf("GLFW init fail");
        glfwTerminate();
        return 1;
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
        return 1;
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
        return 1;
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

    return 0;
}