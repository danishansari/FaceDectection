#ifndef DROWSYNESS_DETECTION
#define DROWSYNESS_DETECTION

#include <cv.h>

#include "featureDetection.h"

using namespace cv;

class DrowsynessDetection
{
    int m_width;
    int m_height;
    int m_nChannels;

    IplImage *m_img;
    IplImage *m_resImg;

    int m_binThreash;
    
    int m_resFaceRect[4];
    int m_actualFaceRect[4];
    int m_possEyeRect[4];
    int m_resEyeRect[4];
    int m_actualEyeRect[4];

    FeatureDetection detection;

    void doBinarization(IplImage *&img);
    void doPreProcessing(IplImage *&img);
    void drawRectangle(IplImage *&img);
    void findPossEyeCoordinate();
    void resizeImage();
    void faceDetection();
    //void detectEyeState();
    //void detectFacialState();
    //void estimateFatigue();

    public:
        DrowsynessDetection();
        ~DrowsynessDetection();

        void processFrame(IplImage *&img);
        //void getResult();
};

#endif //DROWSYNESS_DETECTION
