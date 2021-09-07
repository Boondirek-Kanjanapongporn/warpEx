/*
ID: 63011117
Name: Boondirek Kanjanapongporn
Date: 2/5/2021
*/
#include "ObjectImg.h"

int fileCheck(string path) {
	Mat imgTest = imread(path);
	if (imgTest.empty()) {
		return 0;
	}
	return 1;
}

string saveFile(Mat imgInput, string str) {
	string tempPath;
	time_t timestamp;
	time(&timestamp);
	if (str == "Save")
		tempPath = "Resources/Save/" + to_string(timestamp) + ".png";
	else if (str == "Temp")
		tempPath = "Resources/Temp/" + to_string(timestamp) + ".png";

	imwrite(tempPath, imgInput);
	cout << tempPath << endl;
	return tempPath;
}

void drawPoints(vector<Point> pointInput, Scalar colorInput, Mat imgInputOutput) {
	for (int i = 0; i < pointInput.size(); i++) {
		circle(imgInputOutput, pointInput[i], 10, colorInput, FILLED);
		// putText(imgInputOutput, to_string(i), pointInput[i], FONT_HERSHEY_PLAIN, 5, colorInput, 5);
	}
}

int OpenCVCamera(Mat &imgInput) {
	VideoCapture capture(0);
	Mat img, imgNotice(300, 500, CV_8UC3, Scalar(255, 255, 255));

	putText(imgNotice, "Step 1:", Point(20, 35), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 255), 2);
	putText(imgNotice, "Press [SPACE] to Take Picture", Point(20, 70), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 255), 2);
	putText(imgNotice, "Step 2:", Point(20, 120), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 255), 2);
	putText(imgNotice, "Press [SPACE] to Confirm Picture", Point(20, 155), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 255), 2);
	putText(imgNotice, "To Exit:", Point(20, 205), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 255), 2);
	putText(imgNotice, "Press [BACKSPACE]", Point(20, 240), FONT_HERSHEY_COMPLEX, 0.8, Scalar(0, 0, 255), 2);

	imshow("Info", imgNotice);

	while (1) {
		capture.read(img);
		flip(img, img, 1);

		imshow("OpenCV Camera", img);
		int k = waitKey(1);
		if (k == 32) {
			int k1 = waitKey(-1);
			if (k1 == 32) {
				imgInput = img;
				destroyAllWindows();
				return 1;
			}
			else if (k1 == 8) {
				break;
			}
		}
		else if (k == 8) {
			break;
		}
	}
	destroyAllWindows();
	return 0;
}

// Default Constructor
ObjectImg::ObjectImg(Mat img, string type) {
	imgType = type;
	imgOriginal = img;
	if (type == "resize") {
		float divider = imgOriginal.rows / 525.0;
		resize(imgOriginal, imgOriginal, Size(int(imgOriginal.cols / divider), 525));

		// Save as temprorary image file
		directPath = saveFile(imgOriginal, "Temp");
	}
	imgWidth = imgOriginal.cols;
	imgHeight = imgOriginal.rows;
}
// Actual Constructor
ObjectImg::ObjectImg(string path, string type) {
	imgType = type;
	directPath = path;
	imgOriginal = imread(directPath);
	if (type == "resize") {
		float divider = imgOriginal.rows / 525.0;
		resize(imgOriginal, imgOriginal, Size(int(imgOriginal.cols / divider), 525));

		// Save as temprorary image file
		directPath = saveFile(imgOriginal, "Temp");
	}
	imgWidth = imgOriginal.cols;
	imgHeight = imgOriginal.rows;
}
// Destructor
ObjectImg::~ObjectImg() {
	if (imgType == "resize") {
		char* directPathchar = &directPath[0];
		remove(directPathchar);
	}
}

// All set functions
void ObjectImg::setdirectPath(string path) {
	directPath = path;
}
void ObjectImg::setimgOriginal(Mat img) {
	imgOriginal = img;
}
void ObjectImg::setimgPreprocess(Mat img) {
	imgPreprocess = img;
}
void ObjectImg::setimgWarp(Mat img) {
	imgWarp = img;
}
void ObjectImg::setimgScan(Mat img) {
	imgScan = img;
}
void ObjectImg::setimgWidth(int w) {
	imgWidth = w;
}
void ObjectImg::setimgHeight(int h) {
	imgHeight = h;
}
void ObjectImg::setmainCont(vector<Point> cont) {
	mainCont = cont;
}

