/*
 * richohUndistortMain.cpp
 *
 *  Created on: Mar 24, 2018
 *      Author: ros
 */
/*
 * omnidirectionalCamera.cpp
 *
 *  Created on: Mar 24, 2018
 *      Author: ros
 */
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/PoseStamped.h"
#include "std_msgs/Float64.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/ccalib/omnidir.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/videoio.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include "InscribedAngle.h"
using namespace cv;
using namespace std;
using namespace ros;
RNG rng(12345);
int main( int argc, char** argv )
{
//ROS publisher configuration
	init(argc, argv, "InscribedAngle");
	NodeHandle n;
	Publisher inscribedAngle_pub = n.advertise<std_msgs::Float64>("InscribedAngle", 1000);
	ros::Rate loop_rate(100);





VideoCapture cap(1);
if(!cap.isOpened()){
  std::cout << "Error opening video stream or file" << std::endl;
  return -1;
}


  Mat imageUndistorted,imageUndistortedPoints;
  Mat imageUndistortedFisheye;
  Mat Knew, KnewFish;
  Mat R;
  Size patternsize(6,4); //interior number of corners
  std::vector<Point2f> cornersRicoh, cornersTheta;
  namedWindow("live",WINDOW_AUTOSIZE);
  namedWindow("ThreshTarget",WINDOW_AUTOSIZE);
  //Target=Green Launch=Blue
  while(ros::ok)
  {
  Mat imageC,image,imageTarget,imageLaunch;
  vector<vector<Point> > contoursTarget, contoursLaunch;
  vector<Vec4i> hierarchyTarget, hierarchyLaunch;


  cap >> imageC;
cvtColor(imageC,image,COLOR_BGR2HSV);
inRange(image,Scalar(50,0.4*255,0.2*255),Scalar(80,255,0.9*255),imageTarget);
inRange(image,Scalar(100,0.7*255,0.4*255),Scalar(140,255,255),imageLaunch);


//FOR TARGET
erode(imageTarget, imageTarget, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
dilate( imageTarget, imageTarget, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );

//morphological closing (fill small holes in the foreground)
dilate( imageTarget, imageTarget, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
erode(imageTarget, imageTarget, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );

findContours(imageTarget,contoursTarget,hierarchyTarget, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
/// Approximate contours to polygons + get bounding rects and circles
vector<vector<Point> > contours_poly( contoursTarget.size() );
vector<Rect> boundRectTarget( contoursTarget.size() );

for( int i = 0; i < contoursTarget.size(); i++ )
   { approxPolyDP( Mat(contoursTarget[i]), contours_poly[i], 3, true );
     boundRectTarget[i] = boundingRect( Mat(contours_poly[i]) );
   }

Rect rectTarget=boundRectTarget[0];

for(int i=0;i<boundRectTarget.size();i++)
{
	if(boundRectTarget[i+1].area()>rectTarget.area())
	{
		rectTarget = boundRectTarget[i+1];
	}

}
Scalar color = Scalar(0, 255, 0 );
rectangle(imageC, rectTarget.tl(), rectTarget.br(), color, 2, 8, 0 );

//FOR LAUNCH

erode(imageLaunch, imageLaunch, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
dilate( imageLaunch, imageLaunch, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );

//morphological closing (fill small holes in the foreground)
dilate( imageLaunch, imageLaunch, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
erode(imageLaunch, imageLaunch, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );

findContours(imageLaunch,contoursLaunch,hierarchyLaunch, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
/// Approximate contours to polygons + get bounding rects and circles
vector<vector<Point> > contours_polyLaunch(contoursLaunch.size());
vector<Rect> boundRectLaunch( contoursLaunch.size() );

for( int i = 0; i < contoursLaunch.size(); i++ )
   { approxPolyDP( Mat(contoursLaunch[i]), contours_polyLaunch[i], 3, true );
     boundRectLaunch[i] = boundingRect( Mat(contours_polyLaunch[i]) );
   }

Rect rectLaunch=boundRectLaunch[0];

for(int i=0;i<boundRectLaunch.size();i++)
{
	if(boundRectLaunch[i+1].area()>rectLaunch.area())
	{
		rectLaunch = boundRectLaunch[i+1];
	}

}
Scalar colorlaunch = Scalar(0, 0, 255 );
rectangle(imageC, rectLaunch.tl(), rectLaunch.br(), colorlaunch, 2, 8, 0 );

//Inscribed Angle
Point centerTarget = (rectTarget.br() + rectTarget.tl())*0.5;
Point centerLaunch = (rectLaunch.br() + rectLaunch.tl())*0.5;

std_msgs::Float64 msg;

angles angles;

angles = inscribedAngleCalcColor(centerTarget,centerLaunch,1);

msg.data = angles.inscribedAngle;
inscribedAngle_pub.publish(msg);

printf("%s%f%s%f%s%f","launchAngle= ",angles.launchAngle," targetAngle=",angles.targetAngle," InscribedAngle=",angles.inscribedAngle);
cout << endl;

imshow( "live", imageC );
imshow( "ThreshTarget",imageLaunch);
waitKey(10);

ros::spinOnce();
loop_rate.sleep();


  }
return 0;
}
