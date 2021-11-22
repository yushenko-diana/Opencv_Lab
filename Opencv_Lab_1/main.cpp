#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


void rectangle_drawing(cv::Point pnt, cv::Mat img);


int main( int argc, char** argv ) { 
    cv::Mat copyOfImage;
    cv::Point currPoint;
    
    cv::Mat image;
    image = cv::imread("grass.jpg");
    currPoint = cv::Point(0, image.rows / 2);

    while(1) {
        cv::Mat copyOfImage;
        currPoint = cv::Point(currPoint.x + 1, (sin(((double)currPoint.x + 1.0) / 40) * 200 + image.rows / 2));
        copyOfImage = image.clone();
        cv::circle(image,cv::Point(currPoint.x, currPoint.y + 80), 0, cv::Scalar(0,0,0), 5, 8);
        rectangle_drawing(currPoint, copyOfImage);
        cv::waitKey(20);


        if(currPoint.x == (image.cols / 2)) {
            cv::imwrite("sinTrack.jpg", copyOfImage);
        }
    }
    
    
    return 0;
}

void rectangle_drawing(cv::Point pnt, cv::Mat img) {
    cv::rectangle(img,cv::Point(pnt.x, pnt.y),cv::Point(pnt.x + 60, pnt.y + 80), cv::Scalar(0,0,255),5,8,0);
    cv::imshow( "LabWork_1_Yushenko_Diana", img);
}
