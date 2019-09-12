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

template <class Container>
static void split1(const std::string& str, Container& cont, char delim = ' ')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        
        cont.push_back(token);
    }
}

static int convertStringToNumber(const string& str)
{
    int number;
    std::istringstream iss (str);
    iss >> number;
    
    return number;
}

double jpeg_encoder::getPSNR(const cv::Mat& i1, const cv::Mat& i2, int comp_width, int comp_height)
{

    int d     = CV_32F;
    Mat I1, I2;
    i1.convertTo(I1, d);           // cannot calculate on one byte large values
    i2.convertTo(I2, d);


    Mat s1;
    absdiff(I1, I2, s1);       // |I1 - I2|
    s1.convertTo(s1, CV_32F);  // cannot make a square on 8 bits
    s1 = s1.mul(s1);           // |I1 - I2|^2

    Scalar s = sum(s1);         // sum elements per channel
    double mse = (s[0]); // sum channels

    if( mse <= 1e-10) // for small values return zero
        return 999.99;
    else
    {
        double psnr = 10.0*log10(double(255*255*double(comp_width*comp_height))/mse);
        return psnr;
    }
}



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
        {
//         std::cout << "all characters read successfully." << endl;   
        }
        else
            std::cout << "error: only " << myfile.gcount() << " could be read" << endl;
        myfile.close();
        
        return buffer;
    }
    
    return NULL;
    
}

// video-input-ssim.htm
int main(int argc, char** argv) {
    
    
    if(argc < 4)
    {
        // Tell the user how to run the program
        std::cerr << "Number of arguments should be 3: <file1> <file2> <out_file>" << std::endl;
        /* "Usage messages" are a conventional way of telling the user
         * how to run a program if they enter the command incorrectly.
         */
        return 1;
    }
    
    // Input file:
    std::string f1_yuv = argv[1];

    // Input file:
    std::string f2_yuv = argv[2];
    
    // Output text file:
    std::string out_file = argv[3];
    
    size_t found = f1_yuv.find_last_of("/\\");
    std::string filename_first_token = f1_yuv.substr(found+1);
    found = filename_first_token.find_first_of(".");
    std::string filename_second_token = filename_first_token.substr(0, found); // before yuv
    
    // get width and height
    found = filename_second_token.find_last_of("_");
    
    vector<string> sLists;
    split1(filename_second_token, sLists, '_');
    unsigned long nsLength = sLists.size();
    
    // width and height
    int height = convertStringToNumber(sLists[nsLength - 2]);
    int width  = convertStringToNumber(sLists[nsLength - 3]);
    string rgbStr = sLists[nsLength-1];
    int nComponents = 1;

    if(rgbStr == "RGB")
    {
        nComponents = 3;
    }
    
    char* buf_YUV = read_yuv(f1_yuv, width, height);
    char* buf_YUV2 = read_yuv(f2_yuv, width, height);
    
    if(buf_YUV == NULL)
    {
        cout << "buf_YUV file not found." << endl;
        exit(0);
    }
    
    if(buf_YUV2 == NULL)
    {
        cout << "buf_YUV2 file not found." << endl;
        exit(0);
    }
    
    // YUV
    char * buf_Y = new char[width * height];
    char * buf_U = new char[width * height / 4];
    char * buf_V = new char[width * height / 4];
    
    // YUV2
    char * buf_Y2 = new char[width * height];
    char * buf_U2 = new char[width * height / 4];
    char * buf_V2 = new char[width * height / 4];
    
    for (int i = 0; i < width * height; i++)
    {
        buf_Y[i]  = buf_YUV[i];
        buf_Y2[i] = buf_YUV2[i];
    }
    
    if(nComponents > 1)
    {
        int start = width*height;
        int end = start + (width*height/4);
        
        for(int i = start; i < end; i++)
        {
            buf_U[i]   = buf_YUV[i];
            buf_U2[i] = buf_YUV2[i];
        }
        
        int start2 = end;
        int end2   = start + (width*height/4);
        
        for(int i = start2; i < end2; i++)
        {
            buf_V[i]    = buf_YUV[i];
            buf_V2[i]   = buf_YUV2[i];
        }
    }
    
    // Convert Y into Mats
    cv::Mat Y1(height, width, CV_8UC1, &buf_Y[0]); //in case of BGR image use CV_8UC3
    cv::Mat Y2(height, width, CV_8UC1, &buf_Y2[0]); //in case of BGR image use CV_8UC3
    
    Scalar final_mssim = getMSSIM(Y1, Y2);
    double y_ssim = final_mssim[0];
    double u_ssim = 0;
    double v_ssim = 0;
    
    if(nComponents > 1)
    {
        cv::Mat U1(height/2, width/2, CV_8UC1, &buf_U[0]); //in case of BGR image use CV_8UC3
        cv::Mat U2(height/2, width/2, CV_8UC1, &buf_U2[0]); //in case of BGR image use CV_8UC3
        Scalar final_mssim = getMSSIM(U1, U2);
        u_ssim = final_mssim[0];
        
        cv::Mat V1(height/2, width/2, CV_8UC1, &buf_V[0]); //in case of BGR image use CV_8UC3
        cv::Mat V2(height/2, width/2, CV_8UC1, &buf_V2[0]); //in case of BGR image use CV_8UC3
        final_mssim = getMSSIM(U1, U2);
        v_ssim = final_mssim[0];
    }

    
    double ssim = (6.0*y_ssim + u_ssim + v_ssim)/8.0;
    cout << "ssim=" << ssim << endl;

    // free up resources
    delete [] buf_Y;
    delete [] buf_Y2;
    delete [] buf_U;
    delete [] buf_U2;
    delete [] buf_V;
    delete [] buf_V2;
    

    return 0;
    
}

