#include "stdafx.h"
#include <iostream>
#include <highgui.h>
#include <cv.h>
#include <cxcore.h>
#include "opencv2/opencv.hpp"
#include <vector>
#include "tserial.h"
#include "bot_control.h"

using namespace std;
using namespace cv;
//////
//////#define CAM_INDEX -1
//////#define BOTTOM 1/3
//////#define TURNORLANE 1.5
//////#define GFORARROW 190
//////#define RFORSTOP 190
//////#define BDONTCARE 120

RNG rng(12345);
#define DATA(A,i,j,k)  A.data[A.step*i + A.channels()*j + k]
//////#define FOR(i,a) for(size_t i=0;i<(size_t)(a);i++)
//////
//////using namespace std;
//////using namespace cv;
//////
//////void checkLaneOrTurn(IplImage*, IplImage*);
//////IplImage* getbottom(IplImage*);
//////void Lane(IplImage* , IplImage* );
//////void TurnOrStop(IplImage*, IplImage*);
//////int GetArrowDirection(IplImage* bin);
//////void isTurn(IplImage* bin, IplImage* frame);
//////void isStop();
//////double GetRatio(IplImage*);
//////
serial comm;
int Threshx, Threshy;
void mouse_callback(int event, int x, int y, int flags, void* param)
{
	//This is called every time a mouse event occurs in the window
	if (event == CV_EVENT_LBUTTONDOWN) { //This is executed when the left mouse button is clicked
		//Co-ordinates of the left click are assigned to global variables and flag is set to 1
		Threshx = x;
		Threshy = y;
	}

}

Mat getbinary(cv::Mat img, int t){

	Mat bin; //To store and return the result
	bin = img.clone();
 
	for(int i=0; i < img.rows; i++) {
		for(int j=0; j < img.cols; j++) {
			//It is checked if the gray-scale equivalent is larger than the threshold or not
			if(DATA(img,i,j,0) > t)
				DATA(bin,i,j,0)  = 255;//Assigned white
			else
				DATA(bin,i,j,0) = 0;//Assigned black
		}
	}
	return bin; //Result is returned
}

int main()
{
	VideoCapture cap(0);
	int lt=100,ht=199;
	char c;
	comm.startDevice("\\\\.\\COM11", 9600);
	vector<vector<cv::Point> > f;
	vector<vector<cv::Point> > le;
	vector<vector<cv::Point>> points;
	vector<Vec4i> hierarchy;
	int found=-10;
	const String img_name[][6]={{"a1.jpg","a2.jpg","a3.jpg","a4.jpg","a5.jpg","a6.jpg"},
							{"b1.jpg","b2.jpg","b3.jpg","b4.jpg","b5.jpg","b6.jpg"},
							{"c1.jpg","c2.jpg","c3.jpg","c4.jpg","c5.jpg","c6.jpg"},
							{"d1.jpg","d2.jpg","d3.jpg","d4.jpg","d5.jpg","d6.jpg"},
							{"e1.jpg","e2.jpg","e3.jpg","e4.jpg","e5.jpg","e6.jpg"},
							{"f1.jpg","f2.jpg","f3.jpg","f4.jpg","f5.jpg","f6.jpg"},
							{"g1.jpg","g2.jpg","g3.jpg","g4.jpg","g5.jpg","g6.jpg"},
							{"h1.jpg","h2.jpg","h3.jpg","h4.jpg","h5.jpg","h6.jpg"},
							{"i1.jpg","i2.jpg","i3.jpg","i4.jpg","i5.jpg","i6.jpg"},
							{"j1.jpg","j2.jpg","j3.jpg","j4.jpg","j5.jpg","j6.jpg"},
							{"k1.jpg","k2.jpg","k3.jpg","k4.jpg","k5.jpg","k6.jpg"},
							{"l1.jpg","l2.jpg","l3.jpg","l4.jpg","l5.jpg","l6.jpg"},
							{"m1.jpg","m2.jpg","m3.jpg","m4.jpg","m5.jpg","m6.jpg"},
							{"n1.jpg","n2.jpg","n3.jpg","n4.jpg","n5.jpg","n6.jpg"},
							{"o1.jpg","o2.jpg","o3.jpg","o4.jpg","o5.jpg","o6.jpg"},
							{"p1.jpg","p2.jpg","p3.jpg","p4.jpg","p5.jpg","p6.jpg"},
							{"q1.jpg","q2.jpg","q3.jpg","q4.jpg","q5.jpg","q6.jpg"},
							{"r1.jpg","r2.jpg","r3.jpg","r4.jpg","r5.jpg","r6.jpg"},
							{"s1.jpg","s2.jpg","s3.jpg","s4.jpg","s5.jpg","s6.jpg"},
							{"t1.jpg","t2.jpg","t3.jpg","t4.jpg","t5.jpg","t6.jpg"},
							{"u1.jpg","u2.jpg","u3.jpg","u4.jpg","u5.jpg","u6.jpg"},
							{"v1.jpg","v2.jpg","v3.jpg","v4.jpg","v5.jpg","v6.jpg"},
							{"w1.jpg","w2.jpg","w3.jpg","w4.jpg","w5.jpg","w6.jpg"},
							{"x1.jpg","x2.jpg","x3.jpg","x4.jpg","x5.jpg","x6.jpg"},
							{"y1.jpg","y2.jpg","y3.jpg","y4.jpg","y5.jpg","y6.jpg"},
							{"z1.jpg","z2.jpg","z3.jpg","z4.jpg","z5.jpg","z6.jpg"}};
	int a[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	if ( !cap.isOpened() )  //if not success, exit program
    {
         cout << "Cannot open the video file" << endl;
         return -1;
    }
	namedWindow("Main", CV_WINDOW_AUTOSIZE);
	namedWindow("ROI", CV_WINDOW_AUTOSIZE);
	namedWindow("ROICon", CV_WINDOW_AUTOSIZE);
	namedWindow("Test", CV_WINDOW_AUTOSIZE);
	Mat original;
	Mat roi;
	Mat con;
	Mat learn;
	int size=0;
	int count=0;
	CvKNearest knn;
	cap.read(original);
	Scalar col;
	Rect roirect = Rect(100,100,original.cols/4, original.rows/4);
	cvCreateTrackbar("Low-thresh","Main",&lt,255); 
	cvCreateTrackbar("Hi-thresh","Main",&ht,255);
	for(;;){


	cap.read(original); // get a new frame from camera
	cv::flip(original, original, 1);
	roi=original(roirect);
	con=roi;
	// 1-channel convert
	if(roi.channels() == 3) {
		cv::cvtColor(roi, roi, CV_RGB2GRAY);
	} else if(roi.channels() == 4) {
		cv::cvtColor(roi, roi, CV_RGBA2GRAY);
	}
 
	//original=getbinary(original, threshold);
	Canny(roi,roi,lt,ht,3);
	cv::Mat dilateKernel(cv::Size(2,2), CV_8UC1, cv::Scalar(1));
	cv::dilate(roi, roi, dilateKernel);
    
	char text[255]; 
	sprintf(text, "%c", (char)(found+97));

	CvFont font;
	double hScale=1.0;
	double vScale=1.0;
	int    lineWidth=1;
	col= Scalar( 0,0,255 );
	cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);

	if(found!=-10)putText (original, text, cvPoint(20,100), 1, 8, col, 8);
	rectangle(original, Point(100,100), Point(100+original.cols/4, 100+original.rows/4), Scalar(255,0,0), 2);
	imshow("Main", original);
	imshow("ROI", roi);
	c=cvWaitKey(30);
	 if(c== 27) //wait for 'esc' key press for 30 ms. If 'esc' key is pressed, break loop
       {
                cout << "esc key is pressed by user" << endl; 
                break; 
       }
	 else if(c==92)
	 {
		 cv::findContours(roi, f, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0,0));
		 
		 for( int i = 0; i< f.size(); i++ )
     {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       drawContours( con, f, i, color, 1, 8, hierarchy, 0, Point() );
     }
	 imshow("ROICon", con);
	 }
	 else if(c==97){
		imwrite(img_name[(int)c-97][a[c-97]++], roi );
	 }
	 else if(c==44){
		 count=0;
		 for(int k=0;k<3;k++){
		 for(int i=0;i<6;i++){
		
			 learn =imread(img_name[k][i]);
			 imshow("ROICon",learn);
			 cv::cvtColor(learn,learn,CV_RGB2GRAY);
			Canny(learn,learn,lt,ht,3);
			cv::Mat dilateKernel(cv::Size(3,3), CV_8UC1, cv::Scalar(1));
			cv::dilate(learn,learn, dilateKernel);
    
			 cv::findContours(learn, le, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0,0));
			 Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
			drawContours( learn, le, 0, color, 1, 8, hierarchy, 0, Point() );
			
			imshow("ROICon", learn);
			 points.push_back(le[0]);
			 size+=points[count++].size();
			cout<<size<<"\n";
		 }
		 }
		 cout<<size<<"\n";
		 count=0;
		cv::Mat trainingData(size, 2, CV_32FC1);
		cv::Mat trainingClasses(size, 1, CV_32FC1);
		for(int k=0;k<3;k++){
		for(int i=0;i<6;i++){
		for(int j=0;j<points[k*6+i].size();j++) {
			 trainingData.at<float>(count,0) = points[k*6+i][j].x;
			 trainingData.at<float>(count,1) = points[k*6+i][j].y;
			 trainingClasses.at<float>(count++,0) = k; // we label the shape as "1", the next added shape point set should be labeled as "2" and etc.
		}
	 }
		}
		
	knn.train( trainingData, trainingClasses);
	cout<<"sucess";
	 }
	 else if(c==39){
		 
			cv::Mat testData(f[0].size(), 2, CV_32FC1);
	 for(int i=0;i<f[0].size();i++) {
				testData.at<float>(i,0) = f[0][i].x;
				testData.at<float>(i,1) = f[0][i].y;
			}
			cout<<"done";
			found = knn.find_nearest(testData, 1);
	 }
	 cout<<found<<"\n";
	 comm.stopDevice();
	}
	// the camera will be deinitialized automatically in VideoCapture destructor


	//IplImage* cam_smooth=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
	//IplImage* cam_gray=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	//IplImage* cam_bin=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
	//
	/*int bin_threshold=0;
	int erode=0;
	cvCreateTrackbar("Erode", "Cam", &erode, 20, NULL);
	cvCreateTrackbar("Binary Threshold", "Cam", &bin_threshold, 255, NULL);
	*/
	//while(1)
	//{
	////frame=cvQueryFrame(capture);
	////if(!frame) break;
	//cvShowImage("Cam", frame);
	////cvCvtColor(frame, cam_gray, CV_RGB2GRAY);
	////cvThreshold(cam_gray, cam_bin, bin_threshold, 255, CV_THRESH_BINARY);
	////cvErode(cam_bin, cam_bin, NULL, erode);
	////cvShowImage("Cam_Smooth", cam_bin);
	//char c=cvWaitKey(33);
	//if(c==27) break;
	//}
	////cvReleaseCapture(&capture);
	//cvReleaseImage(&frame);
	////cvReleaseImage(&cam_smooth);
	//cvDestroyWindow("Cam");
	//cvDestroyWindow("Cam_Smooth");

}
//////
//////void checkLaneOrTurn(IplImage* bin, IplImage* frame)
//////{
//////	IplImage* bottom=getbottom(bin);
//////	double ratio=GetRatio(bottom);
//////	if(ratio>TURNORLANE)
//////		TurnOrStop(bin, frame);
//////	else
//////		Lane(bin, frame);
//////}
//////
//////IplImage* getbottom(IplImage* bin)
//////{
//////	IplImage* bottom;
//////	cvSetImageROI(bin, cvRect(0, (bin->height)*BOTTOM, bin->width, (bin->height)*(1-BOTTOM)));
//////	IplImage* tmp = cvCreateImage(cvGetSize(bin),bin->depth,bin->nChannels);
//////	cvCopy(bin, tmp, NULL);
//////	cvResetImageROI(bin);
//////	bottom = cvCloneImage(tmp);	
//////	return bottom;
//////}
//////
//////void Lane(IplImage* bin, IplImage* frame)
//////{
//////	vector<Vec4i> lines;
//////	HoughLinesP(bin, lines, 1, CV_PI/180, 50, 50, 10 );
//////	for( size_t i = 0; i < lines.size(); i++ )
//////	{
//////		Vec4i l = lines[i];
//////		cout<<l[0]<<" "<<l[1]<<" "<<l[2]<<" "<<l[3]<<"\n";
//////	}
//////	//Addslopes to get 0
//////	//Make Bot move as per equirement
//////}
//////
//////void TurnOrStop(IplImage* bin, IplImage* frame)
//////{
//////
//////	int greencnt=0, redcnt=0, i, j;
//////
//////	FOR(i, frame->height)
//////	{
//////		FOR(j, frame->width)
//////		{
//////			if((DATA(frame, i, j, 1)>GFORARROW)&&(DATA(frame, i, j, 1)<RFORSTOP*0.5)&&(DATA(frame, i, j, 0)<BDONTCARE))
//////				greencnt++;
//////			else if((DATA(frame, i, j, 1)<GFORARROW*0.5)&&(DATA(frame, i, j, 1)>RFORSTOP)&&(DATA(frame, i, j, 0)<BDONTCARE))
//////				redcnt++;
//////		}
//////	}
//////
//////	if(greencnt>redcnt*10)isTurn(bin, frame);
//////	else if(redcnt>greencnt*10)isStop();
//////
//////}
//////
//////void isTurn(IplImage* bin, IplImage* frame)
//////{
//////	//GetArrow Direction 0 left 1 right
//////	//TurnBot
//////	//MakeSureitgetsbackonlane
//////	//return
//////}
//////
//////int GetArrowDirection(IplImage* frame)
//////{
//////	IplImage* arrow=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
//////	FOR(i, frame->height)
//////	{
//////		FOR(j, frame->width)
//////		{
//////			if((DATA(frame, i, j, 1)>GFORARROW)&&(DATA(frame, i, j, 1)<RFORSTOP*0.5)&&(DATA(frame, i, j, 0)<BDONTCARE))\
//////			{
//////				DATA(arrow, i, j, 0)=255;
//////			}
//////			else
//////			{
//////				DATA(arrow, i, j, 0)=0;
//////			}
//////		}
//////	}
//////
//////	//Get Hough Line Direction
//////}
//////
//////void isStop()
//////{
//////	//motor_stop();
//////}
//////
//////double GetRatio(IplImage* bin)
//////{
//////	double ratio;
//////
//////	int i, j, black=0, white=0;
//////
//////	FOR(i, bin->height)
//////	{
//////		FOR(j, bin->width)
//////		{
//////			if(DATA(bin, i, j, 0)==0)
//////				black++;
//////			else
//////				white++;
//////		}
//////	}
//////	ratio=black/white;
//////	return ratio;
//////}
////
////
////

