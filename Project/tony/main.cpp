#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
//#include <opencv2\opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;
int camera_number = 0;
int object_found = 0;

//threshold values for hsv
int H_MIN_red = 0;
int H_MAX_red = 20;
int S_MIN_red = 155;
int S_MAX_red = 255;
int V_MIN_red = 50;
int V_MAX_red = 200;

int H_MIN_blue = 100;
int H_MAX_blue = 180;
int S_MIN_blue = 100;
int S_MAX_blue = 255;
int V_MIN_blue = 50;
int V_MAX_blue = 200;

int H_MIN_green = 50;
int H_MAX_green = 90;
int S_MIN_green = 100;
int S_MAX_green = 255;
int V_MIN_green = 50;
int V_MAX_green = 200;

int H_MIN_pink = 190;
int H_MAX_pink = 210;
int S_MIN_pink = 155;
int S_MAX_pink = 255;
int V_MIN_pink = 50;
int V_MAX_pink = 200;

void my_trackbars(int, void*) {// called when trackbars are instantiated
}

void createTrackbars(){
	//create window for trackbars
	namedWindow("trackbars red", WINDOW_NORMAL);
	resizeWindow("trackbars red", 300, 300);
	//create memory to store trackbar name on window
	char TrackbarNamered[50];

	//create trackbars and insert them into window        
	createTrackbar("H_MIN", "trackbars red", &H_MIN_red, 255, my_trackbars);
	createTrackbar("H_MAX", "trackbars red", &H_MAX_red, 255, my_trackbars);
	createTrackbar("S_MIN", "trackbars red", &S_MIN_red, 255, my_trackbars);
	createTrackbar("S_MAX", "trackbars red", &S_MAX_red, 255, my_trackbars);
	createTrackbar("V_MIN", "trackbars red", &V_MIN_red, 255, my_trackbars);
	createTrackbar("V_MAX", "trackbars red", &V_MAX_red, 255, my_trackbars);

	//create window for trackbars
	namedWindow("trackbars blue", WINDOW_AUTOSIZE);
	//create memory to store trackbar name on window
	char TrackbarNameblue[50];

	//create trackbars and insert them into window        
	createTrackbar("H_MIN", "trackbars blue", &H_MIN_blue, 255, my_trackbars);
	createTrackbar("H_MAX", "trackbars blue", &H_MAX_blue, 255, my_trackbars);
	createTrackbar("S_MIN", "trackbars blue", &S_MIN_blue, 255, my_trackbars);
	createTrackbar("S_MAX", "trackbars blue", &S_MAX_blue, 255, my_trackbars);
	createTrackbar("V_MIN", "trackbars blue", &V_MIN_blue, 255, my_trackbars);
	createTrackbar("V_MAX", "trackbars blue", &V_MAX_blue, 255, my_trackbars);


	//create window for trackbars
	namedWindow("trackbars green", WINDOW_AUTOSIZE);
	//create memory to store trackbar name on window
	char TrackbarNamegreen[50];

	//create trackbars and insert them into window        
	createTrackbar("H_MIN", "trackbars green", &H_MIN_green, 255, my_trackbars);
	createTrackbar("H_MAX", "trackbars green", &H_MAX_green, 255, my_trackbars);
	createTrackbar("S_MIN", "trackbars green", &S_MIN_green, 255, my_trackbars);
	createTrackbar("S_MAX", "trackbars green", &S_MAX_green, 255, my_trackbars);
	createTrackbar("V_MIN", "trackbars green", &V_MIN_green, 255, my_trackbars);
	createTrackbar("V_MAX", "trackbars green", &V_MAX_green, 255, my_trackbars);

	//create window for trackbars
	namedWindow("trackbars pink", WINDOW_AUTOSIZE);
	//create memory to store trackbar name on window
	char TrackbarNamepink[50];

	//create trackbars and insert them into window        
	createTrackbar("H_MIN", "trackbars pink", &H_MIN_pink, 255, my_trackbars);
	createTrackbar("H_MAX", "trackbars pink", &H_MAX_pink, 255, my_trackbars);
	createTrackbar("S_MIN", "trackbars pink", &S_MIN_pink, 255, my_trackbars);
	createTrackbar("S_MAX", "trackbars pink", &S_MAX_pink, 255, my_trackbars);
	createTrackbar("V_MIN", "trackbars pink", &V_MIN_pink, 255, my_trackbars);
	createTrackbar("V_MAX", "trackbars pink", &V_MAX_pink, 255, my_trackbars);

}

