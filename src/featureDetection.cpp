#include "featureDetection.h"

using namespace cv;

FeatureDetection::FeatureDetection() 
{ 
    faceCascadeName = "data/haarcascade_frontalface_alt.xml";
    faceCascade = (CvHaarClassifierCascade*) cvLoad(faceCascadeName.c_str());

    eyeCascadeName = "data/haarcascade_eye.xml";
    eyeCascade = (CvHaarClassifierCascade*) cvLoad(eyeCascadeName.c_str());

    storage = cvCreateMemStorage();

    minFeatureSize = cvSize(20, 20);

    flags = CV_HAAR_SCALE_IMAGE | CV_HAAR_DO_ROUGH_SEARCH;

    searchScaleFactor = 1.1f;
}

FeatureDetection::~FeatureDetection() 
{ 
    cvReleaseMemStorage(&storage);
    cvFree(&faceCascade);
    cvFree(&eyeCascade);
}

void FeatureDetection::detectFace(IplImage *&inpImg, int *faceRect)
{
    CvSeq *faces = cvHaarDetectObjects(inpImg, faceCascade, storage, 
                    searchScaleFactor, 3, flags, minFeatureSize);

    if (faces->total > 0)
    {
        CvRect *rect = (CvRect *)cvGetSeqElem(faces, 0);

        faceRect[0] = rect->x;
        faceRect[1] = rect->y;
        faceRect[2] = rect->width;
        faceRect[3] = rect->height;
    }
    else
        memset(faceRect, -1, sizeof(faceRect));
}

void FeatureDetection::detectEye(IplImage *&inpImg, int *eyeRect)
{
    CvSeq *eyes = cvHaarDetectObjects(inpImg, eyeCascade, storage, 
                    searchScaleFactor, 3, flags, minFeatureSize);
    if (eyes->total > 0)
    {
        CvRect *rect = (CvRect *)cvGetSeqElem(eyes, 0);

        eyeRect[0] = rect->x;
        eyeRect[1] = rect->y;
        eyeRect[2] = rect->width;
        eyeRect[3] = rect->height;
        cout << "Eye Detected -------" << endl;
    }
    else
        memset(eyeRect, -1, sizeof(eyeRect));
}
