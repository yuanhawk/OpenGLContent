//
//  main.cpp
//  OpenGLTriangle
//
//  Created by Tan Li Yuan on 1/8/21.
//
#include <stdio.h>
#include <string.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

const GLint WIDTH = 800, HEIGHT = 600;
GLuint VAO, VBO, shader;

// Vertex Shader code
static const char* vShader = "                                                \n\
#version 330                                                                  \n\
layout (location = 0) in vec3 pos;                                            \n\
void main()                                                                   \n\
{                                                                             \n\
    gl_Position = vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);                 \n\
}";

// Fragment Shader
static const char* fShader = "                                                \n\
#version 330                                                                  \n\
out vec4 color;                                                               \n\
void main()                                                                   \n\
{                                                                             \n\
    color = vec4(1.0, 0.0, 0.0, 1.0);                                         \n\
}";

void CreateTriangle() {
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }
    glAttachShader(theProgram, theShader);
}

void CompileShaders() {
    shader = glCreateProgram();
    
    if (!shader) {
        printf("Error creating shader program!\n");
        return;
    }
    
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }
    
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
}

int main() {
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
    
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
    
    if (!mainWindow){
        printf("GLFW window creation failed!");
        glfwTerminate();
        return 1;
    }
    
    // Get Buffer size information
    int bufferWidth, bufferHeight;
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
    
    // Create Viewport
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    CreateTriangle();
    CompileShaders();
    
    // Loop until window closed
    while(!glfwWindowShouldClose(mainWindow)) {
        // Get + Handle user input events
        glfwPollEvents();
        
        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shader);
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        
        glUseProgram(0);
        
        glfwSwapBuffers(mainWindow);
    }
    
    return 0;
}