//#include "opencv2/opencv.hpp"
//#include <iostream>
//#include <cstdio>
//#include <algorithm>
//#include "cv.hpp"
//#include "stdafx.h"
//#include <highgui.h>
//#include <cv.h>
//#include <cxcore.h>
//#include "tserial.h"
//#include "bot_control.h"
//
//#define MIN_THRESH 500       //For Blob
//#define LIM 5  //For following
//#define DEC_THRESH 90      //For Decision
//#define SYM_THRESH 0.7      //For Symmetry
//#define PI 3.14159265359
//#define IMGDATA(image,i,j,k) *((uchar *)&image->imageData[(i)*(image->widthStep) + (j)*(image->nChannels) + (k)])
//
//using namespace cv;
//using namespace std;
//int shape=0;
//int top=0;
//int **a;
//int **stack;
//serial comm;
//
//
//void push(int a, int b)
//{
//top=top+1;
//stack[top][0]=a;
//stack[top][1]=b;
//}
//
//void pop()
//{
//top=top-1;
//}
//
//void dfs(IplImage* I,int m, int n)
//{
//shape=shape+1;
//push(m,n);
//a[m][n]=0;
//int chk=0;
//int i, j;
//int x, y;
//while(top>0)
//{
//    x=stack[top][0];
//    y=stack[top][1];
//    if(IMGDATA(I,x,y,0)==255)
//       {
//		a[x][y]=shape;
//		chk=0;
//	}
//    for(i=x-1;i<=x+1;i++)
//	{
//		for(j=y-1;j<=y+1;j++)
//		 {
//			 if((j>=(I->width))||(i>=I->height)||(j<0)||(i<0))
//				 continue;
//			if((IMGDATA(I,i,j,0)==255)&&(a[i][j]==-1))
//			{
//					push(i,j);
//					chk=1;
//					break;
//			}
//		}
//        if (chk==1)
//            break;
//	}
//    if(chk==0)
//       {
//		   pop();
//        continue;
//		}
//    if(chk==1)
//		continue;
//}
//}
//
//void getblobs(IplImage* I)
//{
//for(int i=0;i<I->height;i++)
//{
//	for(int j=0;j<I->width;j++)
//	{
//		a[i][j]=-1;
//	}
//}
//
//for(int i=0;i<(I->height)*(I->width);i++)
//{
//	stack[i][0]=0;
//	stack[i][1]=0;
//}
//
//
//int ht=I->height;
//int wd=I->width;
//int r, l;
//
//for(r=1;r<ht-1;r++)
//{
//	for(l=1;l<wd-1;l++)
//	{
//        if( (IMGDATA(I,r,l,0)==255)&&(a[r][l]==-1))
//		{
//            dfs(I,r,l);
//		}
//	}
//}
//}
////This function counts the number of white pixels
//int count(IplImage *img)
//{
//    int MAXROW=img->height,MAXCOL=img->width,count=0;
//    for(int i=0;i<MAXROW;i++)
//    {
//        for(int j=0;j<MAXCOL;j++)
//        {
//            if(IMGDATA(img,i,j,0)==255)count++;
//        }
//    }
//    return count;
//}
//
////Creates Blank Copy
//IplImage *CreateBlankCopyOf(IplImage *img,int n=0)
//{
//    int ht,wd;
//    ht=img->height;
//    wd=img->width;
//    if(n==0)
//    {
//        n=img->nChannels;
//    }
//    return cvCreateImage(cvSize(wd,ht),8,n);
//}
//
////Checks if the image has crossed the boundary of screen
//int check_boundary(IplImage *img)
//{
//    int check=0;
//    int MAXROW=img->height,MAXCOL=img->width;
//    for(int j=0;j<MAXCOL;j++)
//    {
//        if(IMGDATA(img,0,j,0)==255 || IMGDATA(img,MAXROW-1,j,0)==255) return 2;
//    }
//    int flag1=0,flag2=0;
//    for(int i=0;i<MAXROW;i++)
//    {
//        if(IMGDATA(img,i,0,0)==255) flag1=1;
//        if(IMGDATA(img,i,MAXCOL-1,0)==255) flag2=1;
//    }
//    if(flag1==1 && flag2==1) return 2;
//    else if(flag1==1) return -1;
//    else if(flag2==1) return 1;
//    return 0;
//}
//
////Calculates column no. of the mid point of image
//int midcol(IplImage *img)
//{
//	int mid=0,count=0,MAXROW=img->height,MAXCOL=img->width;
//	for(int i=0;i<MAXROW;i++)
//	{
//		for(int j=0;j<MAXCOL;j++)
//		{
//			if(IMGDATA(img,i,j,0)==255)
//			{
//				count++;
//				mid+=j;
//			}
//		}
//	}
//	if(count<50) return -1;
//	mid/=count;
//	return mid;
//
//}
//
////Follows the image(green,red)
//void checkdir(IplImage *green,IplImage *red)
//{
//	int col=(green->width)/2;
//	int mid_green=midcol(green);
//	int mid_red=midcol(red);
//	if (mid_green>0)
//	{
//		if(mid_green>col-50 && mid_green<col+50) puts("Straight");
//		else if(mid_green>col+50) puts("Right");
//		else puts("Left");
//
//	}
//	else if (mid_red>0)
//	{
//		if(mid_red>col-50 && mid_red<col+50) puts("Straight");
//		else if(mid_red>col+10) puts("Right");
//		else puts("Left");
//	}
//	else puts("Straight");
//}
//
////Separate Red,Green,White & Black
//void  colors(IplImage *img,IplImage *red ,IplImage *green,IplImage *white,IplImage *black)
//{
//
//	int i, j, lall=0;
//
//	for(i=0;i<img->height;i++)
//     {
//		 for(j=0;j<img->width;j++)
//		{
//         if((IMGDATA(img,i,j,0)<100)&&(IMGDATA(img,i,j,1)<100)&&(IMGDATA(img,i,j,2)<100))
//         {
//             IMGDATA(red,i,j,0)=0;
//             IMGDATA(green,i,j,0)=0;
//             IMGDATA(white,i,j,0)=0;
//             IMGDATA(black,i,j,0)=255;
//         }
//         else if((IMGDATA(img,i,j,0)>150)&&(IMGDATA(img,i,j,1)>150)&&(IMGDATA(img,i,j,2)>150))
//         {
//             IMGDATA(red,i,j,0)=0;
//             IMGDATA(green,i,j,0)=0;
//             IMGDATA(white,i,j,0)=255;
//             IMGDATA(black,i,j,0)=0;
//         }
//         else if((IMGDATA(img,i,j,1))-(IMGDATA(img,i,j,2))>50)
//         {
//             IMGDATA(red,i,j,0)=0;
//             IMGDATA(green,i,j,0)=255;
//             IMGDATA(white,i,j,0)=0;
//             IMGDATA(black,i,j,0)=0;
//         }
//          else if((IMGDATA(img,i,j,1))-(IMGDATA(img,i,j,2))<-50)
//         {
//             IMGDATA(red,i,j,0)=255;
//             IMGDATA(green,i,j,0)=0;
//             IMGDATA(white,i,j,0)=0;
//             IMGDATA(black,i,j,0)=0;
//         }
//         else
//         {
//             IMGDATA(red,i,j,0)=0;
//             IMGDATA(green,i,j,0)=0;
//             IMGDATA(white,i,j,0)=0;
//             IMGDATA(black,i,j,0)=0;
//         }
//
//     }
//	}
//}
//
//
////For taking vertical projection of the image
//void to_graph_hor(IplImage *binary,IplImage *graph)
//{
//    for(int i=0;i<(binary->width);i++ )
//    {
//        int counter =binary->height-1;
//        for(int j=0;j<(binary->height);j++)
//     {
//        if(IMGDATA(binary,j,i,0))
//        {
//            IMGDATA(graph,counter--,i,0)=255;
//        }
//
//     }
//    }
//}
//
////Finds the direction of the arrow
//int direction(IplImage *img)
//{
//    int i,j;
//    int hor_pix=0,vert_pix=0,count_right=0,count_left=0;
//    int flag=0;
//    for(i=0;i<img->height;i++)
//    {
//       for(j=0;j<img->width;j++)
//        {
//            if(IMGDATA(img,i,j,0)==255)
//            {
//                //printf("hor %d  ver %d\n",hor_pix,vert_pix);
//                hor_pix=j;
//                vert_pix=i;
//                flag=1;
//                break;
//            }
//        }
//    if(flag) break;
//    }
//
//    for(j=hor_pix;j<=hor_pix+20;j++)
//        for(i=0;i<img->height;i++)
//            if(IMGDATA(img,i,j,0)==255)
//                count_right++;
//
//    for(j=hor_pix;j>=hor_pix-20;j--)
//        for(i=0;i<img->height;i++)
//            if(IMGDATA(img,i,j,0)==255)
//                count_left++;
//    //printf("count left=%d\ncount right=%d\n",count_left,count_right);
//
//    if(count_right>count_left)
//        return 1;
//    else if(count_right<count_left)
//        return -1;
//    else return 0;
//
//}
//
////Gives the biggest blob from an image
//void biggest_blobs(IplImage *img,IplImage *blob )//,IplImage *blob2);
//{
//	shape=0;
//	getblobs(img);
//    int max_cnt=0;
//    int big_blob=-1;
//    int i, j, k;
//	long int cnt=0;
//	cnt=count(img);
//	for(k=1;k<=shape;k++)
//	{
//		for (i=0;i<img->height;i++)
//        {
//			for(j=0;j<img->width;j++)
//			{
//				if(a[i][j]==k)
//					{
//						cnt++;			
//					}
//			}
//        }
//		if(cnt>max_cnt)
//		{
//			max_cnt=cnt;
//			big_blob=k;
//		}
//		cnt=0;
//	}
//
//	if(big_blob==-1)
//	{
//	for(int i=0;i<img->height;i++)
//        {
//			for(int j=0;j<img->width;j++)
//			{
//               IMGDATA(blob,i,j,0)=0;
//			}
//		}
//	return;
//	}
//
//    for(int i=0;i<img->height;i++)
//        for(int j=0;j<img->width;j++)
//        {
//            if(a[i][j]==big_blob)
//            {
//                IMGDATA(blob,i,j,0)=255;
//            }
//            else
//            {
//                IMGDATA(blob,i,j,0)=0;
//            }
//        }
//	
//}
//
//int centroid(IplImage* img)
//{
//	int white_count=1, white_wt=1, i, j;
//	for(i=0;i<img->height;i++)
//	{
//		for(j=0;j<img->width;j++)
//		{
//			if(IMGDATA(img, i, j, 0)==255)
//			{
//				white_count++;
//				white_wt+=j;	
//			}
//		}
//	}
//	printf("%d w", white_wt);
//	return (int)(white_wt/white_count);
//
//}
//
//void follow(IplImage *img)
//{
//	//Serial Code
//     int wd=img->width;
//     int x;
//     x=centroid(img);
//     if( x>(wd/2+LIM))
//     {
//         printf("Right %d %d\n", x, wd/2);
//		 comm.send_data('d');
//     }
//     else if(x<(wd/2-LIM))
//     {
//         printf("Left %d %d\n", x, wd/2);
//		 comm.send_data('a');
//     }
//     else
//     {
//         printf("Straight %d %d\n", x, wd/2);
//		 comm.send_data('w');
//     }
//}
//
///*
// * 1. Separate out White,Black,Red,Green colors
// * 2. Find the biggest blobs from all of them
// * 3. Check if there is any big blob in green/red(i.e. any symbol)
// * 4. if yes,then follow the symbol until size of white blob on the ground reduces to a definite value & Analyse the image and start recording decisions
// *    if not, follow white blob on ground till 3. is true
// * 5. Take a turn accordingly.
// * 6. Turn until the size of white blob increases to a definite value.
// * 7. Repeat from step 3.
// */
//
//
//int main()
//{
//    //CvCapture *capture=cvCreateCameraCapture(-1);
//	//CvCapture *capture=cvCaptureFromFile("rd0fin.wmv");
//    CvCapture* capture = cvCaptureFromCAM(-1);
//	comm.startDevice("\\\\.\\COM11", 9600);
//		if(!capture)
//	{
//		cout<<"camera not connected";
//		cvWaitKey(0);
//	}
//    IplImage *real;
//	do{
//		real=cvQueryFrame(capture);
//	}while(!real);
//	cvNamedWindow("C", CV_WINDOW_AUTOSIZE);
//	cvShowImage("C", real);
// 	a=(int **)malloc((real->height)*sizeof(int*));
//	for(int i=0;i<real->height;i++)
//	{
//		a[i]=(int *)malloc((real->width)*sizeof(int));
//	}
//
//	stack=(int **)malloc(((real->height)*(real->width))*sizeof(int*));
//	for(int i=0;i<(real->height)*(real->width);i++)
//	{
//		stack[i]=(int *)malloc(2*sizeof(int));
//	}
//		//Creating blank copies of image
//    IplImage *GREEN=CreateBlankCopyOf(real,1);
//    IplImage *RED=CreateBlankCopyOf(real,1);
//    IplImage *WHITE=CreateBlankCopyOf(real,1);
//    IplImage *BLACK=CreateBlankCopyOf(real,1);
//    IplImage *green=CreateBlankCopyOf(real,1);
//    IplImage *red=CreateBlankCopyOf(real,1);
//    IplImage *white=CreateBlankCopyOf(real,1);
//    IplImage *graph=CreateBlankCopyOf(real,1);
//
//
//    //Creating Windows
//    cvNamedWindow("Real",CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("Green",CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("Red",CV_WINDOW_AUTOSIZE);
//    cvNamedWindow("White",CV_WINDOW_AUTOSIZE);
//
//    while(1)
//    {
//        static int decision[5]={0};   // decision[0] for 'white',[1] for 'left',[2] for 'right',[3] for 'stop'
//        int count_red=0,count_white=0,count_green=0,signal=0;
//        real=cvQueryFrame(capture);
//        if(real==NULL)
//        {
//            printf("Image Not Loaded\n");
//			//Close Serial
//        }
//		
//		printf("/f");
//        colors(real,RED,GREEN,WHITE,BLACK);  
//		cvShowImage("C", real);
//        biggest_blobs(RED,red);					
//        biggest_blobs(GREEN,green);
//        biggest_blobs(WHITE,white);           
//        cvShowImage("Green",green);
//        cvShowImage("White",white);
//        cvShowImage("Red",red);
//		count_green=count(green);
//        count_red=count(red);
//        count_white=count(white);
//
//        if(count_white<MIN_THRESH)
//        {
//            //Checking if bot has to turn or not
//            puts("TIME TO DECISION");
//
//            if(decision[1]>DEC_THRESH)
//            {
//                puts("DECISION MADE");
//                printf("Left\n");
//			  	comm.send_data('a');
//                continue;
//            }
//            else if(decision[2]>DEC_THRESH)
//            {
//                puts("DECISION MADE");
//                printf("Right\n");
//				comm.send_data('d');
//                continue;
//            }
//        }
//
//        //Setting values of decisions to zero;
//        if(count_green<MIN_THRESH && (centroid(white)>white->width/2 || count_red>MIN_THRESH/10 ) )
//        {
//			printf("%d %d ", centroid(white), white->width/2);
//            for(int i=0;i<4;i++) decision[i]=0;
//			printf("\n\ndec0\n\n");
//        }
//        //Checking if any colored symbol is present or not
//        if(count_green>MIN_THRESH && count_green>count_red) signal=1;
//        else if(count_red>MIN_THRESH*50 && count_red>count_green) signal=-1;
//        if(signal==1)
//        {
//            puts("GREEN");
//            for(int i=0;i<graph->height;i++) for(int j=0;j<graph->width;j++) IMGDATA(graph,i,j,0)=IMGDATA(green,i,j,0);
//           // to_graph_hor(green,graph);
//            cvShowImage("Graph",graph);
//            if(direction(graph)==1) decision[2]++;                            //Right Turn
//            else if(direction(graph)==-1) decision[1]++;                      //Left Turn
//            //Follow Image
//            follow(green);
//            puts("FOLLOWING GREEN");
//
//        }
//        else if(signal==-1)
//        {
//            puts("RED");
//			if(count_red>MIN_THRESH*170)
//				{
//					comm.send_data('o');
//					break;
//			}
//			else 
//				{
//					puts("FOLLOWING RED\n");
//					follow(red);
//			}
//           
//        }
//        else if(count_white>MIN_THRESH*10)
//        {
//            //Follow white blob
//            puts("FOLLOWING WHITE");
//            follow(white);
//        }
//
//        //cvShowImage("Real",real);
//        //cvShowImage("Green",green);
//        //cvShowImage("Red",red);
//        //cvShowImage("White",white);
//        if(cvWaitKey(50)==27) 
//			{
//				comm.send_data('o');
//				break;
//		}
//    }
//
//    //Releasing images
//    cvReleaseImage(&real);
//    cvReleaseImage(&RED);
//    cvReleaseImage(&GREEN);
//    cvReleaseImage(&WHITE);
//    cvReleaseImage(&BLACK);
//    cvReleaseImage(&red);
//    cvReleaseImage(&green);
//    cvReleaseImage(&white);
//    cvReleaseImage(&graph);
//
//    //Destroying Windows
//    cvDestroyWindow("Real");
//    cvDestroyWindow("Green");
//    cvDestroyWindow("Red");
//    cvDestroyWindow("White");
//    cvDestroyWindow("RED_");
//	
//	cvDestroyWindow("C");
//	comm.stopDevice();
//	
//    return 0;
//}

