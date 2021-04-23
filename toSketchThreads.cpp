#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <cmath>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>

#define THREADS_DFLT 2

using namespace cv;
using namespace std;

static int THREADS = 4;

Mat img, img_gray, img_inv, img_blurred, img_final;
int rows, cols;


void *img2gray(void *arg){
   	int initIterationRow, endIterationRow, initIterationCol, endIterationCol, threadId = *(int *)arg;

	initIterationRow = (floor(rows/THREADS)) * threadId;
	endIterationRow = ((ceil(rows/THREADS)) * (threadId+1));
    for (int i = initIterationRow; i < endIterationRow; i++){
        for (int j = 0; j < cols; j++){
            img_gray.at<uchar>(i, j) = 0.2126 * (float)img.at<cv::Vec3b>(i, j)[2]
                                       + 0.7152 * (float)img.at<cv::Vec3b>(i, j)[1]
                                       + 0.0722 * (float)img.at<cv::Vec3b>(i, j)[0];

            img_inv.at<uchar>(i, j) = 255 - img_gray.at<uchar>(i,j);

        }
    }
    return 0;
}

//Mat imgInv(Mat src, Mat dst){
//    for (int i = 0; i < src.rows; i++){
//        for (int j = 0; j < src.cols; j++){
//            dst.at<uchar>(i, j) = 255 - src.at<uchar>(i,j);
//        }
//    }
//    return dst;
//}


void *imgBlur(void *arg){
    // cout<<"BLUR!!!"<<endl;
    int initIterationRow, endIterationRow, initIterationCol, endIterationCol, threadId = *(int *)arg;

	initIterationRow = (floor(rows/THREADS)) * threadId;
	endIterationRow = ((floor(rows/THREADS)) * (threadId+1));

    float sum = 0;
    int blurSize = 15;
    int s = 0;

    for (int i =  initIterationRow; i < endIterationRow; i++){
        for (int j =  0; j < cols; j++){
            s+=1;
            int ki_start = max(i - blurSize/2, 0);
            int kj_start = max(j - blurSize/2, 0);
            int ki_end = min(i + blurSize/2 + 1, rows);
            int kj_end = min(j + blurSize/2 + 1, cols);

            sum = 0;
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

Mat resize(Mat img, int scale_percent)
{
    float width = int(img.size().width * scale_percent / 100);
    float height = int(img.size().height * scale_percent / 100);
    Size dim = Size(width, height);
    Mat resized;
    resize(img, resized, dim, INTER_AREA);
    return resized;
}


Mat myDodge(Mat img1, Mat img2, Mat dst){
    // cout<<"DODGE!!!"<<endl;
    dst = Mat::zeros(img1.rows, img1.cols, img1.type());
    dst = img1 / (255-img2)*256;
    return dst;
}


int main(int argc, char **argv){

	if(atoi(argv[3]) < 1){
		THREADS = THREADS_DFLT;
		cout<<"Number of threads must be greater than 1"<<endl;
	}else{
		THREADS = atoi(argv[3]);
	}

	int threadId[THREADS], i, *retval;
    pthread_t thread[THREADS];

	struct timeval tval_before, tval_after, tval_result;

    gettimeofday(&tval_before, NULL);

//    img = imread("landscape.jpg", IMREAD_COLOR);
    // img = imread("house.jpg", IMREAD_COLOR);
//    img = imread("imagetest.jpg", IMREAD_COLOR);
//    img = imread("brothers.jpg", IMREAD_COLOR);
    img = imread(argv[1], IMREAD_COLOR);

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

//    img_final = img_gray / (255-img_blured)*256;

    for(i = 0; i < THREADS; i++){
        threadId[i] = i;
        // pthread_create(&thread[i], NULL, (void *)ConvertirGris, &threadId[i]);
		pthread_create(&thread[i], NULL, img2gray, &threadId[i]);
    }

    for(i = 0; i < THREADS; i++){
        pthread_join(thread[i], (void **)&retval);
    }

	for(i = 0; i < THREADS; i++){
        threadId[i] = i;
        // pthread_create(&thread[i], NULL, (void *)Sobelfilter, &threadId[i]);
		pthread_create(&thread[i], NULL, imgBlur, &threadId[i]);
    }

    for(i = 0; i < THREADS; i++){
        pthread_join(thread[i], (void **)&retval);
    }

    // cout<<img_gray.type()<<endl;
    // cout<<img_blurred.type()<<endl;
    img_final = myDodge(img_gray, img_blurred, img_final);
//img_final = img_gray;

	gettimeofday(&tval_after, NULL);
   timersub(&tval_after, &tval_before, &tval_result);
    // printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	printf("%ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);


    if(img.cols > 2000){
        img = resize(img, 25);
        img_final = resize(img_final, 25);
    }else if(img.cols > 1100){
        img = resize(img, 80);
        img_final = resize(img_final, 80);
    }


//    namedWindow( "Image", WINDOW_AUTOSIZE );
    // imshow( "Image", img);
    // imshow( "Image Sketch", img_final);
    imwrite(argv[2], img_final);

    waitKey(0);
//    cout<<img;
    return 0;
}
