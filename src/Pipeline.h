#ifndef PIPELINE_H
#define PIPELINE_H

#include <stdio.h>

#include "GL.h"

namespace Pipeline
{
    inline void contextInitalization() // set up default properties
    {
    }
    
    inline void geometryMode()
    {
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
        glDepthMask(GL_TRUE); // we are done with the depth buffer, unlock it
    }
    
    inline void lightMode()
    {
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE, GL_ONE);
        glDepthMask(GL_FALSE); // lock the depth buffer because we will now be sampling from it
    }
    
    inline void screenMode()
    {
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDepthMask(GL_TRUE); // we are done with the depth buffer, unlock it
    }
    
    inline void errorCheck(){ int error; if((error = glGetError()) != 0) printf("OpenGL error: %i\n", error); }
};

#endif
