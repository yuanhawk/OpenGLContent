//
//  Camera.h
//  OpenGLTriangle
//
//  Created by Tan Li Yuan on 23/12/21.
//

#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"

class Camera {
    public:
        Camera();
        Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
    
        void keyControl(bool* keys, GLfloat deltaTime);
        void mouseControl(GLfloat xChange, GLfloat yChange);
    
        glm::mat4 calculateViewMatrix();
    
        ~Camera();
    
    private:
        glm::vec3 position, front, up, right, worldUp;
        
        GLfloat yaw, pitch, moveSpeed, turnSpeed;
    
        void update();
};
