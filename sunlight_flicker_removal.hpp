#ifndef SUNLIGHTREMOVAL
#define SUNLIGHTREMOVAL

#include "common.h"
#include "butterworthfilter.hpp"

using namespace cv;
using namespace std;
using namespace std::chrono;

class SunlightRemoval
{
    private:
        int N; // buffer length
        std::vector<cv::Mat_<Vec3f> > _inputImages; //Set of input images used for the process
        std::vector<cv::Mat_<Vec3f> > _registeredImages; //Set of input images used for the process
        //std::vector<cv::Mat_<Vec3f> > logImages; //log of images
    
        cv::Mat_<Vec3f> _temporalMedian;

        cv::Mat _srcImage;
        cv::Mat _finalImage;
        
        VideoWriter vout;

        //update image buffer with current image
        void updateInputImages();
        //reigister images w.r.t to the src image
        void registerImages();
        //update image buffer
        bool upadteImageBuffer(Mat img);

    public:
        SunlightRemoval();
        //main fn
        Mat run(Mat src);
 
};

#endif