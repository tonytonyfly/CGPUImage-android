/**
 * Created by lvHiei on 17-4-6.
 * This is a project of GPUImage implemented with c++, you can
 * use it free. if you find some bug please send me a email.
 * My Email is majun_1523@163.com.
 */

#include "GPUImageLookupFilter.h"
#include "../util/FileUtil.h"


#ifdef __GLSL_SUPPORT_HIGHP__

// 片元着色器
extern const char _lookUp_fragment_shader[]=
"varying highp vec2 textureCoordinate;\n"
"varying highp vec2 textureCoordinate2; // TODO: This is not used\n"
"\n"
"uniform sampler2D inputImageTexture;\n"
"uniform sampler2D inputImageTexture2; // lookup texture\n"
"\n"
"uniform lowp float intensity;\n"
"\n"
"void main()\n"
"{\n"
"    highp vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n"
"\n"
"	// this is redundancy, use it because some device will has gl error if the textureCoordinate2 is not used\n "
"	 highp vec4 textureColor2 = texture2D(inputImageTexture, textureCoordinate2);\n "
"\n"
"    highp float blueColor = textureColor.b * 63.0;\n"
"\n"
"    highp vec2 quad1;\n"
"    quad1.y = floor(floor(blueColor) / 8.0);\n"
"    quad1.x = floor(blueColor) - (quad1.y * 8.0);\n"
"\n"
"    highp vec2 quad2;\n"
"    quad2.y = floor(ceil(blueColor) / 8.0);\n"
"    quad2.x = ceil(blueColor) - (quad2.y * 8.0);\n"
"\n"
"    highp vec2 texPos1;\n"
"    texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);\n"
"    texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);\n"
"\n"
"    highp vec2 texPos2;\n"
"    texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);\n"
"    texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);\n"
"\n"
"    lowp vec4 newColor1 = texture2D(inputImageTexture2, texPos1);\n"
"    lowp vec4 newColor2 = texture2D(inputImageTexture2, texPos2);\n"
"\n"
"    lowp vec4 newColor = mix(newColor1, newColor2, fract(blueColor));\n"
"    gl_FragColor = mix(textureColor, vec4(newColor.rgb, textureColor.w), intensity);\n"
"}"
;

#else

// 片元着色器
extern const char _lookUp_fragment_shader[]=
"precision mediump float;\n"
"uniform sampler2D inputImageTexture;\n"
"varying vec2 textureCoordinate;\n"

"uniform sampler2D inputImageTexture2;\n"
"varying vec2 textureCoordinate2;\n"

"uniform float intensity;\n"

"void main()\n"
"{\n"
"	vec4 textureColor = texture2D(inputImageTexture, textureCoordinate);\n "
"	// this is redundancy, use it because some device will has gl error if the textureCoordinate2 is not used\n "
"	vec4 textureColor2 = texture2D(inputImageTexture, textureCoordinate2);\n "
"   // lookupfilter \n"
"   float blueColor = textureColor.b * 63.0;\n"
"\n"
"   vec2 quad1;\n"
"   quad1.y = floor(floor(blueColor) / 8.0);\n"
"   quad1.x = floor(blueColor) - (quad1.y * 8.0);\n"
"\n"
"   vec2 quad2;\n"
"   quad2.y = floor(ceil(blueColor) / 8.0);\n"
"   quad2.x = ceil(blueColor) - (quad2.y * 8.0);\n"
"\n"
"   vec2 texPos1;\n"
"   texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);\n"
"   texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);\n"
"\n"
"   vec2 texPos2;\n"
"   texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);\n"
"   texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);\n"
"\n"
"   lowp vec4 newColor1 = texture2D(inputImageTexture2, texPos1);\n"
"   lowp vec4 newColor2 = texture2D(inputImageTexture2, texPos2);\n"
"\n"
"   lowp vec4 newColor = mix(newColor1, newColor2, fract(blueColor));\n"
"   vec4 lookUpColor = mix(textureColor, vec4(newColor.rgb, textureColor.w), lookupIntensity);\n"
"   gl_FragColor = lookUpColor;\n"
//"   gl_FragColor = vec4(1.0,0.0,0.0,1.0);\n"
"}\n"
;

#endif



GPUImageLookupFilter::GPUImageLookupFilter() :
    GPUImageImageFilter(_lookUp_fragment_shader)
{
    m_fIntensity = 1.0f;

    m_pFilename = NULL;
}


GPUImageLookupFilter::GPUImageLookupFilter(const char *filename)
    : GPUImageImageFilter(_lookUp_fragment_shader)
{
    m_fIntensity = 1.0f;

    m_pFilename = (char *) malloc(strlen(filename) + 1);
    strcpy(m_pFilename, filename);
}


GPUImageLookupFilter::~GPUImageLookupFilter()
{
    this->release();
}

bool GPUImageLookupFilter::createProgramExtra()
{
    GPUImageImageFilter::createProgramExtra();

    m_iIntensityLocation = glGetUniformLocation(m_uProgram, "intensity");

    return true;
}

bool GPUImageLookupFilter::beforeDrawExtra()
{
    glUniform1f(m_iIntensityLocation, m_fIntensity);
    GPUImageImageFilter::beforeDrawExtra();
    return true;
}

void GPUImageLookupFilter::setIntensity(int percent)
{
    if(percent < 0){
        percent = 0;
    }

    if(percent > 100){
        percent = 100;
    }

    float incremental = (1.0f - 0.0f) / 100;

    m_fIntensity = 0.0f + incremental * percent;
}


void GPUImageLookupFilter::setIntensity(float intensity)
{
    m_fIntensity = intensity;
}


bool GPUImageLookupFilter::loadImage()
{
    if(!m_pFilename){
        return GPUImageImageFilter::loadImage();
    }

    const char* filename = m_pFilename;

    m_uPicWidth = 512;
    m_uPicHeight = 512;
    uint32_t fileLen = FileUtil::getFileSize(filename);
    if(fileLen > 0){
        m_pPicDataRGBA = (uint8_t *) malloc(fileLen * sizeof(uint8_t));
        if(!m_pPicDataRGBA){
            return false;
        }

        FileUtil::loadFile(filename, m_pPicDataRGBA, fileLen);
    }

    return true;
}


bool GPUImageLookupFilter::release()
{
    if(m_pFilename){
        free(m_pFilename);
        m_pFilename = NULL;
    }

    return GPUImageImageFilter::release();
}

