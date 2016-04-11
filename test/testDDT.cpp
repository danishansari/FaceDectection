#include <iostream>

#include <highgui.h>

#include "drowsynessDetection.h"

using namespace std;

int main(int argc, char**argv)
{
    if(argc < 2)
        return -1;

    DrowsynessDetection drowsyDetect;

    IplImage *frame;

    CvCapture *capture = cvCaptureFromCAM(0);
    frame = cvQueryFrame(capture);

    int width = frame->width;
    int height = frame->height;

    cout << "CAMERA_WIDTH = " << width << " CAMERA_HEIGHT = " << height << endl;

    while(true)
    {
        frame = cvQueryFrame(capture);

        drowsyDetect.processFrame(frame);
        
        cvShowImage("CAM 0", frame);

        char ch = cvWaitKey(30);
        if((int)ch == 27) break;
    }

    cvReleaseCapture(&capture);
    cvReleaseImage(&frame);

    return 0;
}
