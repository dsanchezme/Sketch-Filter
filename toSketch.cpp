#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;


Mat img2gray(Mat src, Mat dst){
    for (int i = 0; i < src.rows; i++){
        for (int j = 0; j < src.cols; j++){
            dst.at<uchar>(i, j) = 0.2126 * (float)src.at<cv::Vec3b>(i, j)[2]
                                       + 0.7152 * (float)src.at<cv::Vec3b>(i, j)[1]
                                       + 0.0722 * (float)src.at<cv::Vec3b>(i, j)[0];
        }
    }
    return dst;
}

Mat imgInv(Mat src, Mat dst){
    for (int i = 0; i < src.rows; i++){
        for (int j = 0; j < src.cols; j++){
            dst.at<uchar>(i, j) = 255 - src.at<uchar>(i,j);
        }
    }
    return dst;
}


Mat imgBlur(Mat src, Mat dst){
//    cout<<"BLUR!!!"<<endl;
    dst = Mat::zeros(src.rows, src.cols, src.type());
    float sum = 0;
    int blurSize = 9;
    int s = 0;

    for (int i =  0; i < dst.rows; i++){
        for (int j =  0; j < dst.cols; j++){
            s+=1;
            int ki_start = max(i - blurSize/2, 0);
            int kj_start = max(j - blurSize/2, 0);
            int ki_end = min(i + blurSize/2 + 1, dst.rows);
            int kj_end = min(j + blurSize/2 + 1, dst.cols);

            sum = 0;
            for(int ki = ki_start; ki < ki_end; ki++){
                for(int kj = kj_start; kj < kj_end; kj++){
                      sum += src.at<uchar>(ki,kj);
                }
            }
             dst.at<uchar>(i, j) = (sum / (blurSize*blurSize));
        }
    }
    return dst;
}

Mat resize(Mat img, int scale_percent)
{
    float width = int(img.size().width * scale_percent / 100);
    float height = int(img.size().height * scale_percent / 100);
    Size dim = Size(width, height);
    Mat resized;
    resize(img, resized, dim, INTER_AREA);
    return resized;
}


Mat dodge(Mat img1, Mat img2, Mat dst){
//    cout<<"DODGE!!!"<<endl;
    dst = Mat::zeros(img1.rows, img1.cols, img1.type());
    dst = img1 / (255-img2)*256;
    return dst;
}


int main(int argc, char **argv){

    Mat img;
    img = imread(argv[1], IMREAD_COLOR);

    if (!img.data){
        cout << "Image not found or unable to open" << endl;
        return -1;
    }

    Mat img_gray = Mat::zeros(img.rows, img.cols, CV_8UC1);
    Mat img_inv = Mat::zeros(img.rows, img.cols, CV_8UC1);
    Mat img_final = Mat::zeros(img.rows, img.cols, CV_8UC1);
    Mat img_blured = Mat::zeros(img.rows, img.cols,CV_8UC3);

    img_gray = img2gray(img, img_gray);
    img_inv = imgInv(img_gray, img_inv);

    img_blured = imgBlur(img_inv, img_blured);
    img_final = dodge(img_gray, img_blured, img_final);

    // namedWindow( "Image", WINDOW_AUTOSIZE );
    // imshow( "Image", img);
    // imshow( "Image Sketch", img_final);

    imwrite(argv[2], img_final);

    waitKey(0);
    return 0;
}
