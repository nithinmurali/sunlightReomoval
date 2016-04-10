#include "sunlight_flicker_removal.hpp"

SunlightRemoval::SunlightRemoval()
{
    N=4;
    _inputImages.clear();
    _registeredImages.clear();

     VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),10, Size(320,240),true);
     vout = video;
}

void SunlightRemoval::registerImages()
{
    _registeredImages = _inputImages;
}

bool SunlightRemoval::upadteImageBuffer(Mat img)
{
    if (_inputImages.size() >= N)
    {
        _inputImages.erase(_inputImages.begin());
        _inputImages.push_back(img);
        return 1;
    }
    else
    {
        _inputImages.push_back(img);
        return 0;
    }
}

Mat SunlightRemoval::run(Mat src)
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    Mat tmp1,tp1;
    
    bool flag = upadteImageBuffer(src);
    if (!flag) // buffer not full
    {
        return src;
    }
    
    src.copyTo(_srcImage);
    _srcImage.convertTo(_srcImage, CV_32FC3);
    Mat src_log = src;
    src_log.convertTo(src_log, CV_32FC3);
    //cout<<"1"<<endl;
    cv::log(src_log,src_log);
    registerImages();
    
    //convert to log
    std::vector<cv::Mat_<Vec3f> > logImages;
    logImages.resize(_registeredImages.size());
    
    for (int i = 0; i < N; ++i)
    {
        //cv::log(_registeredImages[i], logImages[i]);  
        _registeredImages[i].copyTo(logImages[i]);
    }

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    //cout<<"time taken 1 "<<duration<<endl;

    //calculate the temporal median
    _temporalMedian = cv::Mat::zeros(src.size(), CV_32FC3);
    _temporalMedian = _registeredImages[0];
    
    std::vector<Vec3i> sum;
    sum.resize(logImages.size());
    std::vector<int> sum1[3];
    
    for (int j = 0; j < src.rows; ++j)
    {
        for (int k = 0; k < src.cols; ++k)
        {
            for (int f = 0; f < logImages.size() ; ++f)
            {
                sum[f] = logImages[f].at<Vec3f>(j,k);
                if (f == logImages.size()-1)
                {
                    int size = logImages.size();
                    for (int i = 0; i < sum.size(); ++i)
                    {
                        sum1[0].push_back(sum[i][0]);
                        sum1[1].push_back(sum[i][1]);
                        sum1[2].push_back(sum[i][2]);
                    }
                    
                    std::sort(sum1[0].begin(),sum1[0].end());                    
                    std::sort(sum1[1].begin(),sum1[1].end());                    
                    std::sort(sum1[2].begin(),sum1[2].end());                   
                    //cout<<"2"<<endl;
                    //std::sort(sum.begin(),sum.end());                   
                    
                    if (size  % 2 == 0)
                    {
                      _temporalMedian.at<Vec3f>(j,k)[0] = (sum1[0][size / 2 - 1] + sum1[0][size / 2])*0.5;
                      _temporalMedian.at<Vec3f>(j,k)[1] = (sum1[1][size / 2 - 1] + sum1[1][size / 2])*0.5;
                      _temporalMedian.at<Vec3f>(j,k)[2] = (sum1[2][size / 2 - 1] + sum1[2][size / 2])*0.5;
                    }
                    else 
                    {
                      _temporalMedian.at<Vec3f>(j,k)[0] = sum1[0][size / 2];
                      _temporalMedian.at<Vec3f>(j,k)[1] = sum1[1][size / 2];
                      _temporalMedian.at<Vec3f>(j,k)[2] = sum1[2][size / 2];
                    }
                    
                    break;
                }
            }
        }
    }
    
    //exp(_temporalMedian,_temporalMedian);
    _temporalMedian.copyTo(tp1);
    tp1.convertTo(tp1,CV_8UC3);
    imshow("_temporalMedian",tp1);

    //find the difference
    Mat di(_srcImage.rows,_srcImage.cols, CV_32FC3, Scalar::all(0));
    Mat dif(_srcImage.rows,_srcImage.cols, CV_32FC3, Scalar::all(0));
    di = _srcImage - _temporalMedian;
    //cout<<"4"<<endl;
    tmp1 = di;
    tmp1.copyTo(tp1);
    tp1.convertTo(tp1,CV_8UC3);
    imshow("diff",tp1);
    vout.write(tp1);

    //applay low pass filter
    /*ButterWorthFilter bwf;
    bwf.setParams(1,2,1);
    dif = bwf.filter(di);*/

    GaussianBlur( di, dif, Size( 1, 1 ), 0, 0 );
    
    tmp1 =dif ;
    tmp1.copyTo(tp1);
    tp1.convertTo(tp1,CV_8UC3);
    imshow("diff lowpass",tp1);
    //illumination corrected image
    Mat logFinal(_srcImage.rows,_srcImage.cols, CV_64FC3, Scalar::all(0));
    logFinal = _srcImage - dif;

    //recover linear pixel intensities
    //exp(logFinal,logFinal);
    logFinal.convertTo(_finalImage, CV_8UC3);
    
    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t1 ).count();
    //cout<<"time taken tot "<<duration1<<endl;
    
    return _finalImage;
}

