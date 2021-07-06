#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define THREADS_DFLT 2
#define THREADS 8
// #define PAD 8

using namespace cv;
using namespace std;

// static int THREADS;

Mat img, img_gray, img_inv, img_blurred, img_final;
int rows, cols;


int img2invgray(Mat img, Mat img_gray, Mat img_inv, int ID){
   	int initRow, endRow;

	initRow = floor(rows/omp_get_num_threads()) * ID;
	endRow = ceil(rows/omp_get_num_threads()) * (ID + 1);
   
    #pragma omp parallel for collapse(2)
    for (int i = initRow; i < endRow; i++){
        for (int j = 0; j < cols; j++){
            img_gray.at<uchar>(i, j) = 0.2126 * (float)img.at<cv::Vec3b>(i, j)[2]
                                       + 0.7152 * (float)img.at<cv::Vec3b>(i, j)[1]
                                       + 0.0722 * (float)img.at<cv::Vec3b>(i, j)[0];
            img_inv.at<uchar>(i, j) = 255 - img_gray.at<uchar>(i,j);
            // cout<<"HOLA"<<endl;

        }
    }
    return 0;
}

int imgBlur(Mat imgBlur, Mat img_inv, int ID){
    // cout<<"BLUR!!!"<<endl;
   	int initRow, endRow;
    // cout<<"ID: "<<ID<<endl;
	initRow = floor(rows/omp_get_num_threads()) * ID;
	endRow = floor(rows/omp_get_num_threads()) * (ID + 1);

    float sum = 0;
    int blurSize = 15;

    // #pragma omp parallel for collapse(2)
    for (int i = initRow; i < endRow; i++){
        for (int j = 0; j < cols; j++){
            int ki_start = max(i - blurSize/2, 0);
            int kj_start = max(j - blurSize/2, 0);
            int ki_end = min(i + blurSize/2 + 1, rows);
            int kj_end = min(j + blurSize/2 + 1, cols);

            sum = 0;
            // #pragma omp parallel for collapse(2)
            for(int ki = ki_start; ki < ki_end; ki++){
                for(int kj = kj_start; kj < kj_end; kj++){
                      sum += img_inv.at<uchar>(ki,kj);
                }
            }
            img_blurred.at<uchar>(i, j) = (sum / (blurSize*blurSize));
        }
    }
    return 0;
}

Mat dodge(Mat img1, Mat img2, Mat dst){
    // cout<<"DODGE!!!"<<endl;
    dst = Mat::zeros(img1.rows, img1.cols, img1.type());
    dst = img1 / (255-img2)*256;
    return dst;
}


int main(int argc, char **argv){

	// try{
    //     if(atoi(argv[3]) > 0){
    //         THREADS = atoi(argv[3]);
            
    //     }else{
    //         cout<<"Number of threads must be greater than 1"<<endl;
    //         return -1;
    //     }
    // } catch (...) {
    //     cout<<"Error reading console argumments, remember:"<<endl;
    //     cout<<"1. Input image. \n2. Output Image. \n3. Number of threads to apply the filter"<<endl;
    //     return -1;
    // }

	int i, threads = atoi(argv[3]);

	struct timeval tval_before, tval_after, tval_result;

    gettimeofday(&tval_before, NULL);

    Mat img = imread(argv[1], IMREAD_COLOR);

    if (!img.data){
        cout << "Image not found or unable to open" << endl;
        return -1;
    }
    
    rows = img.rows;
    cols = img.cols;

    img_gray = Mat::zeros(rows, cols, CV_8UC1);
    img_inv = Mat::zeros(rows, cols, CV_8UC1);
    img_final = Mat::zeros(rows, cols, CV_8UC1);
    img_blurred = Mat::zeros(rows, cols,CV_8UC1);
    
    #pragma omp parallel num_threads(threads)
    {
        int ID = omp_get_thread_num();
        img2invgray(img, img_gray, img_inv, ID);
    }

    #pragma omp parallel num_threads(threads)
    {
        int ID = omp_get_thread_num();
        imgBlur(img_blurred, img_inv, ID);
    }
    

    img_final = dodge(img_gray, img_blurred, img_final);
    
	gettimeofday(&tval_after, NULL);
    timersub(&tval_after, &tval_before, &tval_result);

	printf("%ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);

    // namedWindow( "Image", WINDOW_AUTOSIZE );
    // imshow( "Image", img);
    // imshow( "Image Sketch", img_final);

    imwrite(argv[2], img_final);

    waitKey(0);
    return 0;
}
