//
//  Window.cpp
//  OpenGLTriangle
//
//  Created by Tan Li Yuan on 4/9/21.
//

#include <stdio.h>
#include "Window.h"

Window::Window() {
    width = 800;
    height = 600;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
    width = windowWidth;
    height = windowHeight;
}

int Window::initialize() {
    // initialize GLFW
    if (!glfwInit()) {
        printf("GLFW initialization failed");
        glfwTerminate();
        return 1;
    }
    
    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Core profile - No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);
    
    if (!mainWindow){
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }
    
    // Get Buffer size information
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
    
    // Set context for GLEW to use
    glfwMakeContextCurrent(mainWindow);
    
    // ALlow modern extension features
    glewExperimental = GL_TRUE;
    
    if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    
    glEnable(GL_DEPTH_TEST);
    
    // Create Viewport
    glViewport(0, 0, bufferWidth, bufferHeight);
    return 0;
}

Window::~Window() {
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
