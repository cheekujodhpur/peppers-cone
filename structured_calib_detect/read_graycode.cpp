// Written by: Kumar Ayush <cheekujodhpur@gmail.com>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    VideoCapture vc;
    Mat frame;

    if ( argc < 2 ) {
        cout << "Please enter file name" << endl;
        return -1;
    }

    vc.open(argv[1]);
    if ( !vc.isOpened() ) {
        cout << "Video not opened" << endl;
        return -1;
    }
    namedWindow("Display Raw", WINDOW_NORMAL);
    namedWindow("Display Proc", WINDOW_NORMAL);

    Mat frame_flipped, frame_gray, thresh;
    Mat cropped_frame;
    Mat prev_thresh;

    vector<Mat> keyframes, keyframes_raw;
    
    // To set size
    vc.read(frame);
    
    // NOTE: These values are determined by manual inspection
    // NOTE: Also assumes 640x480 original graycode video
    int x_c = 0;
    int y_c = 0;
    // Region of Interest
    Rect ROI(x_c, y_c, frame.cols, frame.rows);

    int width = frame.cols;
    int height = frame.rows;
    cout << width << " x " << height << endl;

    int thresh_bar = 100;
    // To set thresh values
    cropped_frame = Mat(frame, ROI);
    //flip(cropped_frame, frame_flipped, 0);
    cvtColor(cropped_frame, frame_gray, COLOR_RGB2GRAY); 
    threshold(frame_gray, thresh, thresh_bar, 255, THRESH_BINARY);

    cout << "Reading all frames..." << endl;

    int select_count = 0;
    do {

        cropped_frame = Mat(frame, ROI);
        imshow("Display Raw", cropped_frame);

        //flip(cropped_frame, frame_flipped, 0);
        cvtColor(cropped_frame, frame_gray, COLOR_RGB2GRAY); 

        prev_thresh = thresh.clone();
        threshold(frame_gray, thresh, thresh_bar, 255, THRESH_BINARY);

        imshow("Display Proc", thresh);

        cout << "Keep this frame [y/n] " << flush;
        int key = waitKey(0);
        if (key == 'y') {
            select_count++;
            keyframes.push_back(thresh.clone());
            cout << select_count << " selected" << endl;
        }  
        cout << endl;

        // quit on q
        if ( (key & 0xFF) == 'q' )
            break;
    } while ( vc.read(frame) );

    vc.release();

    cout << "Displaying selected frames" << endl;
    for (int frame_iter = 0;frame_iter < keyframes.size();frame_iter++)
    {
        imshow("Display Proc", keyframes[frame_iter]);
        waitKey(500);
    }

    cout << "Confirm to write [y/n]" << flush;
    int key_ =  waitKey(0);
    if (key_ == 'y') {

        ofstream map_file;
        map_file.open("inverse_map.csv", ofstream::out);

        for(int i = 0;i<width*height;i++) {
            string s = "";
            int j = (int)i/width;
            int k = (int)i%width;
            for (int frame_iter = 0;frame_iter < keyframes.size();frame_iter++)
            {
                    if (keyframes[frame_iter].at<uchar>(j,k) > 127)
                        s = s + "1";
                    else 
                        s = s + "0";
            }
            map_file << s << ",";
            map_file << j << ",";
            map_file << k << endl;
        }

        map_file.close();
    }

    destroyAllWindows();
    return 0;
}
