#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

//This is not very clean code.
//I would like to clean it up at some point.

//If defined the program will use target.jpg, if commented out it will use the camera
#define USE_IMAGE

//Defaults/Start value
static const int H_MIN = 0;
static const int H_MAX = 179;
static const int S_MIN = 0; 
static const int S_MAX = 255;
static const int V_MIN = 0;
static const int V_MAX = 255;

#ifdef USE_IMAGE
//If we are using a large image, instead of running inRange all the time, we can run it when we change values
static int PAST_H_Min = H_MIN-1;
static int PAST_H_Max = H_MAX-1;
static int PAST_S_Min = S_MIN-1; 
static int PAST_S_Max = S_MAX-1;
static int PAST_V_Min = V_MIN-1;
static int PAST_V_Max = V_MAX-1;
#endif

//We have to give createTrackbar a callback function, but we don't actually need it
void trackBar(int value, void*){}

//Mouse callback function (prints BGR value of original image)
void onMouse( int event, int x, int y, int, void*);

int main()
{
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE );// Create a window for display (transformed image)
	cv::namedWindow("Config window", cv::WINDOW_NORMAL);// Create a window for configuration (Trackbars)
	cv::namedWindow("Original window",cv::WINDOW_AUTOSIZE ); // Create a window for orginal image
	
	//We need a pointer to the H min/max, S min/max, and V min/max

	int* hueMin;
	int intHueMin = H_MIN;
	hueMin = &intHueMin;

	int* hueMax;
	int intHueMax = H_MAX;
	hueMax = &intHueMax;

	int* satMin;
	int intSatMin = S_MIN;
	satMin = &intSatMin;

	int* satMax;
	int intSatMax = S_MAX;
	satMax = &intSatMax;

	int* valMin;
	int intValMin = V_MIN;
	valMin = &intValMin;

	int* valMax;
	int intValMax = V_MAX;
	valMax = &intValMax;

	//Create the trackbars that are used to get user input	
	cv::createTrackbar("hueMin","Config window",hueMin,179,trackBar);
	cv::createTrackbar("hueMax","Config window",hueMax,179,trackBar);
	cv::createTrackbar("satMin","Config window",satMin,255,trackBar);
	cv::createTrackbar("satMax","Config window",satMax,255,trackBar);
	cv::createTrackbar("valMin","Config window",valMin,255,trackBar);
	cv::createTrackbar("valMax","Config window",valMax,255,trackBar);

	cv::Mat image;
	cv::Mat output;

	#ifndef USE_IMAGE
	//Open and make sure the camera works
	cv::VideoCapture camera (0);
	if(!camera.isOpened()){
		std::cout<<"Could not open camera\n";
		return -1;
	}
	#endif

	#ifdef USE_IMAGE
	//Read the image, we assume it's there
	image = cv::imread("image.jpg");
	cv::imshow("Original window",image);
	//Change the colorspace from Blue green red (camera) to hue saturation value
	//Only needs to happen once, if image is static
	cv::cvtColor(image,image,cv::COLOR_BGR2HSV);
	//On mouse click print pixel values of image
	//We could use this everywhere, but it would be slightly harder to do,
	//and it's not useful to me.
	cv::setMouseCallback("Original window", onMouse, &image);
	#endif
	
	while(true){
		#ifndef USE_IMAGE
		//Send an image from the camera every frame
		camera>>image;
		//Show the orginal image from the camera every frame
		cv::imshow("Orginal Image",image);
		#endif
		
		#ifdef USE_IMAGE
		//Make sure we have new values if we are working on the static image
		if(PAST_H_Min == *hueMin && PAST_H_Max == *hueMax
		&& PAST_S_Min == *satMin && PAST_S_Max == *satMax
		&& PAST_V_Min == *valMin && PAST_V_Max == *valMax){
			cv::waitKey(1); //Wait for one ms
		}else{	
			PAST_H_Min = *hueMin;
			PAST_H_Max = *hueMax;
			PAST_S_Min = *satMin;
			PAST_S_Max = *satMax;
			PAST_V_Min = *valMin;
			PAST_V_Max = *valMax;
		#endif
			#ifndef USE_IMAGE
			//Change the colorspace from Blue green red (camera) to hue saturation value
			cv::cvtColor(image,image,cv::COLOR_BGR2HSV);
			#endif
			//Run inRange on our image with the values given by the trackbar, and send the output to output
			cv::inRange(image, cv::Scalar(*hueMin,*satMin,*valMin), cv::Scalar(*hueMax,*satMax,*valMax), output);

			cv::imshow("Display window", output); // Show the output image
			cv::waitKey(1); //Wait for one ms

		#ifdef USE_IMAGE
		}
		#endif
	}
}

void onMouse( int event, int x, int y, int, void* mat){
	//Mouse clicks
	if(event == 1){
		//If mouse moves outside image, x or y could be -1
		if(x < 0 || y < 0)
			return;

		cv::Mat image = *(cv::Mat*)(mat);
		//Get BGR pixel from original image
		cv::Vec3b pixel = image.at<cv::Vec3b>(x, y);
		//Printf seemed cleaner for this
		//Position : BGR value
		printf("(%d, %d) : %d %d %d\n", x, y, pixel[0], pixel[1], pixel[2]);
	}
}
