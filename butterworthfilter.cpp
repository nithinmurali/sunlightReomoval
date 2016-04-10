#include "butterworthfilter.hpp"
/*
ButterWorthFilter::ButterWorthFilter(float r=2, int n=4, Mode mode = LOW_PASS, float w=3)
{
    radius = r;
    order  = n;
    width  = w;
    type = mode;
}
*/
/**
 * set filter params
 * @param r radius
 * @param n order
 * @param w width
 */
void ButterWorthFilter::setParams(float r, int n, float w, Mode mode)
{
    radius = r;
    order  = n;
    width  = w;
    type   = mode;
}

float getGain()
{
    return 1;
}

/**
 * filter an inp image
 * @param  img image to be filtered
 * @return     filtered output
 */
Mat ButterWorthFilter::filter(Mat img)
{
    Mat padded;     // fourier image objects and arrays
    Mat complexImg, filter, filterOutput;
    Mat planes[2];
    Mat imgGray;  // image object(s)
    int N, M; // fourier image sizes
    int n;
    // setup the DFT image sizes
    M = getOptimalDFTSize( img.rows );
    N = getOptimalDFTSize( img.cols );
    std::vector<Mat> channels;    
    n = img.channels();
    split(img,channels);

    for (int i = 0; i < n; ++i)
    {
        imgGray = channels[i];
        
        // setup the DFT images
        copyMakeBorder(imgGray, padded, 0, M - imgGray.rows, 0, N - imgGray.cols, BORDER_CONSTANT, Scalar::all(0));
        planes[0] = Mat_<float>(padded);
        planes[1] = Mat::zeros(padded.size(), CV_32F);

        merge(planes, 2, complexImg);

        // do the DFT
        dft(complexImg, complexImg);

        // construct the filter (same size as complex image)
        filter = complexImg.clone();
        create_butterworth_filter(filter);
        
        // apply filter
        shiftDFT(complexImg);
        mulSpectrums(complexImg, filter, complexImg, 0);
        shiftDFT(complexImg);

        // create magnitude spectrum for display
        //mag = create_spectrum_magnitude_display(complexImg, true);

        // do inverse DFT on filtered image
        idft(complexImg, complexImg);

        // split into planes and extract plane 0 as output image
        cv::Mat myplanes[2];
        split(complexImg, myplanes);
        channels[i] = myplanes[0];

        // do the same with the filter image
        split(filter, planes);
        normalize(planes[0], filterOutput, 0, 1, CV_MINMAX);
        imshow("filter Img",filterOutput);
    }
    merge(channels,img);
    return img;
}

/**
 * create a 2-channel butterworth low-pass filter mask
 * @param dft_Filter filter image
 */
void ButterWorthFilter::create_butterworth_filter(Mat &dft_Filter)
{
    Mat tmp = Mat(dft_Filter.rows, dft_Filter.cols, CV_32F);

    Point centre = Point(dft_Filter.rows / 2, dft_Filter.cols / 2);
    double cradius;

    //cout<<"radius:"<<radius<<" order:"<<order<<" width:"<<width<<endl;
    for (int i = 0; i < dft_Filter.rows; i++)
    {
        for (int j = 0; j < dft_Filter.cols; j++)
        {
            cradius = (double) sqrt(pow((i - centre.x), 2.0) + pow((double) (j - centre.y), 2.0));

            if (type == LOW_PASS)
            {
                // Butterworth low pass:
                tmp.at<float>(i,j) = (float)
                                        ( 1 / (1 + pow((double) (cradius / radius), (double) (2 * order))));
            }
            else if (type == BAND_REJECT)
            {
                // Butterworth band reject
                tmp.at<float>(i, j) = (float)
                                      ( 1 / (1 + pow((double) (cradius * width) / ( pow((double)cradius, 2) - radius * radius ), (double) (2 * order))));
            }
        }
    }

    Mat toMerge[] = {tmp, tmp};
    merge(toMerge, 2, dft_Filter);
}

/**
 * rearrange quadrents of dft image do that orgin is at image center
 * @param fImage rearranged image
 */
void ButterWorthFilter::shiftDFT(Mat &fImage )
{
    Mat tmp, q0, q1, q2, q3;

    // first crop the image, if it has an odd number of rows or columns

    fImage = fImage(Rect(0, 0, fImage.cols & -2, fImage.rows & -2));

    int cx = fImage.cols / 2;
    int cy = fImage.rows / 2;

    q0 = fImage(Rect(0, 0, cx, cy));
    q1 = fImage(Rect(cx, 0, cx, cy));
    q2 = fImage(Rect(0, cy, cx, cy));
    q3 = fImage(Rect(cx, cy, cx, cy));

    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

/**
 * create  floating point spectrum magnitude image scaled for user viewing
 * @param  complexImg  the complext dft image
 * @param  rearrange   flag for rearranging
 * @return             marnitude image
 */
Mat ButterWorthFilter::create_spectrum_magnitude_display(Mat &complexImg, bool rearrange)
{
    Mat planes[2];

    // compute magnitude spectrum (N.B. for display)
    // compute log(1 + sqrt(Re(DFT(img))**2 + Im(DFT(img))**2))

    split(complexImg, planes);
    magnitude(planes[0], planes[1], planes[0]);

    Mat mag = (planes[0]).clone();
    mag += Scalar::all(1);
    log(mag, mag);

    if (rearrange)
    {
        // re-arrange the quaderants
        shiftDFT(mag);
    }

    normalize(mag, mag, 0, 1, CV_MINMAX);

    return mag;

}
