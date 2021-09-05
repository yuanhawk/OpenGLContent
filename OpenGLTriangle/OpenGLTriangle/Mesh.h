//
//  Mesh.h
//  OpenGLTriangle
//
//  Created by Tan Li Yuan on 4/9/21.
//

#ifndef Mesh_h
#define Mesh_h


#endif /* Mesh_h */

#include "GL/glew.h"

class Mesh {
public:
    Mesh();
    
    void CreateMesh(GLfloat *vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void RenderMesh();
    void ClearMesh();
    
    ~Mesh();
    
private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};
