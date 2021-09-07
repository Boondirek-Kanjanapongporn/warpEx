/*
ID: 63011117
Name: Boondirek Kanjanapongporn
Date: 2/5/2021
*/
#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

//// Define Paper Sizes // + 6
//vector<Point> paperSize{ Point(626, 880), Point(880, 1246), Point(1246, 1760), Point(1760, 2486), Point(2486, 3514), Point(3514, 4973) };

int fileCheck(string path);

string saveFile(Mat imgInput, string str);

void drawPoints(vector<Point> pointInput, Scalar colorInput, Mat imgInputOutput);

int OpenCVCamera(Mat &imgInput);

class ObjectImg {
private:
	string directPath = "", imgType;
	Mat imgOriginal, imgPreprocess, imgWarp, imgScan;
	vector<Point> mainCont;
	int imgWidth, imgHeight;
public:
	// Constructor img type
	ObjectImg(Mat img, string type = "default");
	// Constructor string type
	ObjectImg(string path, string type = "default");
	// Destructor
	~ObjectImg();
	// All set functions
	void setdirectPath(string path);
	void setimgOriginal(Mat img);
	void setimgPreprocess(Mat img);
	void setimgWarp(Mat img);
	void setimgScan(Mat img);
	void setimgWidth(int w);
	void setimgHeight(int h);
	void setmainCont(vector<Point> cont);
	// All get functions
	string getdirectPath();
	Mat getimgOriginal();
	Mat getimgPreprocess();
	Mat getimgWarp();
	Mat getimgScan();
	int getimgWidth();
	int getimgHeight();
	vector<Point> getmainCont();

	// All process functions
	Mat preProcessing(Mat imgInput);

	vector<Point> processBiggestContourPoints(Mat imgInput, Mat imgOutput);

	vector<Point> reorderContourPoints();

	Mat processWarp(Mat imgInput, float w, float h);

	Mat processCrop(Mat imgInput, int cropVal = 3);

	Mat processScan(Mat imgInput);
};

//// Define Objects //
//unique_ptr<ObjectImg> DefaultImg, ResizeImg;
//Mat imgTemp;
