// Modified from: https://docs.opencv.org/3.1.0/db/d56/tutorial_capture_graycode_pattern.html
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/structured_light.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

static const char* keys =
{         "{@proj_width      | | Projector width            }"
          "{@proj_height     | | Projector height           }" };

int main( int argc, char** argv )
{
    structured_light::GrayCodePattern::Params params;
    CommandLineParser parser( argc, argv, keys );
    params.width = parser.get<int>( 0 );
    params.height = parser.get<int>( 1 );

    if( params.width < 1 || params.height < 1 )
    {
        return -1;
    }

    // Set up GraycodePattern with params
    Ptr<structured_light::GrayCodePattern> graycode = structured_light::GrayCodePattern::create( params );
    // Storage for pattern
    vector<Mat> pattern;
    graycode->generate( pattern );

    // Generate the all-white and all-black images needed for shadows mask computation
    Mat white;
    Mat black;
    graycode->getImagesForShadowMasks( black, white );

    // namedWindow( "Pattern Window", WINDOW_NORMAL );
    int i = 0;
    Mat tmp_w;
    Mat tmp_i;
    while( i < (int) pattern.size() )
    {
        /// Display code commented out
        // imshow( "Pattern Window", pattern[i] );
        char fname[80];
        sprintf(fname, "graycode_%d.BMP", i);
        
        cvtColor(pattern[i], tmp_w, COLOR_GRAY2RGB);
        tmp_w.setTo(Scalar(255,0,255), pattern[i]);
        bitwise_not(pattern[i], tmp_i);
        tmp_w.setTo(Scalar(0,255,0), tmp_i); 
        imwrite(fname, tmp_w);
        // int key = waitKey( 0 );
        // if ( key ==  13 ) {
        i = i + 1;
        // }
        // else if (key == 27) {
        //     break;
        // }
    }

    return 0;
}
