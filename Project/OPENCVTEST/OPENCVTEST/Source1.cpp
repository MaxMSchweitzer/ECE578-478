#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2\opencv.hpp>
#include <math.h>

using namespace std;
using namespace cv;
int camera_number = 0;
int object_found = 0;
int robot_standing = 0;

//threshold values for hsv
int H_MIN_red = 255;//0;
int H_MAX_red = 255;//20;
int S_MIN_red = 255;//155;
int S_MAX_red = 255;
int V_MIN_red = 255;//50;
int V_MAX_red = 255;//200;

int H_MIN_blue = 255;//100;
int H_MAX_blue = 255;//180;
int S_MIN_blue = 255;//100;
int S_MAX_blue = 255;//255;
int V_MIN_blue = 255;//50;
int V_MAX_blue = 255;//200;

int H_MIN_green = 255;//50;
int H_MAX_green = 255;//90;
int S_MIN_green = 255;//100;
int S_MAX_green = 255;//255;
int V_MIN_green = 255;//50;
int V_MAX_green = 255;//200;

int H_MIN_pink = 190;
int H_MAX_pink = 210;
int S_MIN_pink = 155;
int S_MAX_pink = 255;
int V_MIN_pink = 50;
int V_MAX_pink = 200;

int Y_MAX_robot_moment = 200;

void my_trackbars(int, void*) {// called when trackbars are instantiated
}

/* Creates the sliders to change variables during run time
 * Commented out sections are for colors not used in this project.
 * Simply uncomment all the sections with the <COLOR>: tag around them.
 * Ex. RED: or BLUE:
 */
void createTrackbars() {
	
	/* RED:
	//create window for trackbars
	//namedWindow("trackbars red", WINDOW_NORMAL);
	//resizeWindow("trackbars red", 300, 300);
	//create memory to store trackbar name on window
	char TrackbarNamered[50];

	//create trackbars and insert them into window        
	createTrackbar("H_MIN", "trackbars red", &H_MIN_red, 255, my_trackbars);
	createTrackbar("H_MAX", "trackbars red", &H_MAX_red, 255, my_trackbars);
	createTrackbar("S_MIN", "trackbars red", &S_MIN_red, 255, my_trackbars);
	createTrackbar("S_MAX", "trackbars red", &S_MAX_red, 255, my_trackbars);
	createTrackbar("V_MIN", "trackbars red", &V_MIN_red, 255, my_trackbars);
	createTrackbar("V_MAX", "trackbars red", &V_MAX_red, 255, my_trackbars);
	*/

	/* BLUE:
	//create window for trackbars
	//namedWindow("trackbars blue", WINDOW_AUTOSIZE);
	//create memory to store trackbar name on window
	char TrackbarNameblue[50];

	//create trackbars and insert them into window        
	createTrackbar("H_MIN", "trackbars blue", &H_MIN_blue, 255, my_trackbars);
	createTrackbar("H_MAX", "trackbars blue", &H_MAX_blue, 255, my_trackbars);
	createTrackbar("S_MIN", "trackbars blue", &S_MIN_blue, 255, my_trackbars);
	createTrackbar("S_MAX", "trackbars blue", &S_MAX_blue, 255, my_trackbars);
	createTrackbar("V_MIN", "trackbars blue", &V_MIN_blue, 255, my_trackbars);
	createTrackbar("V_MAX", "trackbars blue", &V_MAX_blue, 255, my_trackbars);
	*/

	//create window for trackbars
	namedWindow("trackbars green", WINDOW_AUTOSIZE);
	//create memory to store trackbar name on window
	char TrackbarNamegreen[50];

	//namedWindow("Robot stance", WINDOW_AUTOSIZE);
	//char TrackBarNameRobot[5];

	//create trackbars and insert them into window        
	createTrackbar("H_MIN", "trackbars green", &H_MIN_green, 255, my_trackbars);
	createTrackbar("H_MAX", "trackbars green", &H_MAX_green, 255, my_trackbars);
	createTrackbar("S_MIN", "trackbars green", &S_MIN_green, 255, my_trackbars);
	createTrackbar("S_MAX", "trackbars green", &S_MAX_green, 255, my_trackbars);
	createTrackbar("V_MIN", "trackbars green", &V_MIN_green, 255, my_trackbars);
	createTrackbar("V_MAX", "trackbars green", &V_MAX_green, 255, my_trackbars);

	/* PINK:
	//create window for trackbars
	//namedWindow("Obstacles", WINDOW_AUTOSIZE);
	//create memory to store trackbar name on window
	//char TrackbarNamepink[50];

	//create trackbars and insert them into window        
	createTrackbar("H_MIN", "Obstacles", &H_MIN_pink, 255, my_trackbars);
	createTrackbar("H_MAX", "Obstacles", &H_MAX_pink, 255, my_trackbars);
	createTrackbar("S_MIN", "Obstacles", &S_MIN_pink, 255, my_trackbars);
	createTrackbar("S_MAX", "Obstacles", &S_MAX_pink, 255, my_trackbars);
	createTrackbar("V_MIN", "Obstacles", &V_MIN_pink, 255, my_trackbars);
	createTrackbar("V_MAX", "Obstacles", &V_MAX_pink, 255, my_trackbars);
	*/

	namedWindow("robot_stance", WINDOW_AUTOSIZE);
	createTrackbar("Y_MAX", "robot_stance", &Y_MAX_robot_moment, 300, my_trackbars);
}

