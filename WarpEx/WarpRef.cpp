#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>

using namespace std;
using namespace cv;

Mat preProcessing(Mat imgInput) {

	Mat imgGray, imgBlur, imgCanny, imgDilate, imgErode;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat kernel1 = getStructuringElement(MORPH_RECT, Size(3, 3));

	cvtColor(imgInput, imgGray, COLOR_BGR2GRAY);
	// bilateralFilter(imgInput, imgBlur, 9, 75, 75);
	GaussianBlur(imgGray, imgBlur, Size(9, 9), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);
	dilate(imgCanny, imgDilate, kernel);
	erode(imgDilate, imgErode, kernel1);

	return imgErode;
}

vector<Point> getContours(Mat imgInput, Mat& imgOutput) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	Point boundPoint(0, 0);
	int maxArea = 0, Arraynum = 0;

	findContours(imgInput, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 2);

	vector<vector<Point>> contoursPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);

		if (area > 5000) {
			double peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], contoursPoly[i], 0.02 * peri, true);
			// cout << contoursPoly[i].size() << endl;
			// drawContours(imgOutput, contoursPoly, i, Scalar(255, 0, 255), 2);
			if (contoursPoly[i].size() == 4 && area > maxArea) {
				maxArea = area;
				Arraynum = i;
				// drawContours(imgOutput, contoursPoly, i, Scalar(255, 0, 255), 2);
			}
		}
	}
	if (maxArea != 0) {
		return contoursPoly[Arraynum];
	}
	else {
		vector<Point> pointEmpty;
		return pointEmpty;
	}
};

void drawPoints(vector<Point> pointInput, Scalar colorInput, Mat& imgInputOutput) {
	for (int i = 0; i < pointInput.size(); i++) {
		circle(imgInputOutput, pointInput[i], 10, colorInput, FILLED);
		putText(imgInputOutput, to_string(i), pointInput[i], FONT_HERSHEY_PLAIN, 5, colorInput, 5);
	}
}

vector<Point> reorder(vector<Point> pointInput) {
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;
	for (int i = 0; i < pointInput.size(); i++) {
		sumPoints.push_back(pointInput[i].x + pointInput[i].y);
		subPoints.push_back(pointInput[i].x - pointInput[i].y);
	}
	newPoints.push_back(pointInput[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 0
	newPoints.push_back(pointInput[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 1
	newPoints.push_back(pointInput[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]); // 2
	newPoints.push_back(pointInput[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); // 3

	return newPoints;
}

Mat getWarp(Mat imgInput, vector<Point> pointInput, float w, float h) {
	Point2f dstPoints[4] = { {0.0f,0.0f}, {w, 0.0f}, {0.0f,h}, {w,h} };
	Point2f srcPoints[4] = { pointInput[0], pointInput[1], pointInput[2], pointInput[3] };
	Mat matrix, imgWarpOutput;
	matrix = getPerspectiveTransform(srcPoints, dstPoints);
	warpPerspective(imgInput, imgWarpOutput, matrix, Size(w, h));

	return imgWarpOutput;
}

Mat scanPreprocessing(Mat imgInput) {
	Mat imgGrey;

	cvtColor(imgInput, imgGrey, COLOR_BGR2GRAY);
	threshold(imgGrey, imgGrey, 128, 255, THRESH_BINARY | THRESH_OTSU);
	/*adaptiveThreshold(imgGrey, imgGrey, 255, 1, 1, 7, 2);
	bitwise_not(imgGrey, imgGrey);*/

	// Apply Histogram Equalization
	equalizeHist(imgGrey, imgGrey);

	return imgGrey;
}

void ExtractProcess(Mat imgInput) {
	cvtColor(imgInput, imgInput, COLOR_BGR2RGBA);

	tesseract::TessBaseAPI api;
	api.Init(NULL, "eng");
	Pix* img = pixRead("Resources/test2.png");
	api.SetImage(img);
	char* outText = api.GetUTF8Text();
	cout << outText << endl;

	// Release memory space
	api.End();
	delete[] outText;
	pixDestroy(&img);
}

int main() {
	// images //
	Mat img, imgThreshold, imgWarp, imgCrop, imgCropGray, imgScan;
	string path = "Resources/paper.jpg";
	float width = 420, height = 596;
	img = imread(path);
	vector<Point> biggestCont, orderedCont;

	int cropVal = 3;
	Rect roi(cropVal, cropVal, width - (2 * cropVal), height - (2 * cropVal));

	resize(img, img, Size(), 0.4, 0.4);

	// Preprocessing
	imgThreshold = preProcessing(img);
	// Get Contours - Biggest
	biggestCont = getContours(imgThreshold, img);

	if (!biggestCont.empty()) {
		// drawPoints(biggestCont, (0, 0, 255), img);
		orderedCont = reorder(biggestCont);
		// drawPoints(orderedCont, (0, 0, 255), img);
		// Warp
		imgWarp = getWarp(img, orderedCont, width, height);
		// Crop
		imgCrop = imgWarp(roi);

		imgScan = scanPreprocessing(imgCrop);

		imshow("Img Crop", imgCrop);
		imshow("Img Scan", imgScan);

		// Extract Text
		ExtractProcess(imgCrop);

	}

	imshow("image", img);
	// imshow("Img Erode", imgThreshold);

	waitKey(0);
	return 0;
}