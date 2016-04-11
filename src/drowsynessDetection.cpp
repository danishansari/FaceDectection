#include "drowsynessDetection.h"

#include <iostream>
#include <vector>
#include <highgui.h>

using namespace std;

DrowsynessDetection::DrowsynessDetection() 
{ 
    m_width = 0;
    m_height = 0;
    m_nChannels = 1;

    m_img = NULL;
    m_resImg = NULL;

    m_binThreash = 50;
}

DrowsynessDetection::~DrowsynessDetection() 
{ 
    cvReleaseImage(&m_img);
    cvReleaseImage(&m_resImg);
}

void DrowsynessDetection::doBinarization(IplImage *&img)
{
    int width = img->width;
    int height = img->height;
    int steps = img->widthStep;

    uchar *data = (uchar*)img->imageData;

    for (int x = 0; x < height; x++)
    {
        data = (uchar*)(img->imageData + x*steps);
        
        for (int y = 0; y < width; y++)
        {
            if (*data > m_binThreash)
                *data = 0;
            else
                *data = 255;

            data++;
        }
    }
}

void DrowsynessDetection::doPreProcessing(IplImage *&img)
{
    memset(m_resFaceRect, 0, sizeof(m_resFaceRect));
    memset(m_actualFaceRect, 0, sizeof(m_actualFaceRect));
    memset(m_possEyeRect, 0, sizeof(m_possEyeRect));
    memset(m_actualEyeRect, 0, sizeof(m_actualEyeRect));

    if(m_width == 0 || m_height == 0 || m_img == NULL || m_resImg == NULL)
    {
        cout << "Initialization.." << endl;
        m_width = img->width;
        m_height = img->height;
        m_img = cvCreateImage(cvSize(m_width, m_height), IPL_DEPTH_8U, 1);
        m_resImg = cvCreateImage(cvSize(m_width/4, m_height/4), IPL_DEPTH_8U, 1);
    }

    IplImage *temp = cvCreateImage(cvSize(m_width, m_height), IPL_DEPTH_8U, img->nChannels);

    cvCopy(img, temp);

    if(temp->nChannels == 3)
        cvCvtColor(temp, m_img, CV_RGB2GRAY);
    else
        cvCopy(temp, m_img);
    
    cvReleaseImage(&temp);

    cvResize(m_img, m_resImg);
}

void DrowsynessDetection::findPossEyeCoordinate()
{ 
    m_actualFaceRect[0] = m_resFaceRect[0]*4;
    m_actualFaceRect[1] = m_resFaceRect[1]*4;
    m_actualFaceRect[2] = m_resFaceRect[2]*4;
    m_actualFaceRect[3] = m_resFaceRect[3]*4;

    m_possEyeRect[0] = m_actualFaceRect[0]+(m_actualFaceRect[2]/5);
    m_possEyeRect[1] = m_actualFaceRect[1]+(m_actualFaceRect[3]/3.1);
    m_possEyeRect[2] = m_actualFaceRect[2]/1.6;
    m_possEyeRect[3] = m_actualFaceRect[3]/6;
}

void DrowsynessDetection::drawRectangle(IplImage *&img)
{
    if (m_resFaceRect[0] > 0 && m_resFaceRect[1] > 0 &&
        m_resFaceRect[2] > 0 && m_resFaceRect[3] > 0)
    {

        cvRectangle(m_resImg, cvPoint(m_resFaceRect[0], m_resFaceRect[1]), 
                              cvPoint(m_resFaceRect[0]+m_resFaceRect[2], 
                                      m_resFaceRect[1]+m_resFaceRect[3]), 
                              cvScalar(0, 0, 0), 1, 8, 0);
    }

    if (m_actualFaceRect[0] > 0 && m_actualFaceRect[1] > 0 &&
        m_actualFaceRect[2] > 0 && m_actualFaceRect[3] > 0)
    {
        cvRectangle(img, cvPoint(m_actualFaceRect[0], m_actualFaceRect[1]), 
                         cvPoint(m_actualFaceRect[0]+m_actualFaceRect[2],
                                 m_actualFaceRect[1]+m_actualFaceRect[3]), 
                         cvScalar(0, 255, 0), 1, 8, 0);
    }

    int eyeFlag = 0;

    if (m_possEyeRect[0] > 0 && m_possEyeRect[1] > 0 &&
        m_possEyeRect[2] > 0 && m_possEyeRect[3] > 0)
    {
        eyeFlag = 1;
        cvRectangle(img, cvPoint(m_possEyeRect[0], m_possEyeRect[1]), 
                         cvPoint(m_possEyeRect[0]+m_possEyeRect[2], 
                                 m_possEyeRect[1]+m_possEyeRect[3]), 
                         cvScalar(255, 0, 0), 1, 8, 0);
    }


    if (0 && m_resEyeRect[0] > 0 && m_resEyeRect[1] > 0 &&
                   m_resEyeRect[2] > 0 && m_resEyeRect[3] > 0)
    {
        m_actualEyeRect[0] = m_possEyeRect[0]+m_resEyeRect[0];
        //m_actualEyeRect[1] = m_possEyeRect[1]+m_resEyeRect[1];
        m_actualEyeRect[1] = m_resEyeRect[1];
        m_actualEyeRect[2] = m_possEyeRect[2]+m_resEyeRect[2];
        m_actualEyeRect[3] = m_possEyeRect[3]+m_resEyeRect[3];


        cvRectangle(img, cvPoint(m_actualEyeRect[0], m_actualEyeRect[1]), 
                         cvPoint(m_actualEyeRect[0]+m_actualEyeRect[2], 
                                 m_actualEyeRect[1]+m_actualEyeRect[3]), 
                         cvScalar(0, 0, 255), 1, 8, 0);
    } 
}

void DrowsynessDetection::processFrame(IplImage *&img)
{
    doPreProcessing(img);

    detection.detectFace(m_resImg, m_resFaceRect);
    
    findPossEyeCoordinate();

    if (m_possEyeRect[0] > 0 && m_possEyeRect[1] > 0 &&
        m_possEyeRect[2] > 0 && m_possEyeRect[3] > 0)
    {

        cvSetImageROI(m_img, cvRect(m_possEyeRect[0], m_possEyeRect[1], 
                    m_possEyeRect[2], m_possEyeRect[3]));

        IplImage *roiImg = cvCreateImage(cvSize(m_possEyeRect[2], m_possEyeRect[3]), 
                                            IPL_DEPTH_8U, m_img->nChannels);

        cvCopy(m_img, roiImg);

        //doBinarization(roiImg);
        cvAdaptiveThreshold(roiImg, roiImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY,
                            25, 10);

        cvShowImage("binary eye..", roiImg);


        cvResetImageROI(m_img);
        cvReleaseImage(&roiImg);
    }
     
    drawRectangle(img);
    cvShowImage("processing..", m_resImg);
}
