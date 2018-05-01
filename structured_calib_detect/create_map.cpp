// Written by: Kumar Ayush <cheekujodhpur@gmail.com>

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
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
    namedWindow("Display");

    Mat frame_gray;
    cout << "Reading all frames..." << endl;
    int count = 0;

    vc.read(frame);
    int width, height;
    width = frame.cols;
    height = frame.rows;
    vector<int> data[width*height];

    do {

        count = count+1;

        cvtColor(frame, frame_gray, COLOR_RGB2GRAY); 
        imshow("Display", frame_gray);

        for (int i = 0;i < frame_gray.rows; ++i)
            for (int j = 0; j < frame_gray.cols; ++j)
                data[i*frame_gray.cols + j].push_back(frame_gray.at<uchar>(i,j));

        int key = waitKey(1);

        // quit on q
        if ( (key & 0xFF) == 'q' )
            break;
    } while ( vc.read(frame) );
    cout << count << " frames read..." << endl;

    vc.release();
    destroyAllWindows();

    ofstream map_file;
    map_file.open("direct_map.csv", ofstream::out);

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
    return 0;
}
