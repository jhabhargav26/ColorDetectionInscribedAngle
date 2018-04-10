/*
 * InscribedAngle.h
 *
 *  Created on: Apr 7, 2018
 *      Author: ros
 */

#ifndef INSCRIBEDANGLE_H_
#define INSCRIBEDANGLE_H_
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

angles inscribedAngleCalc(vector<Marker> TheMarkers, int TargetID, int LaunchID, int resizeFactor=1);
angles inscribedAngleCalcColor(Point TargetCenter, Point LaunchCenter, int resizeFactor=1);



#endif /* INSCRIBEDANGLE_H_ */
