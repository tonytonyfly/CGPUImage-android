/**
 * Created by lvHiei on 17-4-10.
 * This is a project of GPUImage implemented with c++, you can
 * use it free. if you find some bug please send me a email.
 * My Email is majun_1523@163.com.
 */

#include "GPUImageEmbossFilter.h"


GPUImageEmbossFilter::GPUImageEmbossFilter()
    : GPUImage3x3ConvolutionFilter()
{
    m_fIntensity = 2.0f;

    setIntensity(m_fIntensity);
}

GPUImageEmbossFilter::~GPUImageEmbossFilter()
{

}

void GPUImageEmbossFilter::setIntensity(int percent)
{
    if(percent < 0){
        percent = 0;
    }

    if(percent > 100){
        percent = 100;
    }

    float incremental = (4.0f - 0.0f) / 100;

    m_fIntensity = 0.0f + incremental * percent;

    setIntensity(m_fIntensity);
}

void GPUImageEmbossFilter::setIntensity(float intensity)
{
    m_fIntensity = intensity;


    float matrix[9];
    int idx = 0;
    matrix[idx++] = m_fIntensity*(-2.0f);
    matrix[idx++] = -m_fIntensity;
    matrix[idx++] = 0.0f;

    matrix[idx++] = -m_fIntensity;
    matrix[idx++] = 1.0f;
    matrix[idx++] = m_fIntensity;

    matrix[idx++] = 0.0f;
    matrix[idx++] = m_fIntensity;
    matrix[idx++] = m_fIntensity * 2.0f;

    setConvolutionKernel(matrix);
}