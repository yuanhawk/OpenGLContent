//
//  Texture.h
//  OpenGLTriangle
//
//  Created by Tan Li Yuan on 24/12/21.
//

#include "GL/glew.h"

#include "stb_image.h"

class Texture {
    public:
        Texture();
        Texture(char* fileLoc);
    
        void LoadTexture();
        void UseTexture();
        void ClearTexture();
        ~Texture();
    
    private:
        GLuint textureID;
        int width, height, bitDepth;
    
        char* fileLocation;
};
