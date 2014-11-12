#include "stdafx.h"
/*//#include <iostream>
//#include <highgui.h>
//#include <cv.h>
//#include <cxcore.h>
//#include <vector>
//
//#define CAM_INDEX -1
//#define BOTTOM 1/3
//#define TURNORLANE 1.5
//#define GFORARROW 190
//#define RFORSTOP 190
//#define BDONTCARE 120
//#define DATA(image,i,j,k) *((uchar *)&image->imageData[(i)*(image->widthStep) + (j)*(image->nChannels) + (k)])
//#define FOR(i,a) for(size_t i=0;i<(size_t)(a);i++)
//
//using namespace std;
//using namespace cv;
//
//void checkLaneOrTurn(IplImage*, IplImage*);
//IplImage* getbottom(IplImage*);
//void Lane(IplImage* , IplImage* );
//void TurnOrStop(IplImage*, IplImage*);
//int GetArrowDirection(IplImage* bin);
//void isTurn(IplImage* bin, IplImage* frame);
//void isStop();
//double GetRatio(IplImage*);
//
//int main()
//{
//	cvNamedWindow("Cam", CV_WINDOW_AUTOSIZE);
//	cvNamedWindow("Cam_Smooth", CV_WINDOW_AUTOSIZE);
//	//cvcapture* capture=cvcapturefromcam(cam_index);
//	//if(!capture)
//	//{
//	//	cout<<"camera not connected";
//	//	cvwaitkey(0);
//	//}
//	//iplimage* frame=cvqueryframe(capture);
//
//	IplImage* frame=cvLoadImage("lane.png");
//	IplImage* cam_smooth=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 3);
//	IplImage* cam_gray=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
//	IplImage* cam_bin=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
//	
//	int bin_threshold=0;
//	int erode=0;
//	cvCreateTrackbar("Erode", "Cam", &erode, 20, NULL);
//	cvCreateTrackbar("Binary Threshold", "Cam", &bin_threshold, 255, NULL);
//	while(1)
//	{
//	//frame=cvQueryFrame(capture);
//	//if(!frame) break;
//	cvShowImage("Cam", frame);
//	cvCvtColor(frame, cam_gray, CV_RGB2GRAY);
//	cvThreshold(cam_gray, cam_bin, bin_threshold, 255, CV_THRESH_BINARY);
//	cvErode(cam_bin, cam_bin, NULL, erode);
//	cvShowImage("Cam_Smooth", cam_bin);
//	char c=cvWaitKey(33);
//	if(c==27) break;
//	}
//	//cvReleaseCapture(&capture);
//	cvReleaseImage(&frame);
//	cvReleaseImage(&cam_smooth);
//	cvDestroyWindow("Cam");
//	cvDestroyWindow("Cam_Smooth");
//
//}
//
//void checkLaneOrTurn(IplImage* bin, IplImage* frame)
//{
//	IplImage* bottom=getbottom(bin);
//	double ratio=GetRatio(bottom);
//	if(ratio>TURNORLANE)
//		TurnOrStop(bin, frame);
//	else
//		Lane(bin, frame);
//}
//
//IplImage* getbottom(IplImage* bin)
//{
//	IplImage* bottom;
//	cvSetImageROI(bin, cvRect(0, (bin->height)*BOTTOM, bin->width, (bin->height)*(1-BOTTOM)));
//	IplImage* tmp = cvCreateImage(cvGetSize(bin),bin->depth,bin->nChannels);
//	cvCopy(bin, tmp, NULL);
//	cvResetImageROI(bin);
//	bottom = cvCloneImage(tmp);	
//	return bottom;
//}
//
//void Lane(IplImage* bin, IplImage* frame)
//{
//	vector<Vec4i> lines;
//	HoughLinesP(bin, lines, 1, CV_PI/180, 50, 50, 10 );
//	for( size_t i = 0; i < lines.size(); i++ )
//	{
//		Vec4i l = lines[i];
//		cout<<l[0]<<" "<<l[1]<<" "<<l[2]<<" "<<l[3]<<"\n";
//	}
//	//Addslopes to get 0
//	//Make Bot move as per equirement
//}
//
//void TurnOrStop(IplImage* bin, IplImage* frame)
//{
//
//	int greencnt=0, redcnt=0, i, j;
//
//	FOR(i, frame->height)
//	{
//		FOR(j, frame->width)
//		{
//			if((DATA(frame, i, j, 1)>GFORARROW)&&(DATA(frame, i, j, 1)<RFORSTOP*0.5)&&(DATA(frame, i, j, 0)<BDONTCARE))
//				greencnt++;
//			else if((DATA(frame, i, j, 1)<GFORARROW*0.5)&&(DATA(frame, i, j, 1)>RFORSTOP)&&(DATA(frame, i, j, 0)<BDONTCARE))
//				redcnt++;
//		}
//	}
//
//	if(greencnt>redcnt*10)isTurn(bin, frame);
//	else if(redcnt>greencnt*10)isStop();
//
//}
//
//void isTurn(IplImage* bin, IplImage* frame)
//{
//	//GetArrow Direction 0 left 1 right
//	//TurnBot
//	//MakeSureitgetsbackonlane
//	//return
//}
//
//int GetArrowDirection(IplImage* frame)
//{
//	IplImage* arrow=cvCreateImage(cvGetSize(frame), IPL_DEPTH_8U, 1);
//	FOR(i, frame->height)
//	{
//		FOR(j, frame->width)
//		{
//			if((DATA(frame, i, j, 1)>GFORARROW)&&(DATA(frame, i, j, 1)<RFORSTOP*0.5)&&(DATA(frame, i, j, 0)<BDONTCARE))\
//			{
//				DATA(arrow, i, j, 0)=255;
//			}
//			else
//			{
//				DATA(arrow, i, j, 0)=0;
//			}
//		}
//	}
//
//	//Get Hough Line Direction
//}
//
//void isStop()
//{
//	//motor_stop();
//}
//
//double GetRatio(IplImage* bin)
//{
//	double ratio;
//
//	int i, j, black=0, white=0;
//
//	FOR(i, bin->height)
//	{
//		FOR(j, bin->width)
//		{
//			if(DATA(bin, i, j, 0)==0)
//				black++;
//			else
//				white++;
//		}
//	}
//	ratio=black/white;
//	return ratio;
//}




#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdio>
#include <algorithm>
#include "cv.hpp"
#include "stdafx.h"
#include <highgui.h>
#include <cv.h>
#include <cxcore.h>
#include "tserial.h"
#include "bot_control.h"

#define MIN_THRESH 500       //For Blob
#define LIM 5  //For following
#define DEC_THRESH 20     //For Decision
#define SYM_THRESH 0.7      //For Symmetry
#define PI 3.14159265359
#define IMGDATA(image,i,j,k) *((uchar *)&image->imageData[(i)*(image->widthStep) + (j)*(image->nChannels) + (k)])

using namespace cv;
using namespace std;
int shape=0;
int top=0;
int **a;
int **stack;
serial comm;


void push(int a, int b)
{
top=top+1;
stack[top][0]=a;
stack[top][1]=b;
}

void pop()
{
top=top-1;
}

void dfs(IplImage* I,int m, int n)
{
shape=shape+1;
push(m,n);
a[m][n]=0;
int chk=0;
int i, j;
int x, y;
while(top>0)
{
    x=stack[top][0];
    y=stack[top][1];
    if(IMGDATA(I,x,y,0)==255)
       {
		a[x][y]=shape;
		chk=0;
	}
    for(i=x-1;i<=x+1;i++)
	{
		for(j=y-1;j<=y+1;j++)
		 {
			 if((j>=(I->width))||(i>=I->height)||(j<0)||(i<0))
				 continue;
			if((IMGDATA(I,i,j,0)==255)&&(a[i][j]==-1))
			{
					push(i,j);
					chk=1;
					break;
			}
		}
        if (chk==1)
            break;
	}
    if(chk==0)
       {
		   pop();
        continue;
		}
    if(chk==1)
		continue;
}
}

void getblobs(IplImage* I)
{
for(int i=0;i<I->height;i++)
{
	for(int j=0;j<I->width;j++)
	{
		a[i][j]=-1;
	}
}

for(int i=0;i<(I->height)*(I->width);i++)
{
	stack[i][0]=0;
	stack[i][1]=0;
}


int ht=I->height;
int wd=I->width;
int r, l;

for(r=1;r<ht-1;r++)
{
	for(l=1;l<wd-1;l++)
	{
        if( (IMGDATA(I,r,l,0)==255)&&(a[r][l]==-1))
		{
            dfs(I,r,l);
		}
	}
}
}
//This function counts the number of white pixels
int count(IplImage *img)
{
    int MAXROW=img->height,MAXCOL=img->width,count=0;
    for(int i=0;i<MAXROW;i++)
    {
        for(int j=0;j<MAXCOL;j++)
        {
            if(IMGDATA(img,i,j,0)==255)count++;
        }
    }
    return count;
}

//Creates Blank Copy
IplImage *CreateBlankCopyOf(IplImage *img,int n=0)
{
    int ht,wd;
    ht=img->height;
    wd=img->width;
    if(n==0)
    {
        n=img->nChannels;
    }
    return cvCreateImage(cvSize(wd,ht),8,n);
}

//Checks if the image has crossed the boundary of screen
int check_boundary(IplImage *img)
{
    int check=0;
    int MAXROW=img->height,MAXCOL=img->width;
    for(int j=0;j<MAXCOL;j++)
    {
        if(IMGDATA(img,0,j,0)==255 || IMGDATA(img,MAXROW-1,j,0)==255) return 2;
    }
    int flag1=0,flag2=0;
    for(int i=0;i<MAXROW;i++)
    {
        if(IMGDATA(img,i,0,0)==255) flag1=1;
        if(IMGDATA(img,i,MAXCOL-1,0)==255) flag2=1;
    }
    if(flag1==1 && flag2==1) return 2;
    else if(flag1==1) return -1;
    else if(flag2==1) return 1;
    return 0;
}

//Calculates column no. of the mid point of image
int midcol(IplImage *img)
{
	int mid=0,count=0,MAXROW=img->height,MAXCOL=img->width;
	for(int i=0;i<MAXROW;i++)
	{
		for(int j=0;j<MAXCOL;j++)
		{
			if(IMGDATA(img,i,j,0)==255)
			{
				count++;
				mid+=j;
			}
		}
	}
	if(count<50) return -1;
	mid/=count;
	return mid;

}

//Follows the image(green,red)
void checkdir(IplImage *green,IplImage *red)
{
	int col=(green->width)/2;
	int mid_green=midcol(green);
	int mid_red=midcol(red);
	if (mid_green>0)
	{
		if(mid_green>col-50 && mid_green<col+50) puts("Straight");
		else if(mid_green>col+50) puts("Right");
		else puts("Left");

	}
	else if (mid_red>0)
	{
		if(mid_red>col-50 && mid_red<col+50) puts("Straight");
		else if(mid_red>col+10) puts("Right");
		else puts("Left");
	}
	else puts("Straight");
}

//Separate Red,Green,White & Black
void  colors(IplImage *img,IplImage *red ,IplImage *green,IplImage *white,IplImage *black)
{

	int i, j, lall=0;

	for(i=0;i<img->height;i++)
     {
		 for(j=0;j<img->width;j++)
		{
         if((IMGDATA(img,i,j,0)<100)&&(IMGDATA(img,i,j,1)<100)&&(IMGDATA(img,i,j,2)<100))
         {
             IMGDATA(red,i,j,0)=0;
             IMGDATA(green,i,j,0)=0;
             IMGDATA(white,i,j,0)=0;
             IMGDATA(black,i,j,0)=255;
         }
         else if((IMGDATA(img,i,j,0)>100)&&(IMGDATA(img,i,j,1)>100)&&(IMGDATA(img,i,j,2)>100))
         {
             IMGDATA(red,i,j,0)=0;
             IMGDATA(green,i,j,0)=0;
             IMGDATA(white,i,j,0)=255;
             IMGDATA(black,i,j,0)=0;
         }
         else if((IMGDATA(img,i,j,1))-(IMGDATA(img,i,j,2))>50)
         {
             IMGDATA(red,i,j,0)=0;
             IMGDATA(green,i,j,0)=255;
             IMGDATA(white,i,j,0)=0;
             IMGDATA(black,i,j,0)=0;
         }
          else if((IMGDATA(img,i,j,1))-(IMGDATA(img,i,j,2))<-80)
         {
             IMGDATA(red,i,j,0)=255;
             IMGDATA(green,i,j,0)=0;
             IMGDATA(white,i,j,0)=0;
             IMGDATA(black,i,j,0)=0;
         }
         else
         {
             IMGDATA(red,i,j,0)=0;
             IMGDATA(green,i,j,0)=0;
             IMGDATA(white,i,j,0)=0;
             IMGDATA(black,i,j,0)=0;
         }

     }
	}
}


//For taking vertical projection of the image
void to_graph_hor(IplImage *binary,IplImage *graph)
{
    for(int i=0;i<(binary->width);i++ )
    {
        int counter =binary->height-1;
        for(int j=0;j<(binary->height);j++)
     {
        if(IMGDATA(binary,j,i,0))
        {
            IMGDATA(graph,counter--,i,0)=255;
        }

     }
    }
}

//Finds the direction of the arrow
int direction(IplImage *img)
{
    int i,j;
    int hor_pix=0,vert_pix=0,count_right=0,count_left=0;
    int flag=0;
    for(i=0;i<img->height;i++)
    {
       for(j=0;j<img->width;j++)
        {
            if(IMGDATA(img,i,j,0)==255)
            {
                //printf("hor %d  ver %d\n",hor_pix,vert_pix);
                hor_pix=j;
                vert_pix=i;
                flag=1;
                break;
            }
        }
    if(flag) break;
    }

    for(j=hor_pix;j<=hor_pix+20;j++)
        for(i=0;i<img->height;i++)
            if(IMGDATA(img,i,j,0)==255)
                count_right++;

    for(j=hor_pix;j>=hor_pix-20;j--)
        for(i=0;i<img->height;i++)
            if(IMGDATA(img,i,j,0)==255)
                count_left++;
    //printf("count left=%d\ncount right=%d\n",count_left,count_right);

    if(count_right>count_left)
        return 1;
    else if(count_right<count_left)
        return -1;
    else return 0;

}

//Gives the biggest blob from an image
void biggest_blobs(IplImage *img,IplImage *blob )//,IplImage *blob2);
{
	shape=0;
	getblobs(img);
    int max_cnt=0;
    int big_blob=-1;
    int i, j, k;
	long int cnt=0;
	cnt=count(img);
	for(k=1;k<=shape;k++)
	{
		for (i=0;i<img->height;i++)
        {
			for(j=0;j<img->width;j++)
			{
				if(a[i][j]==k)
					{
						cnt++;			
					}
			}
        }
		if(cnt>max_cnt)
		{
			max_cnt=cnt;
			big_blob=k;
		}
		cnt=0;
	}

	if(big_blob==-1)
	{
	for(int i=0;i<img->height;i++)
        {
			for(int j=0;j<img->width;j++)
			{
               IMGDATA(blob,i,j,0)=0;
			}
		}
	return;
	}

    for(int i=0;i<img->height;i++)
        for(int j=0;j<img->width;j++)
        {
            if(a[i][j]==big_blob)
            {
                IMGDATA(blob,i,j,0)=255;
            }
            else
            {
                IMGDATA(blob,i,j,0)=0;
            }
        }
	
}

int centroid(IplImage* img)
{
	int white_count=1, white_wt=1, i, j;
	for(i=0;i<img->height;i++)
	{
		for(j=0;j<img->width;j++)
		{
			if(IMGDATA(img, i, j, 0)==255)
			{
				white_count++;
				white_wt+=j;	
			}
		}
	}
	printf("%d w", white_wt);
	return (int)(white_wt/white_count);

}

void follow(IplImage *img)
{
	//Serial Code
     int wd=img->width;
     int x;
     x=centroid(img);
     if( x>(wd/2+LIM*5))
     {
         printf("Right %d %d\n", x, wd/2);
		 comm.send_data('d');
     }
     else if(x<(wd/2-LIM*5))
     {
         printf("Left %d %d\n", x, wd/2);
		 comm.send_data('a');
     }
     else
     {
         printf("Straight %d %d\n", x, wd/2);
		 comm.send_data('w');
		 cvWaitKey(200);
     }
}

/*
 * 1. Separate out White,Black,Red,Green colors
 * 2. Find the biggest blobs from all of them
 * 3. Check if there is any big blob in green/red(i.e. any symbol)
 * 4. if yes,then follow the symbol until size of white blob on the ground reduces to a definite value & Analyse the image and start recording decisions
 *    if not, follow white blob on ground till 3. is true
 * 5. Take a turn accordingly.
 * 6. Turn until the size of white blob increases to a definite value.
 * 7. Repeat from step 3.
 */

