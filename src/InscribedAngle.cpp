/*
 * InscribedAngle.cpp
 *
 *  Created on: Apr 7, 2018
 *      Author: ros
 */

 #include <aruco.h>
#include "cvdrawingutils.h"
#include <fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ccalib/omnidir.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/videoio.hpp>
#include <sstream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <sstream>

using namespace std;
using namespace cv;
using namespace aruco;
using namespace chrono;

typedef struct angles{
	float launchAngle;
	float targetAngle;
	float inscribedAngle;
}angles;

angles inscribedAngleCalc(vector<Marker> TheMarkers,int TargetID, int LaunchID, int resizeFactor)
{

angles angles;
Point2f center, centerRicoh, centerTheta;
//ANGLE DETECTION CODE
//angle calculation RICOH side
double angle;
double launchAngle =1000;
double targetAngle =1000;
for (unsigned int i = 0; i < TheMarkers.size(); i++)
            {	angle = 0;
                if(TheMarkers[i].id==LaunchID ||TheMarkers[i].id==TargetID)
                {
                center=TheMarkers[i].getCenter();
                if(center.y<640)
                {
                double ratioR = ((double)center.x)/(640*resizeFactor);
                angle = 180*(ratioR-0.5);

                }
                else
                {
                double ratioT = ((double)center.x-(640.0*resizeFactor))/(640*resizeFactor);
                angle = 180*(ratioT-0.5)+180;
                }

                }
                if(angle<0.0){angle = 360+angle;} //For Ricoh Side negative angle
                if(TheMarkers[i].id==LaunchID) launchAngle = angle;
                else if (TheMarkers[i].id==TargetID) targetAngle = angle;
            }
	angles.launchAngle = launchAngle;
	angles.targetAngle = targetAngle;
	angles.inscribedAngle = launchAngle - targetAngle;

return angles;
	}


