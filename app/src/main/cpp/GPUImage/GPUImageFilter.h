/**
 * Created by lvHiei on 17-4-1.
 * This is a project of GPUImage implemented with c++, you can
 * use it free. if you find some bug please send me a email.
 * My Email is majun_1523@163.com.
 */


#ifndef CGPUIMAGE_GPUIMAGEFILTER_H
#define CGPUIMAGE_GPUIMAGEFILTER_H

#include <stdio.h>
#include <string.h>

#include "openglHeader.h"

class GPUImageFilter {
public:
    GPUImageFilter();
    virtual ~GPUImageFilter();


public:
    virtual bool createProgram();

    virtual void setVertexCoordinate(float* vertexCoordinate);

    virtual void setTextureCoordinate(float* textureCoordinate);

    virtual bool draw(GLuint textureId, int viewWidth, int viewHeight);
    virtual bool release();

    virtual bool createTexture(int textureWidth, int textureHeight);
    virtual bool draw(GLubyte* rgbaData, int viewWidth, int viewHeight);

protected:
    virtual bool createVertexShader(char* vertex, int& length);
    virtual bool createFragmentShader(char* fragment, int& length);

protected:
    bool beforeDraw();

    bool onDraw();

    bool checkGLError(const char* funName);

protected:
    virtual bool createProgramExtra();
    virtual bool beforeDrawExtra();
    virtual bool onDrawExtra();

protected:
    GLuint m_uProgram;
    GLuint m_uVertexShader;
    GLuint m_uFragmentShader;

    GLuint m_uPositionLocation;
    GLuint m_uTextureCoordLocation;

    float* m_pVertexCoordinate;
    float* m_pTextureCoordinate;

    GLuint m_uTextureId;
    int m_iTextureWidth;
    int m_iTextureHeight;
};


#endif //CGPUIMAGE_GPUIMAGEFILTER_H