//
//#include "stdafx.h"
//#include <stdio.h>
//#include <highgui.h>
//#include <cv.h>
//#include <cxcore.h>
// 
//int main()
//{
//  CvCapture* capture = cvCaptureFromCAM(-1);
//		if(!capture)
//	{
//		printf("camera not connected");
//		cvWaitKey(0);
//	}
//  IplImage *real;
//	cvNamedWindow("C", CV_WINDOW_AUTOSIZE);
//	do{
//    real=cvQueryFrame(capture);	
//	  cvShowImage("C", real);
//	  if(cvWaitKey(33)==27)break;
//	  }while(1);
//	  
//  cvReleaseImage(&real);
//  cvDestroyWindow("CV");
//	}

//
//#include "tserial.h"
//#include "bot_control.h"
//
//int main()
//{
//	serial comm;
//	comm.startDevice("\\\\.\\COM11", 9600);
//	char c;
//    while(1)
//    {
//		scanf("%c", &c);
//		switch(c)
//		{
//		case 'w':
//			comm.send_data('w');
//			break;
//		case 'a':
//			comm.send_data('a');
//			break;
//		case 's':
//			comm.send_data('s');
//			break;
//		case 'd':
//			comm.send_data('d');
//			break;
//		case 'o':
//			comm.send_data('o');
//			break;
//				
//		}
//		if(c=='o')break;
//	}
//	comm.stopDevice();
//	
//    return 0;
//}
/*
#include "stdafx.h"
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <math.h>
#include <tserial.h>
#include <bot_control.h>
#include <highgui.h>
#include <vector>
#include <iostream>
#include <cv.h>
#include <cxcore.h>
#define PIX(img,i,j,k) (((uchar *)img->imageData)[i*img->widthStep+j*img->nChannels+k])
#define PI 3.142


typedef struct{
	int x,y;
}element;
typedef struct{
	int p;
	int thita;
	int points;
	element endpoints[2];
}laneLine;

void imgToBinary(IplImage *src,IplImage *dest,int value){
	CvScalar pixel,pix;
	int i,j;
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			pix=cvGet2D(src,i,j);
			pixel.val[0]=(pix.val[0]+pix.val[1]+pix.val[2])/3;
			if(pixel.val[0]>value)
				pixel.val[0]=0;
			else
				pixel.val[0]=255;
			cvSet2D(dest,i,j,pixel);
		}
	}
}

void imgToGrey(IplImage *src,IplImage *dest){
	CvScalar pixel,pix;
	int i,j;
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			pix=cvGet2D(src,i,j);
			pixel.val[0]=pixel.val[1]=pixel.val[2]=(pix.val[0]+pix.val[1]+pix.val[2])/3;
			cvSet2D(dest,i,j,pixel);
		}
	}
}

void Leftflip(IplImage *img,IplImage *img2){
	int i,j;
	for(i=0;i<img->height;i++)//Left flip
	{
		for(j=0;j<img->width;j++)
		{
			PIX(img2,i,j,0)=PIX(img,i,j,0);
			PIX(img2,i,j,1)= PIX(img,i,j,1);
			PIX(img2,i,j,2)=PIX(img,i,j,2);
			//pixels=cvGet2D(img,i,j);
			//cvSet2D(img2,(img2->height-i-1),(img->width-j-1),pixels);
		}
	}
}

void imageDilation(IplImage *src,IplImage *dest){
	int r,s,i,j;
	CvScalar pixels;

	for(i=1;i<src->height-1;i++){
		for(j=1;j<src->width-1;j++){
			int white=0,black=0;
			for(r=i-1;r<=(i+1);r++){
				for(s=j-1;s<=(j+1);s++){
					pixels=cvGet2D(src,r,s);
					if(pixels.val[0]==255)
						white++;
					else
						black++;
				}
			}
			if(white>black){
				pixels.val[0]=255;
				cvSet2D(dest,i,j,pixels);
			}
			else{
				pixels.val[0]=0;
				cvSet2D(dest,i,j,pixels);
			}
		}
	}
}

void imageNoiseReduction(IplImage *src,IplImage *dest){
	int r,s,i,j;
	CvScalar pixels;

	for(i=1;i<src->height-1;i++){
		for(j=1;j<src->width-1;j++){
			int white=0,black=0;
			for(r=i-1;r<=(i+1);r++){
				for(s=j-1;s<=(j+1);s++){
					if(r==i && s==j)
						continue;
					pixels=cvGet2D(src,r,s);
					if(pixels.val[0]==255)
						white++;
					else
						black++;
				}
			}
			if(black>white){
				pixels.val[0]=0;
				cvSet2D(dest,i,j,pixels);
			}
		}
	}
}

void imageEdgeDetection(IplImage *src,IplImage *dest,int thresh){
	
	int r,s,i,j;
	int **edge;
	CvScalar pixels;

	edge=(int **)malloc(sizeof(int *)*src->height);
	for(i=0;i<src->height;i++)
		edge[i]=(int *)malloc(sizeof(int)*src->width);
	
	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++)
			edge[i][j]=0;
	}

	for(i=1;i<src->height-1;i++){
		for(j=1;j<src->width-1;j++){
			int max=0,min=255;
			for(r=i-1;r<=(i+1);r++){
				for(s=j-1;s<=(j+1);s++){
					pixels=cvGet2D(src,r,s);
					if(pixels.val[0]>max)
						max=pixels.val[0];
					if(pixels.val[0]<min)
						min=pixels.val[0];
					}
			}
			if((max-min)>=thresh){
				edge[i][j]=1;				
			}
		}
	}

	for(i=0;i<src->height;i++){
		for(j=0;j<src->width;j++){
			if(edge[i][j]==1){
				pixels.val[0]=255;
				cvSet2D(dest,i,j,pixels);
			}
			else{
				pixels.val[0]=0;
				cvSet2D(dest,i,j,pixels);
			}
		}
	}				

}

void createVideo(){

	CvCapture *capture=cvCaptureFromCAM(1);
	IplImage *sample;
	sample=cvQueryFrame(capture);
	while(!sample)
		sample=cvQueryFrame(capture);
	cvNamedWindow("Capture",0);
	CvVideoWriter *writer=cvCreateVideoWriter("newVideo.avi",CV_FOURCC('M','J','P','G'),30,cvGetSize(sample));
	
	while(1){
		sample=cvQueryFrame(capture);
		cvShowImage("Capture", sample);
		cvWriteFrame(writer,sample);
		if(cvWaitKey(33)==27)break;
		if(!sample){
			printf("No image");
			break;
		}
	}
	cvReleaseVideoWriter(&writer);	
}

void noiseEdgeVideo(IplImage *dest){
	
	CvCapture *capture=cvCreateFileCapture("newVideo.avi");
	IplImage *shape;
	cvNamedWindow("C",0);
	cvNamedWindow("New",0);
	int thresh=10;
	shape=cvQueryFrame(capture);
	IplImage *shapeGrey;
	dest=cvCreateImage(cvGetSize(shape),IPL_DEPTH_8U,1);
	cvCreateTrackbar("Threshold","C",&thresh, 255);

	while(1){
		shape=cvQueryFrame(capture);
		if(!shape){
			printf("No image");
			break;
		}
		shapeGrey=cvCreateImage(cvGetSize(shape),IPL_DEPTH_8U,1);
		cvCvtColor(shape,shapeGrey,CV_RGB2GRAY);
		cvShowImage("C",shapeGrey);
		
		imageEdgeDetection(shapeGrey,dest,thresh);
		imageDilation(dest,dest);

		imageNoiseReduction(dest,dest);
		cvShowImage("New", dest);
		if(cvWaitKey(10)==27)
			break;
		
	}

	cvReleaseImage(&shape);
	cvReleaseImage(&shapeGrey);	
	
}

void noiseEdgeImage(IplImage *shape,IplImage *dest){
	
	//int thresh=10;
	//cvNamedWindow("New",CV_WINDOW_AUTOSIZE);

	imageDilation(shape,dest);
	imageNoiseReduction(dest,dest);

	//cvShowImage("New", dest);

}

void calculateAvg(IplImage *img)
{
	int i,j;
	long long int rSum=0,gSum=0,bSum=0;
	
	for(i=0;i<img->height;i++){
		for(j=0;j<img->width;j++)
		{
			rSum+=PIX(img,i,j,0);
			gSum+=PIX(img,i,j,1);
			bSum+=PIX(img,i,j,2);
		}
	}
	long long int rAvg=rSum/(img->height*img->width),gAvg=gSum/(img->height*img->width),bAvg=bSum/(img->height*img->width);	
}

int createBlob(IplImage *dest,int **marked){
	
	int **visited;
	int count,i,j;
	int *row,*col;
	row=(int *)malloc(sizeof(int)*dest->height*dest->width);
	col=(int *)malloc(sizeof(int)*dest->height*dest->width);
	
	visited=(int **)malloc(sizeof(int *)*dest->height);
	for(i=0;i<dest->height;i++){
		visited[i]=(int *)malloc(sizeof(int)*dest->width);
	}	
	for(i=0;i<dest->height;i++){
		for(j=0;j<dest->width;j++){
			visited[i][j]=0;
		}
	}

	for(i=0;i<dest->height*dest->width;i++){
		row[i]=-1;
		col[i]=-1;
	}

	count=1;
	CvScalar pixels;
	long long int start,end;
	for(i=1;i<dest->height-1;i++)
	{
		for(j=1;j<dest->width-1;j++)
		{
			pixels=cvGet2D(dest,i,j);
			int t=(int)pixels.val[0];
			
			if(t==255 && visited[i][j]==0)
			{
				start=0;
				end=0;
				row[0]=i;
				col[0]=j;
				end=end+1;
				while(end>start && end<(dest->height*dest->width))
				{
					int r,c;
					r=row[start];
					c=col[start];
					row[start]=-1;
					col[start]=-1;
					start++;
					visited[r][c]=1;
					marked[r][c]=count;

					int p,q;
					for(p=r-1;p<=(r+1);p++){
						for(q=c-1;q<=(c+1);q++){
							int pix=(int )cvGet2D(dest,p,q).val[0];
							if(pix==255 && visited[p][q]==0){
								visited[p][q] = 1;
								row[end]=p;
								col[end]=q;
								end++;
							}
						}
					}
				}
				count++;
			}
		}
	}
	return count;
}

element findCentroid(IplImage *dest,int **marked,int value){
	int i,j;
	int xSum=0,ySum=0,total=0;
	element centroid;
	for(i=1;i<dest->height-1;i++){
		for(j=1;j<dest->width-1;j++){
			if(marked[i][j]==value){
				xSum+=j;
				ySum+=i;
				total++;
			}
		}
	}
	centroid.x=xSum/total;
	centroid.y=ySum/total;
	return centroid;
}

int largestBlob(IplImage *dest,int **marked,int count){
	int i,j;
	int *a=(int *)malloc(sizeof(int)*count);
	for(i=0;i<count;i++)
		a[i]=0;
	for(i=1;i<dest->height-1;i++){
		for(j=1;j<dest->width-1;j++){
			if(marked[i][j]==0)
				continue;
			else
				a[marked[i][j]]++;
		}
	}
	int max=0,pos=0;
	for(i=0;i<count;i++){
		if(a[i]>max){
			max=a[i];
			pos=i;
		}
	}
	return pos;
}

void drawPath(IplImage *blob,element centroid1,element centroid2){
	
	int i,j;
	int x1=centroid1.x,y1=blob->height-centroid1.y,x2=centroid2.x,y2=blob->height-centroid2.y;
	CvScalar pixels,pixy1,pixy2,pixy3,pix;
	pixy3.val[0]=pixy3.val[1]=pixy3.val[2]=0;
	pixy1=cvGet2D(blob,centroid1.y,centroid1.x);
	pixy2=cvGet2D(blob,centroid2.y,centroid2.x);

	pixels.val[0]=255;
	pixels.val[1]=0;
	pixels.val[2]=0;	
		
	if((x2-x1)==0){
		if(y2>y1){
			for(i=y1;i<=y2;i++){
				pix=cvGet2D(blob,blob->height-i,x1);
				if(!((pix.val[0]==pixy1.val[0] && pix.val[1]==pixy1.val[1] && pix.val[2]==pixy1.val[2])||(pix.val[0]==pixy2.val[0] && pix.val[1]==pixy2.val[1] && pix.val[2]==pixy2.val[2])||(pix.val[0]==pixy3.val[0] && pix.val[1]==pixy3.val[1] && pix.val[2]==pixy3.val[2]))){
					printf("No path exists");
					break;
				}
				cvSet2D(blob,blob->height-i,x1,pixels);
			}
		}
		else{
			for(i=y1;i>=y2;i--){
				pix=cvGet2D(blob,blob->height-i,x1);
				if(!((pix.val[0]==pixy1.val[0] && pix.val[1]==pixy1.val[1] && pix.val[2]==pixy1.val[2])||(pix.val[0]==pixy2.val[0] && pix.val[1]==pixy2.val[1] && pix.val[2]==pixy2.val[2])||(pix.val[0]==pixy3.val[0] && pix.val[1]==pixy3.val[1] && pix.val[2]==pixy3.val[2]))){
					printf("No path exists");
					break;
				}
				cvSet2D(blob,blob->height-i,x1,pixels);	
			}
		}
	}
	else{
		double m=(double)(y2-y1)/(double)(x2-x1);
		double c=y1-m*x1;
		if(x2>x1){
			for(i=x1;i<=x2;i++){
				int y=m*i+c;
				y=blob->height-y;
				pix=cvGet2D(blob,y,i);
				if(!((pix.val[0]==pixy1.val[0] && pix.val[1]==pixy1.val[1] && pix.val[2]==pixy1.val[2])||(pix.val[0]==pixy2.val[0] && pix.val[1]==pixy2.val[1] && pix.val[2]==pixy2.val[2])||(pix.val[0]==pixy3.val[0] && pix.val[1]==pixy3.val[1] && pix.val[2]==pixy3.val[2]))){
					printf("No path exists");
					break;
				}
				cvSet2D(blob,y,i,pixels);
			}
		}
		else{
			for(i=x1;i>=x2;i--){
				int y=m*i+c;
				y=blob->height-y;
				pix=cvGet2D(blob,y,i);
				if(!((pix.val[0]==pixy1.val[0] && pix.val[1]==pixy1.val[1] && pix.val[2]==pixy1.val[2])||(pix.val[0]==pixy2.val[0] && pix.val[1]==pixy2.val[1] && pix.val[2]==pixy2.val[2])||(pix.val[0]==pixy3.val[0] && pix.val[1]==pixy3.val[1] && pix.val[2]==pixy3.val[2]))){
					printf("No path exists");
					break;
				}
				cvSet2D(blob,y,i,pixels);
			}
		}
	}
}

int **markedArray(IplImage *dest){
	int **marked;
	int i,j;
	marked=(int **)malloc(sizeof(int *)*dest->height);
	for(i=0;i<dest->height;i++){
		marked[i]=(int *)malloc(sizeof(int)*dest->width);
	}	
	for(i=0;i<dest->height;i++){
		for(j=0;j<dest->width;j++){
			marked[i][j]=0;
		}
	}
	return marked;
}

int findDistance(int x1,int y1,int x2,int y2){
	int dist=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
	return (int)sqrt((double)dist);
}

void arrowCorner(IplImage *blob,int forRow[5],int forColumn[5],int **marked,int bigBlob){
	
	int i,j;
	CvScalar pixels;
	int flag=0;
	
	pixels.val[0]=255;
	pixels.val[1]=0;
	pixels.val[2]=0;

	for(i=1;i<blob->height-1;i++){
		for(j=1;j<blob->width-1;j++){
			if(marked[i][j]==bigBlob){
				forRow[0]=i;
				forColumn[0]=j;
				cvSet2D(blob,i,j,pixels);
				flag=1;
				break;
			}
		}
		if(flag==1)
			break;
	}
	
	flag=0;
	for(i=blob->height-1;i>=1;i--){
		for(j=1;j<blob->width-1;j++){
			if(marked[i][j]==bigBlob){
				forRow[1]=i;
				forColumn[1]=j;
				cvSet2D(blob,i,j,pixels);
				flag=1;
				break;
			}
		}
		if(flag==1)
			break;
	}

	flag=0;
	for(i=1;i<blob->width-1;i++){
		for(j=1;j<blob->height-1;j++){
			if(marked[j][i]==bigBlob){
				forRow[2]=j;
				forColumn[2]=i;
				cvSet2D(blob,j,i,pixels);
				flag=1;
				break;
			}
		}
		if(flag==1)
			break;
	}

	flag=0;
	for(i=blob->width;i>=1;i--){
		for(j=1;j<blob->height-1;j++){
			if(marked[j][i]==bigBlob){
				forRow[3]=j;
				forColumn[3]=i;
				cvSet2D(blob,j,i,pixels);
				flag=1;
				break;
			}
		}
		if(flag==1)
			break;
	}
}

int commonCorner(int pos1,int pos2){
	int a1,a2,a3,a4;
	if(pos1<3){
		a1=0;
		a2=pos1+1;
	}
	else if(pos1<5){
		a1=1;
		a2=pos1-1;
	}
	else{
		a1=2;
		a2=3;
	}
	if(pos2<3){
		a3=0;
		a4=pos2+1;
	}
	else if(pos2<5){
		a3=1;
		a4=pos2-1;
	}
	else{
		a3=2;
		a4=3;
	}
	if(a1==a3||a1==a4)
		return a1;
	if(a2==a4||a2==a3)
		return a2;
	return -1;
}

element findTip(IplImage *blob,int **marked,int bigBlob){
	int i,j;
	int forRow[5],forColumn[5];
	arrowCorner(blob,forRow,forColumn,marked,bigBlob);

	int dist[6];
	int index=0;
	for(i=0;i<4;i++){
		for(j=i+1;j<4;j++){
			dist[index++]=findDistance(forRow[i],forColumn[i],forRow[j],forColumn[j]);		
		}
	}

	int high1=0,high2=0,high3=0,high4=0,pos1,pos2,pos3,pos4,range=10,maxvalue=0;
	
	for(i=0;i<6;i++){
		if(dist[i]>maxvalue){
			maxvalue=dist[i];
			pos1=i;
		}
	}
	high1=maxvalue;
	for(i=0;i<6;i++){
		if(dist[i]>high2 && dist[i]<high1){
			high2=dist[i];
			pos2=i;
		}
	}
	for(i=0;i<6;i++){
		if(dist[i]>high3 && dist[i]<high2){
			high3=dist[i];
			pos3=i;
		}
	}
	for(i=0;i<6;i++){
		if(dist[i]>high4 && dist[i]<high3){
			high4=dist[i];
			pos4=i;
		}
	}

	for(i=0;i<6;i++)
		printf("%d \n",dist[i]);
		
	int tip;
	printf("%d %d %d %d\n",high1,high2,high3,high4);
	int type=0;
	if((high1-high2)<range){
		if((high3-high4),range){
			//tip missing
			int a=0,apos;
			for(i=0;i<6;i++){
				if(!((dist[i]==high1)||(dist[i]==high2)||(dist[i]==high3)||(dist[i]==high4))){
					if(dist[i]>a){
						a=dist[i];
						apos=i;
					}
				}
			}

			int a1,a2;
			if(apos<3){
				a1=0;
				a2=apos+1;
			}
			else if(apos<5){
				a1=1;
				a2=apos-1;
			}
			else{	
				a1=2;
				a2=3;
			}
			forRow[4]=(forRow[a1]+forRow[a2])/2;
			forColumn[4]=(forColumn[a1]+forColumn[a2])/2;
			tip=4;
		}
		else{
			//triangle point missing
			tip=commonCorner(high1,high2);
		}
	}
	else{
		//bottom missing
		range=5;
		int same1=0,same2=0,posi1=0,posi2=0;
		for(i=0;i<6;i++){
			for(j=i+1;j<6;j++){
				if(dist[i]>dist[j]){
					if((dist[i]-dist[j])<range){
						
						same1=dist[i];
						same2=dist[j];
						posi1=i;
						posi2=j;
					}
				}
				else{
					if((dist[j]-dist[i])<range){
						same1=dist[i];
						same2=dist[j];
						posi1=i;
						posi2=j;
					}
				}
			}
		}
		tip=commonCorner(posi1,posi2);
	}
	printf("tip:%d\n",tip);
	element head;
	if(tip==-1){
		head.x=-1;
		head.y=-1;
	}
	else{
		head.x=forColumn[tip];
		head.y=forRow[tip];
	}
	return head;
}

void drawLine(IplImage *test,int pMax,int thetaMax,laneLine *ll,int index){
	CvScalar pixels;
	pixels.val[0]=255;
	pixels.val[1]=0;
	pixels.val[2]=0;
	
	int cx=test->width/2;
	int cy=test->height/2;
	double angle=2*PI*thetaMax/400;
	int x1=0,y1=0;
	x1=pMax*cos(angle);
	y1=pMax*sin(angle);
	//cvSet2D(test,y1,x1,pixels);
	double slope,c;

	if(angle<0.01||(angle>3.1 && angle<3.2)){	
		int y2=y1;
		while((test->height-cy-y1)<test->height && (test->height-cy-y1)>=0){
			if((cx+x1)>=0 && (cx+x1)<test->width)
				cvSet2D(test,test->height-cy-y1,cx+x1,pixels);
			y1++;
			if((test->height-cy-y1)==(test->height-1)){
				ll[index].endpoints[0].x=(cx+x1);
				ll[index].endpoints[0].y=test->height-1;
			}
			if((test->height-cy-y1)==0){
				ll[index].endpoints[0].x=(cx+x1);
				ll[index].endpoints[0].y=0;
			}
		}
		while((test->height-cy-y2)>=0 && (test->height-cy-y2)<test->height){
			if((cx+x1)>=0 && (cx+x1)<test->width)
				cvSet2D(test,test->height-cy-y2,(cx+x1),pixels);
			y2--;
			if((test->height-cy-y2)==(test->height-1)){
				ll[index].endpoints[1].x=(cx+x1);
				ll[index].endpoints[1].y=test->height-1;
			}
			if((test->height-cy-y2)==0){
				ll[index].endpoints[1].x=(cx+x1);
				ll[index].endpoints[1].y=0;
			}
		}
	}
	else{
		int x2=x1,y2=y1;
		//printf("else: %d %d %d %d",x1,y1,x2,y2);
		slope=-1/tan(angle);
		c=y1-slope*x1;
		//printf("hieght: %d widht: %d cx; %d cy:%d",test->height,test->width,cx,cy);
		//printf("c:%lf slope:%lf",c,slope);
		while((test->height-cy-y1)<(test->height-1) && ((cx+x1)>=0) && ((test->height-cy-y1)>=0) && ((cx+x1)<(test->width-1))){
			cvSet2D(test,(test->height-cy-y1),(cx+x1),pixels);
			x1++;
			y1=slope*x1+c;
			if((cx+x1)==0){
				ll[index].endpoints[0].x=0;
				ll[index].endpoints[0].y=(test->height-cy-y1);
			}
			if((test->height-cy-y1)==(test->height-1)){
				ll[index].endpoints[0].x=(cx+x1);
				ll[index].endpoints[0].y=test->height-1;
			}
			if((test->height-cy-y1)==0){
				ll[index].endpoints[0].x=(cx+x1);
				ll[index].endpoints[0].y=0;
			}
			if((cx+x1)==test->width-1){
				ll[index].endpoints[0].x=test->width-1;
				ll[index].endpoints[0].y=(test->height-cy-y1);
			}
		}
		
		while((test->height-cy-y2)<(test->height-1) && ((cx+x2)>=0) && ((test->height-cy-y2)>=0) && ((cx+x2)<(test->width-1))){
			cvSet2D(test,(test->height-cy-y2),(cx+x2),pixels);
			x2--;
			y2=slope*x2+c;
			if((cx+x2)==0){
				ll[index].endpoints[1].x=0;
				ll[index].endpoints[1].y=(test->height-cy-y2);
			}
			if((test->height-cy-y2)==(test->height-1)){
				ll[index].endpoints[1].x=(cx+x2);
				ll[index].endpoints[1].y=test->height-1;
			}
			if((test->height-cy-y2)==0){
				ll[index].endpoints[1].x=(cx+x2);
				ll[index].endpoints[1].y=0;
			}
			if((cx+x2)==test->width-1){
				ll[index].endpoints[1].x=test->width-1;
				ll[index].endpoints[1].y=(test->height-cy-y2);
			}
		}
	}
}

int lineDetection(IplImage *blob,IplImage *test,laneLine *ll,int thresh){
	
	int **pthita,i,j;
	CvScalar pixels;
	int p,thita,dia=findDistance(0,0,blob->width,blob->height);
	pthita=(int **)malloc(sizeof(int *)*dia/2);
	for(i=0;i<dia/2;i++)
		pthita[i]=(int *)malloc(sizeof(int)*400);
	
	for(i=0;i<dia/2;i++){
		for(j=0;j<400;j++)
			pthita[i][j]=0;
	}

	int height=blob->height,width=blob->width;
	int cx=width/2,cy=height/2;
	double angle;
	for(i=0;i<blob->height;i++){
		for(j=0;j<blob->width;j++){
			pixels=cvGet2D(blob,i,j);
			if(pixels.val[0]==255){
				//cvSet2D(test,i,j,pixels);
				for(thita=0;thita<400;thita++){
					angle=2*thita*PI/400;
					int p=((j-cx)*cos(angle)+(cy-i)*sin(angle));
					if(p>=0 && p<dia/2)
						pthita[p][thita]++;
				}
			}
		}
	}
	int a=0;
	int index=0;
	for(i=0;i<dia/2;i++){
		for(j=0;j<400;j++){
			if(pthita[i][j]>=thresh){
				int max=pthita[i][j];
				if(i==0)
					max/=2;
				int lx,ly;
				int flag=1;
				for(ly=i-8;ly<=(i+8);ly++){
					for(lx=j-8;lx<=(j+8);lx++){
						if((ly>=0 && ly<dia/2) && (lx>=0 && lx<400)){
							if(pthita[ly][lx]>max){
								flag=0;
								break;
							}
						}
					}
					if(flag==0)
						break;
				}
				if(flag==1){
					//printf(" max: %d pMax: %d thetaMax: %d\n",pthita[i][j],i,j);
					//if((thita>5 && thita<181)||(thita>185 && thita<395)){
						drawLine(test,i,j,ll,index);
						ll[index].p=i;
						ll[index].thita=j;
						ll[index].points=pthita[i][j];			
						index++;
					//}
				}
			}
		}
	}
	return index;
}

void printLanes(IplImage *lane,laneLine *ll,int count){
	int i,j;
	int max1=0,max2=0,pos1=0,pos2=0;
	for(i=0;i<count;i++){
		if(ll[i].points>max1){
			max1=ll[i].points;
			pos1=i;
		}
	}
	for(i=0;i<count;i++){
		if(ll[i].points>max2 && ll[i].points<max1){
			pos2=i;
			max2=ll[i].points;
		}
	}

	CvScalar pixels;
	pixels.val[0]=255;
	pixels.val[1]=0;
	pixels.val[2]=0;
	drawLine(lane,ll[pos1].p,ll[pos1].thita,ll,pos1);
	drawLine(lane,ll[pos2].p,ll[pos2].thita,ll,pos2);
	laneLine temp;
	if(count>=1){
		temp=ll[pos1];	
		ll[pos1]=ll[0];
		ll[0]=temp;
	}
	if(count>=2){
		temp=ll[pos2];
		ll[pos2]=ll[1];
		ll[1]=temp;
	}
}

void cvCannyinVideo(){

	CvCapture *capture=cvCaptureFromCAM(-1);
	cvNamedWindow("tester",CV_WINDOW_AUTOSIZE);
	IplImage *sample=cvQueryFrame(capture);
	IplImage *tester=cvCreateImage(cvGetSize(sample),IPL_DEPTH_8U,1);
	IplImage *tester1=cvCreateImage(cvGetSize(sample),IPL_DEPTH_8U,1);
	while(1){
		sample=cvQueryFrame(capture);
		if(!sample){
			printf("no image");
			break;
		}
		cvCvtColor(sample,tester,CV_RGB2GRAY);
		//cvSobel(sample,tester,1,1,3);
		cvCanny(tester,tester1,50,70,3);
		//imageEdgeDetection(tester,tester1,20);
		cvShowImage("tester",tester1);
		if(cvWaitKey(100)==27)
			break;
	}
	
}

void centroidSlopesOfPath(laneLine *ll,element centroid,double slopes[2]){
	/*printf("%d\n",forEdge->height);
	printf("%d %d, %d %d\n",ll[0].endpoints[0].x,ll[0].endpoints[0].y,ll[0].endpoints[1].x,ll[0].endpoints[1].y);
	printf("%d %d, %d %d\n",ll[1].endpoints[0].x,ll[1].endpoints[0].y,ll[1].endpoints[1].x,ll[1].endpoints[1].y);
	printf("%d %d\n",ll[0].thita,ll[1].thita);
	
	int i,j;
	element centroidOflines[2];
	//origin is top-left, x and y are real inverse of i and j
	centroidOflines[0].x=(ll[0].endpoints[0].x+ll[0].endpoints[1].x)/2;
	centroidOflines[0].y=(ll[0].endpoints[0].y+ll[0].endpoints[1].y)/2;
	centroidOflines[1].x=(ll[1].endpoints[0].x+ll[1].endpoints[1].x)/2;
	centroidOflines[1].y=(ll[1].endpoints[0].y+ll[1].endpoints[1].y)/2;

	centroid.x=(centroidOflines[0].x+centroidOflines[1].x)/2;
	centroid.y=(centroidOflines[0].y+centroidOflines[1].y)/2;
	
	for(i=0;i<2;i++){
		if(ll[i].thita>=180)
			ll[i].thita-=180;
		double angle=2*ll[i].thita*PI/400;
		if(angle<0.01){
			slopes[i]=90;
		}
		else{
			slopes[i]=-1/tan(angle);
			slopes[i]=tanh(slopes[i])*180/PI;
		}
	}
	//printf("%lf %lf\n",slopes[0],slopes[1]);
	
}

int arduino(char c){
    
	switch(c)
	{
	case 'w':
		comm.send_data('w');
		break;
	case 'a':
		comm.send_data('a');
		break;
	case 's':
		comm.send_data('s');
		break;
	case 'd':
		comm.send_data('d');
		break;
	case 'o':
		comm.send_data('o');
		break;		
	}
	if(c=='o')
		comm.stopDevice();
	
    return 0;
}

void laneDetectionCode(){

	comm.startDevice("\\\\.\\COM11", 9600);
	laneLine *ll;
	ll=(laneLine *)malloc(sizeof(laneLine)*10000);
	int thresh=70;
	int i,j;

	CvCapture *capture=cvCaptureFromCAM(1);
	IplImage *sample=cvQueryFrame(capture);
	while(!sample){
		sample=cvQueryFrame(capture);
	}
	IplImage *forEdge=cvCreateImage(cvGetSize(sample),IPL_DEPTH_8U,1);
	cvCanny(sample,forEdge,50,100,3);
	IplImage *detectedLines=cvCreateImage(cvGetSize(forEdge),IPL_DEPTH_8U,1);
	IplImage *lane=cvCreateImage(cvGetSize(forEdge),IPL_DEPTH_8U,1);
	int minthresh=100,maxthresh=200;
	
	cvNamedWindow("tester",CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("min threshold","tester",&minthresh,400);
	cvCreateTrackbar("max threshold","tester",&maxthresh,500);
	cvCreateTrackbar("No of pixels","tester",&thresh,300);
	//cvShowImage("tester",forEdge);
	
	while(1){
		sample=cvQueryFrame(capture);
		cvCanny(sample,forEdge,minthresh,maxthresh,3);

		if(!forEdge){
			printf("no image");
			break;
		}
		
		CvScalar p;
		p.val[0]=0;
		for(i=0;i<forEdge->height;i++){
			for(j=0;j<forEdge->width;j++){
				cvSet2D(lane,i,j,p);
				cvSet2D(detectedLines,i,j,p);
			}
		}

		int linesCount=0;
		linesCount=lineDetection(forEdge,detectedLines,ll,thresh);
		printLanes(lane,ll,linesCount);
	
		double slopes[2];
		element finalCentroid;
		finalCentroid.x=0;
		finalCentroid.y=0;
		
		centroidSlopesOfPath(ll,finalCentroid,slopes);

		
		cvShowImage("tester",forEdge);
		cvNamedWindow("detectedLines",CV_WINDOW_AUTOSIZE);
		cvShowImage("detectedLines",detectedLines);
		cvNamedWindow("lane_printing",CV_WINDOW_AUTOSIZE);
		cvShowImage("lane_printing",lane);

		if((slopes[0]+slopes[1])>35){
			printf("%lf %lf sum:%lf\n",slopes[0],slopes[1],slopes[0]+slopes[1]);
			printf("move right\n");
			char c='d';
			arduino(c);
		}
		else if(slopes[0]+slopes[1]<-35){
			printf("%lf %lf sum:%lf\n",slopes[0],slopes[1],slopes[0]+slopes[1]);
			printf("move left\n");
			char c='a';
			arduino(c);
		}
		else{
			printf("%lf %lf sum:%lf\n",slopes[0],slopes[1],slopes[0]+slopes[1]);
			printf("move straight\n");
			char c='w';
			arduino(c);
		}
		
		if(cvWaitKey(100)==27){
			arduino('o');
			break;
		}
	
	}
}

int main()
{
	srand((unsigned int)time(NULL));
	int i,j,k,s,thresh;
	CvScalar pixels,pixels2;
	//IplImage *img=cvLoadImage("shape_detection.jpg");
	//IplImage *dest=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	//imgToBinary(img,dest,220);
	//noiseEdgeImage(dest,dest);
	//int **marked=markedArray(dest);
	
	/*Blob detection
	int count;
	count=createBlob(dest,marked);
	IplImage *blob=cvCreateImage(cvGetSize(dest),IPL_DEPTH_8U,1);
	for(i=1;i<img->height-1;i++){
		for(j=1;j<img->width-1;j++){
			if(marked[i][j])
				pixels.val[0]=255;
			else
				pixels.val[0]=0;
			cvSet2D(blob,i,j,pixels);	
		}
	}
	printf("%d\n",count);	
	cvNamedWindow("Blob",CV_WINDOW_AUTOSIZE);
	cvShowImage("Blob",blob);
	*/

	/*Hough Circle
	CvMemStorage *mem=cvCreateMemStorage(0);
	IplImage *edges=cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
	cvCvtColor(img,edges,CV_RGB2GRAY);
	cvSmooth(edges,edges, CV_GAUSSIAN, 5, 5 );
	CvSeq *results = cvHoughCircles(edges,mem,CV_HOUGH_GRADIENT,2,edges->width/10,100,100,0,0);
	IplImage *newimage=cvCreateImage(cvGetSize(edges),IPL_DEPTH_8U,3);
	pixels.val[0]=0;
	pixels.val[1]=255;
	pixels.val[2]=0;
	for(i=0; i<results->total; i++ )
	{
		printf("here");
		float* p = (float*) cvGetSeqElem( results, i );
		CvPoint pt = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );
		cvCircle(newimage,pt,cvRound( p[2] ),pixels);
	}
	cvNamedWindow("edge",CV_WINDOW_AUTOSIZE);
	cvShowImage("edge",newimage);
	

	//Line detection and lane identification
	laneDetectionCode();
	

	Arrow direction
	int bigBlob=largestBlob(dest,marked,count);
	element centroid=findCentroid(dest,marked,bigBlob);
	element tip=findTip(blob,marked,bigBlob);
	double slope=(double)(centroid.y-tip.y)/(double)(tip.x-centroid.x);
	printf("\n%lf\n",slope);
	*/
	
	
	/*Create path between centroids
	int value1,value2;
	printf("Enter blob 1:\n");
	scanf("%d",&value1);
	printf("Enter blob 2:\n");
	scanf("%d",&value2);
	element centroid1,centroid2;
	centroid1=findCentroid(dest,marked,value1);
	centroid2=findCentroid(dest,marked,value2);
	drawPath(blob,centroid1,centroid2);
	

	cvWaitKey(0);
	return 0;
}
*/