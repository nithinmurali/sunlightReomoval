
#include "common.h"
#include "sunlight_flicker_removal.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat imgOriginal,finalop;
    VideoCapture cap(argv[1]); // try capture the video
    SunlightRemoval sun;
    ButterWorthFilter bwf;
    
    while (true)
    {
        int k = waitKey(33);
        bool bSuccess = cap.read(imgOriginal); // read a new frame from video
        if (!bSuccess || k=='q') //if not success, break loop
        {
            cout << "Cannot read a frame from video stream" << endl;
            break;
        }
        imshow("original",imgOriginal);

        finalop = sun.run(imgOriginal);
        //bwf.setParams(1,2,1);
        //finalop = bwf.filter(imgOriginal);
        //finalop = imgOriginal;
        
        imshow("final",finalop);
    }
}