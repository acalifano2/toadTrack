//
//
// Author: Armando Califano
// Purpose:	To segment background, reducing as much noise as possible
//			to locate moving toads. Toads are defined by a specific amount of pixels
//
//

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/bgsegm.hpp"

using namespace std;
using namespace cv;
using namespace cv::bgsegm;

#define MAXFRAMES 1000
#define MINAREA   10
#define MAXAREA   1000

Mat frames;
Mat fgErode, binary;
Mat fgMaskMOG;
Ptr<BackgroundSubtractor> pMOG;

void processImages(char* filePath);
void blurImage(Mat fgMaskMOG);
void thresholdImage(Mat fgMaskMOG);

int main(int argc, char* argv[])
{
    //Input only requires two arguments
    if(argc != 2) {
		cerr <<"ERROR: Invalid input" << endl;
		cerr <<"Correct Input: ./executable {file_path}" << endl;
		return -1;
    }

    //Create windows to display processed images
    namedWindow("DepthFrames", CV_WINDOW_NORMAL);
	moveWindow("DepthFrames", 0, 0);
    namedWindow("Foreground Mask MOG");
	moveWindow("Foreground Mask MOG", 580, 0);
    namedWindow("Thresholded Image");
	moveWindow("Thresholded Image", 1100, 0);

    //Initialize BackgroundSubtractor object
    pMOG = createBackgroundSubtractorMOG();

    //Run function from input to process images
    processImages(argv[1]);

    //Destroys windows
    destroyAllWindows();

    return 0;
}


//Reads input string loading frames into a Mat structure. A foreground
//mask is taken and the background is subtracted using the MOG method.
void processImages(char* filePath){
    string file(filePath);

    //Iterate through all frames in a sequence
    for( int i=0; i<MAXFRAMES; i++){
		//Create strings to parse the file pathname
		size_t delimiter1 = file.find_last_of("/");
		size_t delimiter2 = file.find_last_of(".");
		string path = file.substr(0,delimiter1+11);
		string extension = file.substr(delimiter2);
		string frameNumberString = file.substr(delimiter1+11,
									delimiter2-delimiter1-11);
		//Replace framNumberString with an integer
		int frameNumberInt = i;
		ostringstream oss;
		//Convert int back to string
		oss << (frameNumberInt);
		frameNumberString = oss.str();
		//Put full file path together
		string nextFile = path + frameNumberString + extension;
		cout << nextFile << endl;
		file.assign(nextFile);

		//load files into Mat structure
		frames = imread(file);
		if(!frames.data){
	    	cerr << "ERROR: Unable to load file" << endl;
	    	exit(-1);
	}

	//Apply background subtractor
	pMOG->apply(frames, fgMaskMOG);
	//Further process frames
	blurImage(fgMaskMOG);
	thresholdImage(fgMaskMOG);

	//Display frames
	imshow("DepthFrames", frames);
	imshow("Foreground Mask MOG", fgMaskMOG);
	imshow("Thresholded Image", binary);

	//Wait for user input to display next frame
	waitKey();
    }
}

//Function to reduce noise in the foreground mask
void blurImage(Mat fgMaskMOG){
    erode(fgMaskMOG, fgErode, NULL);
    medianBlur(fgErode, fgMaskMOG, 3);
}

//Function to threshold the image to binary and identify objects of a 
//certain size
void thresholdImage(Mat fgMaskMOG){
    //Converts pixels to binary
    threshold(fgMaskMOG, binary, 180, 255, THRESH_BINARY);

    //Create vectors for parameters of findContours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    //Identitfy object based on amount of pixels
    findContours( binary, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE );
    cout<< "Number of contours " << contours.size() << endl;

    for (int i = 0; i< contours.size(); i++) {
		//Create a bounding box centered on the targeted object
		Rect box = boundingRect(contours[i]);
		int area = contourArea(contours[i], false);
		if (area>MINAREA && area<MAXAREA) {
	    	cout << "Target " << i << ":" << area << endl;
	    	cout << box.x <<" "<< box.y <<" "<< box.width <<" "<< box.height<< endl;
	    	rectangle(binary, Point(box.x-10, box.y-10), Point(box.x+box.width+10,box.y+box.height+10), Scalar(180,180,180,180), 1, 8, 0);
		}
    }
}