void track_object(int &color, int &x, int &y, Mat threshold, Mat &cameraFeed){
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();  //if this is big, we have a noisy image
		for (int index = 0; index >= 0; index = hierarchy[index][0]) {
			Moments moment = moments((cv::Mat)contours[index]);
			double area = moment.m00; //area
			x = moment.m10 / area;
			y = moment.m01 / area;
			objectFound = true;
			refArea = area;
		}
		//let user know you found an object
		if (objectFound == true){
			putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
			//draw object location on screen			
			circle(cameraFeed, Point(x, y), 10, Scalar(0, 255, 0), 2);
			object_found = 1;
			//printf("Color %d position: y = %d, z = %d, \n", color, x ,y);
		}
	}
}

int main()
{
	Mat cameraFeed;
	Mat HSV;
	Mat pink_threshold;
	Mat green_threshold;
	Mat red_threshold;
	Mat blue_threshold;
	//x and y values for the location of the object

	int color = 0;
	createTrackbars();
	VideoCapture capture;
	capture.open(camera_number);

	int first_loop = 0;  //if the first loop, position the windows, don't do it over and over or I will not be able to drag windows around
	while (1){
		int rx = 0, ry = 0; //red x and y
		int gx = 0, gy = 0; //green x and y
		int bx = 0, by = 0; //blue x and y
		int px = 0, py = 0; //pink x and y
		capture.read(cameraFeed);
		resize(cameraFeed, cameraFeed, Size(400, 300));
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		//filter and store in threshold matrix

		inRange(HSV, Scalar(H_MIN_green, S_MIN_green, V_MIN_green), Scalar(H_MAX_green, S_MAX_green, V_MAX_green), green_threshold);
		//erode and dilate to clean up the image
		Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));
		erode(green_threshold, green_threshold, erodeElement);
		dilate(green_threshold, green_threshold, dilateElement);
		//track the object
		color = 1;
		track_object(color, gx, gy, green_threshold, cameraFeed);

		inRange(HSV, Scalar(H_MIN_pink, S_MIN_pink, V_MIN_pink), Scalar(H_MAX_pink, S_MAX_pink, V_MAX_pink), pink_threshold);
		//erode and dilate to clean up the image
		Mat erodeElement_pink = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat dilateElement_pink = getStructuringElement(MORPH_RECT, Size(8, 8));
		erode(pink_threshold, pink_threshold, erodeElement);
		dilate(pink_threshold, pink_threshold, dilateElement);
		//track the object
		color = 2;
		track_object(color, px, py, pink_threshold, cameraFeed);


		inRange(HSV, Scalar(H_MIN_blue, S_MIN_blue, V_MIN_blue), Scalar(H_MAX_blue, S_MAX_blue, V_MAX_blue), blue_threshold);
		//erode and dilate to clean up the image
		Mat erodeElement_blue = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat dilateElement_blue = getStructuringElement(MORPH_RECT, Size(8, 8));
		erode(blue_threshold, blue_threshold, erodeElement);
		dilate(blue_threshold, blue_threshold, dilateElement);
		//track the object
		color = 2;
		track_object(color, bx, by, blue_threshold, cameraFeed);

		inRange(HSV, Scalar(H_MIN_red, S_MIN_red, V_MIN_red), Scalar(H_MAX_red, S_MAX_red, V_MAX_red), red_threshold);
		//erode and dilate to clean up the image
		Mat erodeElement_red = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat dilateElement_red = getStructuringElement(MORPH_RECT, Size(8, 8));
		erode(red_threshold, red_threshold, erodeElement);
		dilate(red_threshold, red_threshold, dilateElement);
		//track the object
		color = 2;
		track_object(color, rx, ry, red_threshold, cameraFeed);

		//get distance beween points
		float a, b, dist_gp;
		a = gx - px;
		b = gy - py;
		dist_gp = sqrt((a*a) + (b*b));

		float c, d, dist_gr;
		c = gx - rx;
		d = gy - ry;
		dist_gr = sqrt((c*c) + (d*d));

		//get angle between points
		float angle_gp = 0;

		if (a == 0 && b != 0) { // on x-axis
			angle_gp = (b < 0) ? 180 : 0;
		} else if (a != 0 && b == 0) { // on y-axis
			angle_gp = (a < 0) ? -90 : 90;
		}
		else { // normal move
			if (b > 0) { // correction factor to scale with OpenCV headings
				angle_gp = 0;
			}
			else {
				angle_gp = (a > 0) ? 180 : -180;
			}
			//printf("b = %f\n", b);
			angle_gp += (atan(a / b) * 180.0) / 3.141592654;
			angle_gp -= (angle_gp > 180) ? 360 : 0;
			angle_gp += (angle_gp < -180) ? 360 : 0;
		}
		/*
		if (b != 0)
			angle_gp = atan(a / b);
		angle_gp = (angle_gp * 180) / 3.141592654;
		*/

		float angle_gr = 0;
		if (d != 0)
			angle_gr = atan(c / d);
		angle_gr = (angle_gr * 180) / 3.141592654;

		//cout << "\n The hypotenuse length is: " << a << " " << b << " " << (result);
		printf("GRN:(%d,%d)\tPNK:(%d,%d)\tRED:(%d,%d)\tBLU:(%d,%d)\tG-P:(%d,%d)\tAngGP:%.1f%c\tDstGP:%.1f\tG-P:(%d,%d)\tAngGR:%.1f%c\tDstGR:%.1f\n", gx, gy, px, py, rx, ry, bx, by, gx - px, gy - py, angle_gp, 248, dist_gp, rx - bx, ry - by, angle_gr, 248, dist_gr); //subtrace pink coordinates from pink, print the delta
		ofstream my_file;
		//write a file with motor commands
		if (object_found == 1){
			try{
				my_file.open("../../cat_tracking.txt");
				if (!my_file.is_open())
					throw 1;
				my_file << "greenX " << gx << "\n";
				my_file << "greenY " << gy << "\n";
				my_file << "pinkX " << px << "\n";
				my_file << "pinkY " << py << "\n";
				//my_file << "catAng ";  //y
				//my_file << angle_gp;
				//my_file << "\n";
				my_file << "dot1X ";
				my_file << bx;
				my_file << "\n";
				my_file << "dot1Y ";  //y
				my_file << by;
				my_file << "\n";
			}
			catch (int e)
			{			
			}
			my_file.close();
		}

		imshow("camera_feed", cameraFeed);
		imshow("green_threshold", green_threshold);
		imshow("pink_threshold", pink_threshold);
		imshow("red_threshold", red_threshold);
		imshow("blue_threshold", blue_threshold);
		if (first_loop == 0){
			first_loop = 1;
			moveWindow("camera_feed", 600, 330);
			moveWindow("green_threshold", 0, 0);
			moveWindow("pink_threshold", 430, 0);
			moveWindow("red_threshold", 860, 0);
			moveWindow("blue_threshold", 1290, 0);
			resizeWindow("trackbars green", 300, 200);
			resizeWindow("trackbars pink", 300, 200);
			resizeWindow("trackbars red", 300, 200);
			resizeWindow("trackbars blue", 300, 200);
			moveWindow("trackbars green", 0, 500);
			moveWindow("trackbars pink", 430, 500);
			moveWindow("trackbars red", 860, 500);
			moveWindow("trackbars blue", 1290, 500);

		}
		waitKey(40);
	}
}
