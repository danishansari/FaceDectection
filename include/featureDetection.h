#ifndef FEATUREDETECTION_H
#define FEATUREDETECTION_H

#include <cv.h>
#include <string>
#include <vector>

using namespace std;

class FeatureDetection
{
    string faceCascadeName;
    CvHaarClassifierCascade *faceCascade;

    string eyeCascadeName;
    CvHaarClassifierCascade *eyeCascade;

    CvMemStorage *storage;

    CvSize minFeatureSize;

    int flags;

    float searchScaleFactor;

    public:
        FeatureDetection();
        ~FeatureDetection();

        void detectFace(IplImage *&inpImg, int *faceRect);
        void detectEye(IplImage *&inpImg, int *eyeRect);
};

#endif // FACEDETECTION_H
