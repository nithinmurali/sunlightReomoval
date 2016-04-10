#ifndef COMMON1_H
#define COMMON1_H

/**
 * @file common.h
 * @brief file containing common header files
 * @author Prashant Iyengar
 */

/** @brief file containing common header files*/
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/ml/ml.hpp"
#include <fstream>

#include <chrono>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "stdio.h"
#include "string.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <sys/time.h>
using namespace cv;
using namespace std;
#include <string>
#include <sstream>
#include <iostream>
#include "opencv2/imgproc/types_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv/cv.h"
#include <stdlib.h>
#include <vector>
#include <stack>
#include <iostream>
#include <map>
#include <ctime>
#include "opencv2/highgui/highgui.hpp"

namespace AUV
{
//post processing parameters
const int SRG_MODE_DEFAULT=0;
const int SRG_MODE_MODIFIED=1;
const int SRG_DIST_MODE_DEFAULT=2;
const int SRG_DIST_MODE_NORM_1=3;
const int SRG_DIST_MODE_NORM_2=4;
const int PRE_LOCALCE=5;
const int PRE_LOCALAHE=6;
const int COLOR_RGB=7;
const int COLOR_HSV=8;
const int COLOR_RED=20;
const int COLOR_GREEN=21;
const int COLOR_BLUE=22;
const int COLOR_YELLOW=23;
const int COLOR_ORANGE=24;
}

#endif // COMMON1_H
