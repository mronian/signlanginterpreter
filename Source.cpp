#include "stdafx.h"
//#include <iostream>
//#include <highgui.h>
//#include <cv.h>
//#include <cxcore.h>
//#include <vector>
//#include "tserial.h"
//#include "bot_control.h"
//#define DATA(image,i,j,k) *((uchar *)&image->imageData[(i)*(image->widthStep) + (j)*(image->nChannels) + (k)])
//#define FOR(i,a) for(size_t i=0;i<(size_t)(a);i++)
//
//using namespace std;
//using namespace cv;
//#define cam_index 1
//
//serial comm;
//
//int main()
//{
//	cvNamedWindow("Cam", CV_WINDOW_AUTOSIZE);
//	CvCapture* capture=cvCaptureFromCAM(cam_index);
//	comm.startDevice("\\\\.\\COM11", 9600);
//	char c;
//
//		if(!capture)
//	{
//		cout<<"camera not connected";
//		cvWaitKey(0);
//	}
//    IplImage *frame;
//	do{
//		frame=cvQueryFrame(capture);
//	}while(!frame);
//
//	int bin_threshold=0;
//	int erode=0;
//	cvCreateTrackbar("Erode", "Cam", &erode, 20, NULL);
//	cvCreateTrackbar("Binary Threshold", "Cam", &bin_threshold, 255, NULL);
//	while(1)
//	{
//	frame=cvQueryFrame(capture);
//	if(!frame) break;
//	cvShowImage("Cam", frame);
//	char c=cvWaitKey(33);
//	if(c==27) break;
//	else
//	{
//		switch(c)
//		{
//		case 'a':
//		case 'w':
//		case 'd':
//		case 's':
//		case 'o':
//			comm.send_data(c);
//			break;
//		default:
//			printf("Inv");
//		}
//	}
//	}
//	cvReleaseCapture(&capture);
//	cvReleaseImage(&frame);
//	cvDestroyWindow("Cam");
//	comm.stopDevice();
//
//}