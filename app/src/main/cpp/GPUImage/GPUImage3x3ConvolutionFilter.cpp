/**
 * Created by lvHiei on 17-4-6.
 * This is a project of GPUImage implemented with c++, you can
 * use it free. if you find some bug please send me a email.
 * My Email is majun_1523@163.com.
 */

#include "GPUImage3x3ConvolutionFilter.h"


// 片元着色器
const char _3x3Convolution_fragment_shader[]=
"precision mediump float;\n"
"uniform sampler2D inputImageTexture;\n"
"\n"
"uniform mat3 convolutionMatrix;\n"
"\n"
"varying vec2 textureCoordinate;\n"
"varying vec2 leftTextureCoordinate;\n"
"varying vec2 rightTextureCoordinate;\n"
"\n"
"varying vec2 topTextureCoordinate;\n"
"varying vec2 topLeftTextureCoordinate;\n"
"varying vec2 topRightTextureCoordinate;\n"
"\n"
"varying vec2 bottomTextureCoordinate;\n"
"varying vec2 bottomLeftTextureCoordinate;\n"
"varying vec2 bottomRightTextureCoordinate;\n"
"\n"
"void main()\n"
"{\n"
"    vec3 bottomColor = texture2D(inputImageTexture, bottomTextureCoordinate).rgb;\n"
"    vec3 bottomLeftColor = texture2D(inputImageTexture, bottomLeftTextureCoordinate).rgb;\n"
"    vec3 bottomRightColor = texture2D(inputImageTexture, bottomRightTextureCoordinate).rgb;\n"
"    vec4 centerColor = texture2D(inputImageTexture, textureCoordinate);\n"
"    vec3 leftColor = texture2D(inputImageTexture, leftTextureCoordinate).rgb;\n"
"    vec3 rightColor = texture2D(inputImageTexture, rightTextureCoordinate).rgb;\n"
"    vec3 topColor = texture2D(inputImageTexture, topTextureCoordinate).rgb;\n"
"    vec3 topRightColor = texture2D(inputImageTexture, topRightTextureCoordinate).rgb;\n"
"    vec3 topLeftColor = texture2D(inputImageTexture, topLeftTextureCoordinate).rgb;\n"
"\n"
"    vec3 resultColor = topLeftColor * convolutionMatrix[0][0] + topColor * convolutionMatrix[0][1] + topRightColor * convolutionMatrix[0][2];\n"
"    resultColor += leftColor * convolutionMatrix[1][0] + centerColor.rgb * convolutionMatrix[1][1] + rightColor * convolutionMatrix[1][2];\n"
"    resultColor += bottomLeftColor * convolutionMatrix[2][0] + bottomColor * convolutionMatrix[2][1] + bottomRightColor * convolutionMatrix[2][2];\n"
"\n"
"    gl_FragColor = vec4(resultColor, centerColor.a);\n"
"}"
;


GLfloat convolution_init[] = {
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
};


GLfloat convolution_example[] = {
        -1.0f, 0.0f, 1.0f,
        -2.0f, 0.0f, 2.0f,
        -1.0f, 0.0f, 1.0f,
};

GPUImage3x3ConvolutionFilter::GPUImage3x3ConvolutionFilter() :
    GPUImage3x3TextureSamplingFilter(_3x3Convolution_fragment_shader)
{
    memcpy(m_pConvolutionKernel, convolution_example, 9 * sizeof(float));
}

GPUImage3x3ConvolutionFilter::~GPUImage3x3ConvolutionFilter()
{

}

bool GPUImage3x3ConvolutionFilter::createProgramExtra()
{
    m_iConvolutionUniformLocation = glGetUniformLocation(m_uProgram, "convolutionMatrix");
    GPUImage3x3TextureSamplingFilter::createProgramExtra();
    return true;
}

bool GPUImage3x3ConvolutionFilter::beforeDrawExtra()
{
    glUniformMatrix3fv(m_iConvolutionUniformLocation, 1, false, m_pConvolutionKernel);
    GPUImage3x3TextureSamplingFilter::beforeDrawExtra();
    return true;
}

void GPUImage3x3ConvolutionFilter::setConvolutionKernel(float *convolutionKernel)
{
    memcpy(m_pConvolutionKernel, convolutionKernel, 9 * sizeof(float));
}