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
    Mat frame, prev_frame;

    if ( argc < 2 ) {
        cout << "Please enter file name" << endl;
        return -1;
    }

    vc.open(argv[1]);
    if ( !vc.isOpened() ) {
        cout << "Video not opened" << endl;
        return -1;
    }
    namedWindow("Display Raw");
    namedWindow("Display Proc");

    Mat frame_flipped, frame_gray, thresh;

    // To set size
    vc.read(frame);
    prev_frame = frame.clone();
    
    vector<int> data[frame.cols*frame.rows];
    vector<Mat> keyframes;

    cout << "Reading all frames..." << endl;

    while ( vc.read(frame) ) {
        imshow("Display Raw", frame);


        flip(frame, frame_flipped, 0);
        cvtColor(frame_flipped, frame_gray, COLOR_RGB2GRAY); 
        threshold(frame_gray, thresh, 127, 255, THRESH_BINARY);

        imshow("Display Proc", thresh);


        double diff = norm(frame, prev_frame, NORM_L2)/(frame.rows*frame.cols);
        if(diff > 0.01) {
            keyframes.push_back(thresh.clone());
            for (int i = 0;i < thresh.rows; ++i)
                for (int j = 0; j < thresh.cols; ++j)
                    data[i*thresh.cols + j].push_back(thresh.at<int>(i,j));
        }
        prev_frame = frame.clone();

        int key = waitKey(1);

        // quit on q
        if ( (key & 0xFF) == 'q' )
            break;
    }

    vc.release();

    destroyWindow("Display Raw");
    cout << "Displaying " << keyframes.size() << " keyframes..." << endl;
    vector<Mat> keyframes_filtered;

    for(int i = 0;i<keyframes.size();++i) {
        imshow("Display Proc", keyframes[i]);
        cout << "Keep this frame [y/n] ";
        int key = waitKey(0);
        if (key == 'y') {
            keyframes_filtered.push_back(keyframes[i]);
        }  
        cout << endl;
    }

    cout << "Displaying final keyframes..." << endl;
    for(int i = 0;i<keyframes_filtered.size();i++) {
        imshow("Display Proc", keyframes_filtered[i]);
        int key = waitKey(1000);
    }

    destroyAllWindows();
    return 0;
}
