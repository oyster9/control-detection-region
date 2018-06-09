#include "calibrateregion.h"

static Point pre_point = Point(-1, -1);
static Point cur_point = Point(-1, g_img_height);

int car_front_edge_y = g_img_height * 5.0 / 6;
Point vanish_point = Point(g_img_width/2, g_img_height/3);

void onMouse(int event, int x, int y, int flags, void *param){


    if(event == CV_EVENT_LBUTTONDOWN){
        pre_point = Point(x, y);
    }
    else if(event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON)){
        //if the mouse move in the bottom half part of img,control the car front edge line
        if(y > g_img_height*2/3 && y < g_img_height && x < g_img_width/2-50){
            cur_point = Point(x, y);
        }
        //if the mouse move in the upper half part of img, control the vanish point
        else if(y > 0 && y < g_img_height*2/3){
            vanish_point.y = y;
        }
    }
    else if(event == CV_EVENT_LBUTTONUP){
        car_front_edge_y = cur_point.y;
    }
}

CalibrateRegion::CalibrateRegion()
{

}

//=============Select the detectin region===========
void CalibrateRegion::selectRegion(Mat &src, Mat &dst){
    namedWindow("video");
    setMouseCallback("video", onMouse, 0);

    //car front edge points and horizontal line
    circle(src, cur_point, 5, Scalar(0,0,255), -1);
    circle(src, Point(src.cols-cur_point.x, cur_point.y), 5, Scalar(0,0,255), -1);

    line(src, Point2d(0, cur_point.y), Point2d(src.cols, cur_point.y),
         Scalar(255,0,0), 2, 8);

    //vanish point
    circle(src, vanish_point, 5, Scalar(255,0,0), 1);
    line(src, Point(vanish_point.x-20, vanish_point.y), Point(vanish_point.x+20, vanish_point.y),
         Scalar(0,255,0), 1);
    line(src, Point(vanish_point.x, vanish_point.y-20), Point(vanish_point.x, vanish_point.y+20),
         Scalar(0,255,0), 1);

    //--------------get the detect region--------------
    //use cv::fillPoly to get the trapezoid area
    //-------------------------------------------------
    //here, slope will never equal to 0 or infinite
    float slope = (vanish_point.y - cur_point.y)*1.0 / (vanish_point.x - cur_point.x);
    Point left_up;
    left_up.y = (cur_point.y-vanish_point.y) * 4.0 / 10 + vanish_point.y;
    left_up.x = 1.0/slope * (left_up.y-vanish_point.y) + vanish_point.x;

    //    Point left_up = Point((vanish_point.x-cur_point.x)*6/10+cur_point.x, (cur_point.y-vanish_point.y)*2/10+vanish_point.y);
    Point right_up = Point(src.cols-left_up.x, left_up.y);

    Point root_points[1][4];
    root_points[0][0] = left_up;
    root_points[0][1] = right_up;
    root_points[0][2] = Point(src.cols-cur_point.x, cur_point.y);
    root_points[0][3] = cur_point;

    const Point* ppt[1] = {root_points[0]};
    int npt[] = {4};
    //    polylines(src, ppt, npt,1,1,Scalar(255,0,0),1);

    Mat mask1, mask2;

    //just show the detection region in the other image
    src.copyTo(mask2);
    mask2.setTo(Scalar::all(0));
    fillPoly(mask2, ppt, npt, 1, Scalar(255, 255, 255));
    src.copyTo(dst, mask2);

    //mask the region in the original image
    mask1 = src.clone();
    fillPoly(mask1, ppt, npt, 1, Scalar(0,255,0));
    addWeighted(src, 0.7, mask1, 0.3, 0, src);

    //    imshow("video", src);

}

