#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;

#define TASK 1

void blurPixel(Mat* newImg, int x, int y, int size);
void boxFiltering(Mat* img, int kernelSize);
void imageDifferent(Mat* orig, Mat* blr);
void filterComparison(void);
void unsharpMask(Mat& src, Mat& blured, Mat& dst, int k);
Mat laplasFilter(Mat* src);

Mat blurImg;
Mat imDiff;

int main(int argc, char** argv ) { 

    TickMeter exeTime;

    Mat origImage;
    origImage = imread("Lenna.png");

    Mat grayImage;
    cvtColor(origImage, grayImage, COLOR_BGR2GRAY);

    #if TASK == 1
    exeTime.start();
    boxFiltering(&grayImage, 3);
    exeTime.stop();
    std::cout<< "boxFiltering: " << exeTime.getTimeMilli()<< std::endl;
    imshow("blurLenna.png", blurImg);

    Mat opencvBlurImage;
    exeTime.start();
    blur(grayImage,opencvBlurImage,Size(3,3),Point(-1,-1));   
    exeTime.stop();
    std::cout<< "opencv blur: " << exeTime.getTimeMilli()<< std::endl;
    imshow("open.png", opencvBlurImage);

    imageDifferent(&blurImg, &opencvBlurImage);
    imshow("dif.png", imDiff);
    #endif

    #if TASK == 2
    Mat gauss;
    Mat boxFltr;
    boxFilter(grayImage,boxFltr, -1, Size(3, 3));
    GaussianBlur(grayImage, gauss, Size(3, 3), 0);
    imageDifferent(&gauss, &boxFltr);
    imshow("boxFilter", boxFltr);
    imshow("gauss", gauss);
    imshow("diff", imDiff);
    #endif

    #if TASK == 3
    Mat gauss;
    GaussianBlur(grayImage, gauss, Size(3, 3), 0);
    Mat boxFltr;
    boxFilter(grayImage,boxFltr, -1, Size(3, 3));
    Mat dest1;
    Mat dest2;
    unsharpMask(grayImage,gauss, dest1, 2);
    unsharpMask(grayImage,boxFltr,dest2, 2);
    imageDifferent(&dest1, &dest2);
    imshow("gaussUnsharp", dest1);
    imshow("boxFilterUnsharp", dest2);
    imshow("diff", imDiff);
    #endif

    #if TASK == 4
    Mat dest;
    Mat dest1;
    dest = laplasFilter(&grayImage);
    Laplacian(grayImage,dest1,-1);
    imshow("laplassFilter", dest);
    imshow("laplacian", dest);
    #endif

    #if TASK == 5
    Mat dest, laplas, gauss,boxFltr, dest1, dest2;
    laplas = laplasFilter(&grayImage);
    GaussianBlur(grayImage, gauss, Size(3, 3), 0);
    boxFilter(grayImage,boxFltr, -1, Size(3, 3));

    unsharpMask(grayImage,laplas, dest, 2);
    unsharpMask(grayImage,gauss, dest1, 2);
    unsharpMask(grayImage,boxFltr,dest2, 2);

    imshow("gaussUnsharp", dest1);
    imshow("boxFilterUnsharp", dest2);
    imshow("laplasUnsharp", dest);
    #endif

    waitKey(0);
    return 0;
}

void blurPixel(Mat* newImg, int x, int y, int size) {
    int sum = 0;
    int rCnt, colCnt;
    for(colCnt = x; colCnt < (x + size); colCnt++) {
        for (rCnt = y; rCnt < (y + size); rCnt++) {
            sum += newImg->at<uchar>(rCnt, colCnt);
        }
    }
    newImg->at<uchar>(y + (size / 2), x + (size / 2)) = (sum / (size * size));
}
void boxFiltering(Mat* img, int kernelSize) {
    blurImg = img->clone();
    int iRows = img->rows;
    int iCols = img->cols;
    int i,j;
    for( i = 0; i < (iRows - kernelSize); i++) {
        for ( j = 0; j < (iCols - kernelSize); j++) {
            blurPixel(&blurImg, i, j, kernelSize);
        }
    }
}


void imageDifferent(Mat* orig, Mat* blr){
    float dif;
    float sumDif = 0;
    imDiff = orig->clone();
    for( int l = 0; l < orig->rows; l++) {
        for (int m = 0; m < orig->cols; m++) {
            dif = abs((orig->at<uchar>(l,m) - blr->at<uchar>(l,m)));
            sumDif += dif;
            imDiff.at<uchar>(l,m) = (uint)dif;
        }
    }
    #if TASK == 1
    float differ, sim;
    differ = (sumDif / 255) * 100;
    sim = 100 - (differ/ (blurImg.cols * blurImg.rows));
    std::cout<< "Сходство: "<<  sim << std::endl;
    #endif
}

void unsharpMask(Mat& src, Mat& blured, Mat& dst, int k) {
    Mat sub;
    dst = src - blured;
    dst =src + k * dst;
}

Mat laplasFilter(Mat* src){
    int sum;
    int rCnt, colCnt;
    int laplasOperator[3][3] = {{0,1,0},{1,-4,1},{0,1,0}};
    Mat laplasImage;
    laplasImage = src->clone();
    
    for(int j= 1; j < (src->rows - 1);j++) {
        for (int i = 1; i < (src->cols - 1); i++) {
            sum = src->at<uchar>(j-1,i) + src->at<uchar>(j+1,i) + 
            src->at<uchar>(j,i-1)+src->at<uchar>(j,i+1)-4*src->at<uchar>(j,i);
            if(sum > 255){
                sum = 255;
            }
            if(sum < 0){
                sum = 0;
            }
            laplasImage.at<uchar>(j,i) = sum;
        }
    }
    return laplasImage;
}
