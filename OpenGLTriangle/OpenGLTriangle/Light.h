//
//  Light.h
//  OpenGLTriangle
//
//  Created by Tan Li Yuan on 27/12/21.
//

#include "GL/glew.h"
#include "glm/glm.hpp"

class Light {
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);
    
    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColorLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);
    
    ~Light();
    
private:
    glm::vec3 color, direction;
    GLfloat ambientIntensity, diffuseIntensity;
};
