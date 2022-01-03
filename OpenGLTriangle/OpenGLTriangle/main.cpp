//
//  main.cpp
//  OpenGLTriangle
//
//  Created by Tan Li Yuan on 1/8/21.
//
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <vector>
#include <assert.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"

const float toRadians = M_PI / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture, dirtTexture;
Light mainLight;

GLfloat deltaTime = 0.0f, lastTime = 0.0f;

// Vertex Shader code
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset) {
    for (size_t i = 0; i < indiceCount; i += 3) {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i + 1] * vLength;
        unsigned int in2 = indices[i + 2] * vLength;
        
        glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);
        
        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;
        
        vertices[in0] += normal.x;
        vertices[in0 + 1] += normal.y;
        vertices[in0 + 2] += normal.z;
        
        vertices[in1] += normal.x;
        vertices[in1 + 1] += normal.y;
        vertices[in1 + 2] += normal.z;
        
        vertices[in2] += normal.x;
        vertices[in2 + 1] += normal.y;
        vertices[in2 + 2] += normal.z;
    }
    
    for (size_t i = 0; i < verticeCount / vLength; i++) {
        unsigned int nOffset = static_cast<int>(i * vLength + normalOffset);
        glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = glm::normalize(vec);
        vertices[nOffset] = vec.x;
        vertices[nOffset + 1] = vec.y;
        vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects() {
    unsigned int indices[] = {
        0, 3, 1,
        1, 2, 2,
        2, 3, 0,
        0, 1, 2
    };
    
    GLfloat vertices[] = {
    //   x      y      z         u      v           nx    ny    nz
        -1.0f, -1.0f, 0.0f,      0.0f, 0.0f,        0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 1.0f,       0.5f, 0.0f,        0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f,       1.0f, 0.0f,        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,        0.5f, 1.0f,        0.0f, 0.0f, 0.0f
    };
    
    calcAverageNormals(indices, 12, vertices, 32, 8, 5);
    
    Mesh *obj1 = new Mesh();
    obj1 -> CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);
    
    Mesh *obj2 = new Mesh();
    obj2 -> CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj2);
}

void CreateShaders() {
    Shader *shader1 = new Shader();
    shader1 -> CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main() {
    
    mainWindow = Window(800, 600);
    mainWindow.initialize();
    
    CreateObjects();
    CreateShaders();
    
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 90.0f, 0.0f, 5.0f, 0.5f);
    
    brickTexture = Texture((char*) "Textures/brick.png");
    brickTexture.LoadTexture();
    dirtTexture = Texture((char*) "Textures/dirt.png");
    dirtTexture.LoadTexture();
    
    mainLight = Light(1.0f, 1.0f, 1.0f, 0.2f,
                    2.0f, -1.0f, -2.0f, 1.0f);
    
    GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformAmbientIntensity = 0, uniformAmbientColor = 0, uniformDirection = 0, uniformDiffuseIntensity = 0;
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat) mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);
    
    // Loop until window closed
    while(!mainWindow.getShouldClose()) {
        GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
        deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
        lastTime = now;
        
        // Get + Handle user input events
        glfwPollEvents();
        
        camera.keyControl(mainWindow.getKeys(), deltaTime);
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
        
        // Clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        uniformView = shaderList[0].GetViewLocation();
        uniformAmbientColor = shaderList[0].GetAmbientColorLocation();
        uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
        uniformDirection = shaderList[0].GetDirectionLocation();
        uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
        
        mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor, uniformDiffuseIntensity, uniformDirection);
        
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, 0.0f * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
        brickTexture.UseTexture();
        meshList[0] -> RenderMesh();
        
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        dirtTexture.UseTexture();
        meshList[1] -> RenderMesh();
        
        glUseProgram(0);
        
        mainWindow.swapBuffers();
    }
    
    return 0;
}