/*Mat SunlightRemoval::run(Mat src)
{
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
    Mat tmp1,tp1;
    
    bool flag = upadteImageBuffer(src);
    if (!flag) // buffer not full
    {
        return src;
    }
    
    src.copyTo(_srcImage);
    
    registerImages();
    
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    //cout<<"time taken 1 "<<duration<<endl;

    //calculate the temporal median
    _temporalMedian = cv::Mat::zeros(src.size(), CV_8UC3);

    for (int j = 0; j < src.rows; ++j)
    {
        for (int k = 0; k < src.cols; ++k)
        {
            std::vector<Vec3i> sum;
            sum.resize(_registeredImages.size());
            
            for (int f = 0; f < _registeredImages.size() ; ++f)
            {
                sum[f] = _registeredImages[f].at<Vec3i>(j,k);
                if (f == _registeredImages.size()-1)
                {
                    std::vector<float> sum1[3];
                    int size = _registeredImages.size();
                    for (int i = 0; i < sum.size(); ++i)
                    {
                        sum1[0].push_back(sum[i][0]);
                        sum1[1].push_back(sum[i][1]);
                        sum1[2].push_back(sum[i][2]);
                    }
                    
                    std::sort(sum1[0].begin(),sum1[0].end());                    
                    std::sort(sum1[1].begin(),sum1[1].end());                    
                    std::sort(sum1[2].begin(),sum1[2].end());                   
                    //cout<<"2"<<endl;
                    
                    if (size  % 2 == 0)
                    {
                      _temporalMedian.at<Vec3i>(j,k)[0] = (sum1[0][size / 2 - 1] + sum1[0][size / 2])*0.5;
                      _temporalMedian.at<Vec3i>(j,k)[1] = (sum1[1][size / 2 - 1] + sum1[1][size / 2])*0.5;
                      _temporalMedian.at<Vec3i>(j,k)[2] = (sum1[2][size / 2 - 1] + sum1[2][size / 2])*0.5;
                    }
                    else 
                    {
                      _temporalMedian.at<Vec3i>(j,k)[0] = sum1[0][size / 2];
                      _temporalMedian.at<Vec3i>(j,k)[1] = sum1[1][size / 2];
                      _temporalMedian.at<Vec3i>(j,k)[2] = sum1[2][size / 2];
                    }
                    
                    break;
                }
            }
        }
    }
    
    _temporalMedian.copyTo(tp1);
    tp1.convertTo(tp1,CV_8UC3);
    imshow("_temporalMedian",tp1);

    cout<<"1"<<endl;
    //find the difference
    Mat di, dif;
    di.convertTo(di,CV_8UC3);
    dif.convertTo(dif,CV_8UC3);
    cout<<_srcImage.type()<<" "<<_temporalMedian.type();
    di = _srcImage - _temporalMedian;
    
    cout<<"2"<<endl;
    
    //applay low pass filter
    //ButterWorthFilter bwf;
    //bwf.setParams(1,2,1);
    //dif = bwf.filter(di);

    GaussianBlur( di, dif, Size( 1, 1 ), 0, 0 );
    
    imshow("diff lowpass",di);
    //illumination corrected image
    _finalImage = _srcImage - dif;
    _finalImage.convertTo(_finalImage, CV_8UC3);
    
    high_resolution_clock::time_point t3 = high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>( t3 - t1 ).count();
    //cout<<"time taken tot "<<duration1<<endl;
    
    return _finalImage;
}*/