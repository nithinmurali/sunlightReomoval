#ifndef BWF
#define BWF

#include "common.h"
using namespace cv;

class ButterWorthFilter
{

public:

    enum Mode
    {
        LOW_PASS=0,BAND_REJECT=1
    };

    //ButterWorthFilter(float , int , Mode mode , float );
    void setParams(float r, int n, float w, Mode mode = LOW_PASS);
    float getGain();
    Mat filter(Mat img);

private:
    float radius;              // low pass filter parameter
    int order;
    float width;
    Mode type;

    void shiftDFT(Mat &fImage );
    Mat create_spectrum_magnitude_display(Mat &complexImg, bool rearrange);
    void create_butterworth_filter(Mat &dft_Filter);
    void create_butterworth_lowpass_filter(Mat &dft_Filter, int D, int n, int W);
};

#endif