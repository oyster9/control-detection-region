#include <iostream>
#include<opencv2/opencv.hpp>
#include"calibrateregion.h"

using namespace std;
using namespace cv;

int g_img_height = 432;
int g_img_width = 768;

string video_path = "C:\\Pojects\\yaxian\\HPIM0056.mov";

int main()
{
    VideoCapture cap(video_path);
    if(!cap.isOpened()){
        throw runtime_error("The video is not opened!");
        return -1;
    }

    CalibrateRegion _selectRegion;

    while(1){

        double t = (double)getTickCount();//start time

        Mat frame, dst;
        cap >> frame;
        if(frame.empty()){
            cout << "End of video..." << endl;
            return 1;
        }

        resize(frame, frame, Size(), 0.6, 0.6);
        cout << "frame.size: " << frame.size() << endl;

        _selectRegion.selectRegion(frame,dst);

        //calculate the program consuming time
        t = (double)getTickCount() - t;
        cout <<"time: " <<  t*1000. / getTickFrequency() << "ms" << endl;

        imshow("video", frame);
        imshow("region", dst);

        int key = waitKey(20);
        if(key == 32){
            waitKey(0);
        }
    }

    return 0;
}

