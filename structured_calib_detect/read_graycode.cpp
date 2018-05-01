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
    namedWindow("Display Raw");
    namedWindow("Display Proc");

    Mat frame_flipped, frame_gray, thresh;
    Mat cropped_frame;
    Mat prev_thresh;

    vector<Mat> keyframes, keyframes_raw;
    
    // To set size
    vc.read(frame);
    
    // NOTE: These values are determined by manual inspection
    // NOTE: Also assumes 640x480 original graycode video
    int x_c = 150;
    int y_c = 150;
    // Region of Interest
    Rect ROI(x_c, y_c, 320, 240);

    int width = 320;
    int height = 240;
    cout << width << " x " << height << endl;
    vector<int> data[width*height];

    int thresh_bar = 200;
    // To set thresh values
    cropped_frame = Mat(frame, ROI);
    flip(cropped_frame, frame_flipped, 0);
    cvtColor(frame_flipped, frame_gray, COLOR_RGB2GRAY); 
    threshold(frame_gray, thresh, thresh_bar, 255, THRESH_BINARY);

    cout << "Reading all frames..." << endl;

    do {

        cropped_frame = Mat(frame, ROI);
        imshow("Display Raw", cropped_frame);

        flip(cropped_frame, frame_flipped, 0);
        cvtColor(frame_flipped, frame_gray, COLOR_RGB2GRAY); 

        prev_thresh = thresh.clone();
        threshold(frame_gray, thresh, thresh_bar, 255, THRESH_BINARY);

        imshow("Display Proc", thresh);

        //double diff = norm(thresh, prev_thresh, NORM_L2)/
        //                  (width*height);

        // NOTE: This threshold is also set by manual inspection
        //if(diff > 0.02) {
            keyframes_raw.push_back(frame.clone());
            keyframes.push_back(thresh.clone());
        //}

        int key = waitKey(1);

        // quit on q
        if ( (key & 0xFF) == 'q' )
            break;
    } while ( vc.read(frame) );

    vc.release();

    cout << "Displaying " << keyframes.size() << " keyframes..." << endl;
    vector<Mat> keyframes_filtered,
                keyframes_filtered_raw;

    while (1) {
        int select_count = 0;
        keyframes_filtered.clear();
        keyframes_filtered_raw.clear();
        bool found = false;
        for(int i = 0;i<keyframes.size();++i) {
            imshow("Display Raw", keyframes_raw[i]);
            imshow("Display Proc", keyframes[i]);
            cout << "Keep this frame [y/n] " << flush;
            int key = waitKey(0);
            if (key == 'y') {
                select_count++;
                keyframes_filtered.push_back(keyframes[i]);
                keyframes_filtered_raw.push_back(keyframes_raw[i]);
                cout << select_count << " selected" << endl;
            }  
            else if (key == 'r') break;
            else if (key == 'q') {found = true; break;}
            cout << endl;
        }
        if(found) break;
    }

    cout << "Displaying " << keyframes_filtered.size() << " final keyframes..."
         << endl;
    for(int i = 0;i<keyframes_filtered.size();i++) {
        imshow("Display Raw", keyframes_filtered_raw[i]);
        imshow("Display Proc", keyframes_filtered[i]);
        for (int j = 0;j < height; ++j)
            for (int k = 0; k < width; ++k)
                data[j*width + k].push_back(keyframes_filtered[i].at<int>(j,k));
        int key = waitKey(1000);
    }

    cout << "Confirm to write [y/n]" << flush;
    int key_ =  waitKey(0);
    if (key_ == 'y') {

        ofstream map_file;
        map_file.open("inverse_map.csv", ofstream::out);

        for(int i = 0;i<width*height;i++){
            vector<int> tmp = data[i];
            string s = "";
            for(int j = 0;j<tmp.size();j++){
                if (tmp[j] > 127) s = s+"1";
                else s = s + "0";
            }
            map_file << s << ",";
            map_file << (int)i/width << ",";
            map_file << (int)i%width << endl;
        }

        map_file.close();
    }

    destroyAllWindows();
    return 0;
}