/*
int main()
{
    //CvCapture *capture=cvCreateCameraCapture(-1);
	//CvCapture *capture=cvCaptureFromFile("rd0fin.wmv");
    CvCapture* capture = cvCaptureFromCAM(1);
	comm.startDevice("\\\\.\\COM11", 9600);
		if(!capture)
	{
		cout<<"camera not connected";
		cvWaitKey(0);
	}
    IplImage *real;
	do{
		real=cvQueryFrame(capture);
	}while(!real);
	cvNamedWindow("C", CV_WINDOW_AUTOSIZE);
	cvShowImage("C", real);
 	a=(int **)malloc((real->height)*sizeof(int*));
	for(int i=0;i<real->height;i++)
	{
		a[i]=(int *)malloc((real->width)*sizeof(int));
	}

	stack=(int **)malloc(((real->height)*(real->width))*sizeof(int*));
	for(int i=0;i<(real->height)*(real->width);i++)
	{
		stack[i]=(int *)malloc(2*sizeof(int));
	}
		//Creating blank copies of image
    IplImage *GREEN=CreateBlankCopyOf(real,1);
    IplImage *RED=CreateBlankCopyOf(real,1);
    IplImage *WHITE=CreateBlankCopyOf(real,1);
    IplImage *BLACK=CreateBlankCopyOf(real,1);
    IplImage *green=CreateBlankCopyOf(real,1);
    IplImage *red=CreateBlankCopyOf(real,1);
    IplImage *white=CreateBlankCopyOf(real,1);
    IplImage *graph=CreateBlankCopyOf(real,1);


    //Creating Windows
    cvNamedWindow("Real",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Green",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Red",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("White",CV_WINDOW_AUTOSIZE);
	int chk=0;
    while(1)
    {
        static int decision[5]={0};   // decision[0] for 'white',[1] for 'left',[2] for 'right',[3] for 'stop'
        int count_red=0,count_white=0,count_green=0,signal=0;
        real=cvQueryFrame(capture);
        if(real==NULL)
        {
            printf("Image Not Loaded\n");
			//Close Serial
        }
		
		printf("/f");
        colors(real,RED,GREEN,WHITE,BLACK);  
		cvShowImage("C", real);
        biggest_blobs(RED,red);					
        biggest_blobs(GREEN,green);
        biggest_blobs(WHITE,white);           
        cvShowImage("Green",green);
        cvShowImage("White",white);
        cvShowImage("Red",red);
		count_green=count(green);
        count_red=count(red);
        count_white=count(white);

        if(count_green>50)
        {
            //Checking if bot has to turn or not
            printf("TIME TO DECISION %d %d", decision[1], decision[2]);

            if(decision[1]>DEC_THRESH)
            {
                puts("DECISION MADE");
                printf("Left\n");
			  	comm.send_data('a');
				chk=1;
                continue;
            }
            else if(decision[2]>DEC_THRESH)
            {
                puts("DECISION MADE");
                printf("Right\n");
				comm.send_data('d');
				chk=2;
                continue;
            }
        }
		if(chk==1)
		{
			printf("hulllo %d %d\n", count_red, red->width/2);

			if(count_red<MIN_THRESH*5)
			{
				puts("leftwa");
				comm.send_data('a');
				continue;

			}
			chk=0;

		}
		else if(chk==2)
		{
			printf("hulllo %d %d\n", count_red, red->width/2);

			if(count_red<MIN_THRESH*5)
			{
				puts("rightwa");
				comm.send_data('d');
				continue;
			}
			chk=0;
		}
        //Setting values of decisions to zero;
        if(count_green<MIN_THRESH && (centroid(white)<white->width/2+LIM || count_red>MIN_THRESH ) )
        {
			printf("%d %d ", centroid(white), white->width/2);
            for(int i=0;i<4;i++) decision[i]=0;
			printf("\n\ndec0\n\n");
        }
        //Checking if any colored symbol is present or not
        if(count_green>MIN_THRESH*100 && count_green>count_red) signal=1;
        else if(count_red>MIN_THRESH && count_red>count_green) signal=-1;
        if(signal==1)
        {
            puts("GREEN");
            for(int i=0;i<graph->height;i++) for(int j=0;j<graph->width;j++) IMGDATA(graph,i,j,0)=IMGDATA(green,i,j,0);
           // to_graph_hor(green,graph);
            cvShowImage("Graph",graph);
            if(direction(graph)==1) decision[2]++;                            //Right Turn
            else if(direction(graph)==-1) decision[1]++;                      //Left Turn
            //Follow Image
            //follow(green);
			comm.send_data('o');
            puts("FOLLOWING GREEN");

        }
        else if(signal==-1)
        {
            puts("RED");
			if(count_red>MIN_THRESH*150)
				{
					comm.send_data('o');
					break;
			}
			else 
				{
					puts("FOLLOWING RED\n");
					follow(red);
			}
           
        }
        else //if(count_white>MIN_THRESH*10)
        {
            //Follow white blob
			puts("Going Straight");
			comm.send_data('w')d;
        }

        //cvShowImage("Real",real);
        //cvShowImage("Green",green);
        //cvShowImage("Red",red);
        //cvShowImage("White",white);
        if(cvWaitKey(50)==27) 
			{
				comm.send_data('o');
				break;
		}
    }

    //Releasing images
    cvReleaseImage(&real);
    cvReleaseImage(&RED);
    cvReleaseImage(&GREEN);
    cvReleaseImage(&WHITE);
    cvReleaseImage(&BLACK);
    cvReleaseImage(&red);
    cvReleaseImage(&green);
    cvReleaseImage(&white);
    cvReleaseImage(&graph);

    //Destroying Windows
    cvDestroyWindow("Real");
    cvDestroyWindow("Green");
    cvDestroyWindow("Red");
    cvDestroyWindow("White");
    cvDestroyWindow("RED_");
	
	cvDestroyWindow("C");
	comm.stopDevice();
	
    return 0;
}

*/