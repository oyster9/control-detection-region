#ifndef CALIBRATEREGION_H
#define CALIBRATEREGION_H

#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

extern int g_img_height;
extern int g_img_width;


class CalibrateRegion
{
public:
    CalibrateRegion();
    ~CalibrateRegion(){}

public:
    void selectRegion(Mat& src, Mat &dst);

public:



};

#endif // CALIBRATEREGION_H
