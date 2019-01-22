#include <stdio.h>
#include <vector>
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <time.h>
#include <math.h> 
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp" 

using namespace std;
using namespace cv;

vector<double>Ritos;

//获取中心点附近的一个矩形
Mat GetROi( Mat src )
{
   Mat Temp;
   Point center =Point(src.cols/2,src.rows/2);
   int width   =300;
   int height  =300;
   Rect roi =Rect(center.x-width/2,center.y-height/2,width,height);

   Temp =src(roi);

   return Temp;
}

 //计算出圆形度factor
double centerPoints(vector<Point>contour)
{
    double factor = (contourArea(contour) * 4 * CV_PI) /(pow(arcLength(contour, true), 2));
    return factor;
}

int main()
{
    VideoCapture capture(-1);
    if(!capture.isOpened())
    {
        std::cout<<"video not open."<<std::endl;
        return 1;
    }
    Mat imgSrc ;
    Mat Temp;
    while( 1 )
    { 
    	  capture>>imgSrc;
        double xFactor=1.0;
        double yFactor=1.0;
 	      Mat Roi;

        if( imgSrc.cols>300 && imgSrc.rows>300 )
        {
          Roi =GetROi(imgSrc);
        }

        int xShift =imgSrc.cols/2-150;
        int yShift =imgSrc.rows/2-150;
        
        Temp = Roi.clone();
        //imgSrc =imread("3.jpg");
    
    	  Mat gray ;  
      	//转换为灰度图
    	  cvtColor(Roi, gray, CV_BGR2GRAY);

    	  //降噪
    	  blur(gray, gray, Size(3,3));
    	  //运行Canny算子，3为threshold1，9为threshold2
    	  Mat edge; 
    	  Canny(gray, edge, 30, 120,3); 

    	  std::vector< std::vector< cv::Point> > contours;  
    	  cv::findContours(  
        edge,  
        contours,  
        cv::noArray(),  
        CV_RETR_TREE,  
        CV_CHAIN_APPROX_NONE  
        );  
    
    	  cv::drawContours(Temp, contours, -1, cv::Scalar(0,0,255),2);
        //imshow("Temp",Temp);
        //waitKey(1);

    	  Ritos.clear();
    	  vector<vector<Point>>contoursFinal;  
    	  for (unsigned int i = 0; i < contours.size(); ++i)
    	  {
        	 double circleRito = centerPoints(contours[i]);
        	 if( circleRito >= 0.85 )  
        	 {
           		contoursFinal.push_back( contours[i] );
        	 }
    	  }

    	  vector<vector<Point> >::iterator itc= contoursFinal.begin();  
    	  while (itc!=contoursFinal.end()) 
    	 {  
        	if( itc->size()<=30)
        	{  
           	 	itc= contoursFinal.erase(itc);  
        	}
        	else
        	{  
           	 	++itc;  
        	}  
    	}  

       //计算矩
       vector<Moments>mu(contoursFinal.size());
       for (unsigned int i = 0; i < contoursFinal.size(); i++)
       {
           mu[i] = moments(contoursFinal[i], false);
       }

       //计算矩中心
       vector<Point2f>mc(contoursFinal.size());
       for (unsigned int i = 0; i < contoursFinal.size(); i++)
       {
          mc[i] = Point2f(static_cast<float>(mu[i].m10 / mu[i].m00), static_cast<float>(mu[i].m01 / mu[i].m00));
       }
       for (unsigned int i = 0; i< contoursFinal.size(); ++i)
       {
         circle(imgSrc, Point(mc[i].x+xShift,mc[i].y+yShift), 1, Scalar(0, 0, 255), -1, 8, 0);
         Rect rectRoi=boundingRect(contoursFinal.at(i));
         int radius  =rectRoi.width/2;
 
         circle(imgSrc, Point(mc[i].x+xShift,mc[i].y+yShift), radius, Scalar(0, 255, 0), 2, 8, 0);

         string string1 =to_string(int(mc[i].x));
	       string string2 =to_string(int(mc[i].y));

         string string  ="("+string1+","+string2+")";

         //输出质心坐标
         putText(imgSrc, string, Point(mc[i].x+xShift, mc[i].y-10+yShift), FONT_HERSHEY_SIMPLEX,1, Scalar(255, 0, 255), 2);

        }
        rectangle(imgSrc,Rect(xShift,yShift,300,300),Scalar(255,0,0,0),2,8,0);
    	cout <<"size"<<"\t"<<contoursFinal.size()<<endl;
    	//cv::drawContours(Roi, contoursFinal, -1, cv::Scalar(255,0,0),2); 
      
    	cv::imshow("imgSrc", imgSrc);  
      
    	cv::waitKey(1); 
     } 
     return 0; 
} 
