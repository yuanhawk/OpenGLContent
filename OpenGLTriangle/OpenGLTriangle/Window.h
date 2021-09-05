//
//  Window.h
//  OpenGLTriangle
//
//  Created by Tan Li Yuan on 4/9/21.
//

#ifndef Window_h
#define Window_h


#endif /* Window_h */

#include <stdio.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Window {
public:
    Window();
    
    Window(GLint windowWidth, GLint windowHeight);
    int initialize();
    
    GLfloat getBufferWidth() {
        return bufferWidth;
    }
    
    GLfloat getBufferHeight() {
        return bufferHeight;
    }
    
    bool getShouldClose() {
        return glfwWindowShouldClose(mainWindow);
    }
    
    void swapBuffers() {
        glfwSwapBuffers(mainWindow);
    }
    
    ~Window();
    
    
private:
    GLFWwindow *mainWindow;
    
    GLint width, height;
    GLint bufferWidth, bufferHeight;
};
