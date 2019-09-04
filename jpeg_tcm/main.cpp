//
//  main.cpp
//  TCM
//
//  Created by Hossam Amer on 2018-06-27.
//  Copyright © 2018 Hossam Amer. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <math.h>
//#include <ctime>
//#include <cv.h>
//#include <cxcore.h>
//#include <highgui.h>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;


Scalar getMSSIM( const Mat& i1, const Mat& i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d     = CV_32F;
    
    Mat I1, I2;
    i1.convertTo(I1, d);           // cannot calculate on one byte large values
    i2.convertTo(I2, d);
    
    Mat I2_2   = I2.mul(I2);        // I2^2
    Mat I1_2   = I1.mul(I1);        // I1^2
    Mat I1_I2  = I1.mul(I2);        // I1 * I2
    
    /*************************** END INITS **********************************/
    
    Mat mu1, mu2;   // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);
    
    Mat mu1_2   =   mu1.mul(mu1);
    Mat mu2_2   =   mu2.mul(mu2);
    Mat mu1_mu2 =   mu1.mul(mu2);
    
    Mat sigma1_2, sigma2_2, sigma12;
    
    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;
    
    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;
    
    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;
    
    ///////////////////////////////// FORMULA ////////////////////////////////
    Mat t1, t2, t3;
    
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))
    
    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))
    
    Mat ssim_map;
    divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;
    
    Scalar mssim = mean( ssim_map ); // mssim = average of ssim map
    return mssim;
}

char* read_yuv(string filename, int width, int height)
{
    int size = width*height;
    std::ifstream myfile (filename, std::ifstream::binary);
    
    if(myfile)
    {
        // get length of file:
        myfile.seekg (0, myfile.end);
        long long length = myfile.tellg();
        myfile.seekg (0, myfile.beg);
        
        char * buffer = new char [length];
        
        std::cout << "Reading " << length << " characters... " << endl;
        // read data as a block:
        myfile.read (buffer, length);
        
        if (myfile)
            std::cout << "all characters read successfully." << endl;
        else
            std::cout << "error: only " << myfile.gcount() << " could be read" << endl;
        myfile.close();
        
        return buffer;
    }
    
    return NULL;
    
}

// video-input-ssim.htm
int main(int argc, char** argv) {
    
    //create a gui window:
    namedWindow("Output",1);
    
    //
    // read image 1
    string f1 = "/Volumes/MULTICOMHD2/validation_original/1/ILSVRC2012_val_00000037.JPEG";
    string  f2 = "/Volumes/MULTICOMHD2/validation_generated_QF/1/ILSVRC2012_val_00000037-QF-80.JPEG";
    
    Mat image1;
    image1 = imread(f1);
    
    Mat image2;
    image2 = imread(f2);
    
    Scalar out1 = getMSSIM(image1, image2);
    cout << "Value  " << out1 << endl;
    
    
    int height = 240;
    int width = 416;
    // Read YUV
    string f1_yuv = "/Volumes/DATA/TS/BQSquare_416x240_60.yuv";
    string  f2_yuv = "/Users/hossam.amer/7aS7aS_Works/work/workspace/TESTS/hevc_intraML_bits/bin/Build/Products/Release/Gen/Seq-RECONS/BQSquare_416x240_60_51_1.yuv";
    
    
    char* buf_YUV = read_yuv(f1_yuv, width, height);
    
    if(buf_YUV == NULL)
    {
        cout << "buf_YUV file not found." << endl;
        exit(0);
    }
    
    char * buf_Y = new char[width * height];
    for (int i = 0; i < width * height; i++)
    {
        buf_Y[i] = buf_YUV[i];
    }
    
    
    char* buf_YUV2 = read_yuv(f2_yuv, width, height);
    
    if(buf_YUV2 == NULL)
    {
        cout << "buf_YUV2 file not found." << endl;
        exit(0);
    }
    
    char * buf_Y2 = new char[width * height];
    for (int i = 0; i < width * height; i++)
    {
        buf_Y2[i] = buf_YUV2[i];
    }
    
    
    // Convert Y into Mats
    cv::Mat Y1(height, width, CV_8UC1, &buf_Y[0]); //in case of BGR image use CV_8UC3
    cv::Mat Y2(height, width, CV_8UC1, &buf_Y2[0]); //in case of BGR image use CV_8UC3
    
    
    Scalar final_mssim = getMSSIM(Y1, Y2);
    cout << "Final MSSIM Value  " << final_mssim << endl;
    
    delete [] buf_Y;
    delete [] buf_Y2;
    
    
    return 0;
    
}

