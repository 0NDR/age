#include "MultiShader.h"

void MultiShader::CreateFrameBuffer(glm::vec2 newSize)
{

    Activate();
    Resolution = newSize;
    glGenTextures(1,&texColorBuffer);
    glGenTextures(1,&texDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glGenRenderbuffers(1,&renderBufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBufferObject);

    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,Resolution.x,Resolution.y,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer,0);

    glBindTexture(GL_TEXTURE_2D, texDepthBuffer);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32,Resolution.x,Resolution.y,0,GL_DEPTH_COMPONENT,GL_FLOAT,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texDepthBuffer,0);

   /* glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, ResX, ResY);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBufferObject);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);*/
}

void MultiShader::Delete()
{
    glDeleteFramebuffers(1, &frameBuffer);
    glDeleteTextures(1, &texColorBuffer);
    glDeleteTextures(1,&texDepthBuffer);
    glDeleteRenderbuffers(1, &renderBufferObject);
}

void MultiShader::Activate()
{
    if(frameBuffer ==0)
        glGenFramebuffers(1,&frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void MultiShader::setResolution(glm::vec2 newSize)
{
    Activate();
    Resolution = newSize;
    glBindTexture(GL_TEXTURE_2D, texDepthBuffer);
    glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT32,Resolution.x,Resolution.y,0,GL_DEPTH_COMPONENT,GL_FLOAT,0);

    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,Resolution.x,Resolution.y,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);

}
void MultiShader::setResolution(int x, int y)
{
    setResolution(glm::vec2(x,y));
}
void MultiShader::CreateFrameBuffer(int x, int y)
{
    CreateFrameBuffer(glm::vec2(x,y));
}
GLuint MultiShader::RenderToTexture(int shaderIndex, int ResX, int ResY)
{

}