// All get functions
string ObjectImg::getdirectPath() {
	return directPath;
}
Mat ObjectImg::getimgOriginal() {
	return imgOriginal;
}
Mat ObjectImg::getimgPreprocess() {
	return imgPreprocess;
}
Mat ObjectImg::getimgWarp() {
	return imgWarp;
}
Mat ObjectImg::getimgScan() {
	return imgScan;
}
int ObjectImg::getimgWidth() {
	return imgWidth;
}
int ObjectImg::getimgHeight() {
	return imgHeight;
}
vector<Point> ObjectImg::getmainCont() {
	return mainCont;
}

// All process functions
Mat ObjectImg::preProcessing(Mat imgInput) {
	// Temporary preprocessing images
	Mat imgGray, imgBlur, imgCanny, imgDilate, imgErode;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat kernel1 = getStructuringElement(MORPH_RECT, Size(5, 5));

	// Start precprocessing using this->imgOriginal as reference
	cvtColor(imgInput, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(9, 9), 3, 0);				// bilateralFilter(imgInput, imgBlur, 9, 75, 75);
	Canny(imgBlur, imgCanny, 25, 75);
	dilate(imgCanny, imgDilate, kernel);
	erode(imgDilate, imgErode, kernel1);

	return imgErode;
}

vector<Point> ObjectImg::processBiggestContourPoints(Mat imgInput, Mat imgOutput) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Point boundPoint(0, 0);
	int maxArea = 0, Arraynum = 0;

	// Find all contours (Detected shapes) in the preprocessed image
	findContours(imgInput, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> contoursPoly(contours.size());

	// Loop through all contours detected
	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);

		if (area > 2500) {
			double peri = arcLength(contours[i], true);

			// Find the edges of each contours detected in the prepocessed image
			approxPolyDP(contours[i], contoursPoly[i], 0.02 * peri, true);

			// Find biggest area Contour
			if (contoursPoly[i].size() == 4 && area > maxArea) {
				maxArea = area;
				Arraynum = i;
				// drawContours(imgOutput, contoursPoly, i, Scalar(255, 0, 255), 2);
			}
		}
	}
	// Found biggest Area return that contour
	if (maxArea != 0) {
		return contoursPoly[Arraynum];
	}
	// return empty contour
	else {
		vector<Point> pointEmpty;
		return pointEmpty;
	}
}

vector<Point> ObjectImg::reorderContourPoints() {
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;
	for (int i = 0; i < mainCont.size(); i++) {
		sumPoints.push_back(mainCont[i].x + mainCont[i].y);		// Maximum distance
		subPoints.push_back(mainCont[i].x - mainCont[i].y);		// Minimum distance
	}
	newPoints.push_back(mainCont[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 0
	newPoints.push_back(mainCont[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 1
	newPoints.push_back(mainCont[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 2
	newPoints.push_back(mainCont[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 3

	// reordering Points and return
	return newPoints;
}

Mat ObjectImg::processWarp(Mat imgInput, float w, float h) {
	Point2f dstPoints[4] = { {0.0f,0.0f}, {w, 0.0f}, {0.0f,h}, {w,h} };
	Point2f srcPoints[4] = { mainCont[0], mainCont[1], mainCont[2], mainCont[3] };
	Mat matrix, imgWarpOutput;
	matrix = getPerspectiveTransform(srcPoints, dstPoints);
	warpPerspective(imgInput, imgWarpOutput, matrix, Size(w, h));

	// Warp image and return
	return imgWarpOutput;
}

Mat ObjectImg::processCrop(Mat imgInput, int cropVal) {
	Rect roi(cropVal, cropVal, imgInput.cols - (2 * cropVal), imgInput.rows - (2 * cropVal));
	return imgWarp(roi);
}

Mat ObjectImg::processScan(Mat imgInput) {
	Mat imgTemp;

	// Convert Color to grey then use threshold algorithm
	cvtColor(imgInput, imgTemp, COLOR_BGR2GRAY);
	threshold(imgTemp, imgTemp, 128, 255, THRESH_BINARY | THRESH_OTSU);

	// Apply Histogram Equalization
	equalizeHist(imgTemp, imgTemp);

	return imgTemp;
}