/* This function can track objects not based off the color, but off of the actual contour of the object.
 * It is unused for this particular porject, but can be used for future iterations.
 * i.e. To track the robots based off their shape, instead of their color.
 */
void track_object(int &color, int &x, int &y, Mat threshold, Mat &cameraFeed) {
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
			x = moment.m10 / area; //this is where the proram finds the x/y coordinates of the middle of the object
			y = moment.m01 / area; //this is where the proram finds the x/y coordinates of the middle of the object
			objectFound = true;
			refArea = area;
		}
		//let user know you found an object
		if (objectFound == true) {
			putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
			//draw object location on screen			
			circle(cameraFeed, Point(x, y), 10, Scalar(0, 255, 0), 2);
			//pathfinding subroutine?
			object_found = 1;
			//printf("Color %d position: y = %d, z = %d, \n", color, x ,y);
		}
	}
}

/* Tracks an object based off of its' color.
 * Used in this projectto track a robot with colored components (3d printed)
 */
void track_obstacle(int &color, int &x, int &y, Mat threshold, Mat &cameraFeed) {
	Mat threshold_output;
	threshold.copyTo(threshold_output);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point (0,0) );

	// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center( contours.size() );
	vector<float>radius(contours.size());
	int i;
	double refArea = 0;                                                                     
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();  //if this is big, we have a noisy image
		for (int index = 0; index >= 0; index = hierarchy[index][0]) {
			Moments moment = moments((cv::Mat)contours[index]);
			for (i = 0; i < contours.size(); i++) {
				approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
				boundRect[i] = boundingRect(Mat(contours_poly[i]));
				minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
			}
			double area = moment.m00; //area
			x = moment.m10 / area; //this is where the proram finds the x/y coordinates of the middle of the object
			y = moment.m01 / area; //this is where the proram finds the x/y coordinates of the middle of the object
			//int w = boundRect.width;
			//int h = boundRect.height;
			objectFound = true;
			refArea = area; 
		}
		//let user know you found an object
		if (objectFound == true) {
			for (i = 0; i < contours.size(); i++) {
				rectangle(cameraFeed, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 2, 8, 0); //tl == top left, br == bottom right
				circle(cameraFeed, center[i], (int)radius[i], Scalar(255, 0, 0), 2, 8, 0);
			}
			putText(cameraFeed, "Tracking Obstacle", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
			circle(cameraFeed, Point(x, y), 10, Scalar(255, 0, 0), 2);
			object_found = 1;
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
	Mat text = Mat::zeros(300, 300, CV_8UC3);


	char robot_posture[50] = "";
	int color = 0;
	createTrackbars();
	VideoCapture capture;
	capture.open(camera_number);

	CvFont font;
	double hScale = 1.0;
	double vScale = 1.0;
	int    lineWidth = 1;
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);

	int first_loop = 0;  //if the first loop, position the windows, don't do it over and over or I will not be able to drag windows around
	while (1) {
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



		/* BLUE:
		inRange(HSV, Scalar(H_MIN_blue, S_MIN_blue, V_MIN_blue), Scalar(H_MAX_blue, S_MAX_blue, V_MAX_blue), blue_threshold);
		//erode and dilate to clean up the image
		Mat erodeElement_blue = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat dilateElement_blue = getStructuringElement(MORPH_RECT, Size(8, 8));
		erode(blue_threshold, blue_threshold, erodeElement);
		dilate(blue_threshold, blue_threshold, dilateElement);
		//track the object
		color = 2;
		track_object(color, bx, by, blue_threshold, cameraFeed);
		*/
		/* RED:
		inRange(HSV, Scalar(H_MIN_red, S_MIN_red, V_MIN_red), Scalar(H_MAX_red, S_MAX_red, V_MAX_red), red_threshold);
		//erode and dilate to clean up the image
		Mat erodeElement_red = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat dilateElement_red = getStructuringElement(MORPH_RECT, Size(8, 8));
		erode(red_threshold, red_threshold, erodeElement);
		dilate(red_threshold, red_threshold, dilateElement);
		//track the object
		color = 2;
		track_object(color, rx, ry, red_threshold, cameraFeed);
		*/
		/* PINK:
		inRange(HSV, Scalar(H_MIN_pink, S_MIN_pink, V_MIN_pink), Scalar(H_MAX_pink, S_MAX_pink, V_MAX_pink), pink_threshold);
		//erode and dilate to clean up the image
		Mat erodeElement_pink = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat dilateElement_pink = getStructuringElement(MORPH_RECT, Size(8, 8));
		erode(pink_threshold, pink_threshold, erodeElement);
		dilate(pink_threshold, pink_threshold, dilateElement);
		//blur(HSV, HSV, Size(3, 3));
		//track the object
		color = 2;
		track_obstacle(color, px, py, pink_threshold, cameraFeed);
		*/
		//get distance beween points
		
		/* This code is used for pathfinding, which is not used in this program.
		 * It is left in for potential future extension however.
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
		}
		else if (a != 0 && b == 0) { // on y-axis
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
		}*/
		/*
		if (b != 0)
		angle_gp = atan(a / b);
		angle_gp = (angle_gp * 180) / 3.141592654;
		*/

		/*float angle_gr = 0;
		if (d != 0)
			angle_gr = atan(c / d);
		angle_gr = (angle_gr * 180) / 3.141592654;
		*/
		//cout << "\n The hypotenuse length is: " << a << " " << b << " " << (result);
		//printf("GRN:(%d,%d)\tPNK:(%d,%d)\tRED:(%d,%d)\tBLU:(%d,%d)\tG-P:(%d,%d)\tAngGP:%.1f%c\tDstGP:%.1f\tG-P:(%d,%d)\tAngGR:%.1f%c\tDstGR:%.1f\n", gx, gy, px, py, rx, ry, bx, by, gx - px, gy - py, angle_gp, 248, dist_gp, rx - bx, ry - by, angle_gr, 248, dist_gr); //subtrace pink coordinates from pink, print the delta
		
		ofstream my_file;
		//write a file with motor commands
		if (object_found == 1) {
			try {
				my_file.open("../../cat_robot_posture.txt");
				if (!my_file.is_open())
					throw 1;

				// Use greater than because coordinates start at 0,0 in upper left hand corner
				// so lower bounds of the image are a larger y coordinate value
				if (gy > Y_MAX_robot_moment)
				{
					// Our robot is laying down, as it's moment is at the bottom of the image.
					sprintf_s(robot_posture, "Robot is: %s", "laying down");
					robot_standing = 0;
				}
				else
				{
					sprintf_s(robot_posture, "Robot is: %s", "standing up");
					robot_standing = 1;
				}
				my_file << robot_standing << "\n";
			}
			catch (int e)
			{
			}
			my_file.close();
		}
		else
			text.setTo(cv::Scalar(0, 0, 0));

		// Display the robot posture string and the coordinates of the moment
		if (robot_standing == 1)
		{
			text.setTo(cv::Scalar(0, 0, 0));
			putText(text, robot_posture, cvPoint(10, 20), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 255, 0), 1, CV_AA);
		}
		else
		{
			text.setTo(cv::Scalar(0, 0, 0));
			putText(text, robot_posture, cvPoint(10, 40), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 255, 0), 1, CV_AA);
		}

		char coordinates[255];
		sprintf_s(coordinates, "x,y %d,%d", (int)gx, (int)gy);

		putText(text, coordinates, cvPoint(10, 60), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255, 255, 0), 1, CV_AA);
		//open all windows
		imshow("camera_feed", cameraFeed);
		imshow("green_threshold", green_threshold);
		imshow("robot_stance", text);
		//imshow("pink_threshold", pink_threshold); PINK:
		//imshow("red_threshold", red_threshold); RED:
		//imshow("blue_threshold", blue_threshold); BLUE:
		if (first_loop == 0) {
			first_loop = 1;
			moveWindow("camera_feed", 600, 330);
			moveWindow("green_threshold", 0, 0);
			//moveWindow("pink_threshold", 430, 0); PINK:
			//moveWindow("red_threshold", 860, 0); RED:
			//moveWindow("blue_threshold", 1290, 0); BLUE:
			resizeWindow("trackbars green", 300, 200);
			//resizeWindow("trackbars pink", 300, 200); PINK:
			//resizeWindow("trackbars red", 300, 200); RED:
			//resizeWindow("trackbars blue", 300, 200); BLUE:
			moveWindow("trackbars green", 0, 500);
			//moveWindow("trackbars pink", 430, 500); PINK:
			//moveWindow("trackbars red", 860, 500); RED:
			//moveWindow("trackbars blue", 1290, 500); BLUE:

		}

		waitKey(40);
	}
}
