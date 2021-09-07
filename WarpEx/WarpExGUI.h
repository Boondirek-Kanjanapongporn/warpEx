/*
ID: 63011117
Name: Boondirek Kanjanapongporn
Date: 2/5/2021
*/
#pragma once
#include <raylib.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define screenWidth 550
#define screenHeight 825

#define LIGHTGRAY  Color{200, 200, 200, 230}	// Light Gray
#define BLACKTRANS85 Color{0, 0, 0, 216}		// BLACK color transparent 85%
#define DARKRED Color{139, 0, 0, 255}			// Dark Red
#define CHERRYRED Color{225, 27, 34, 255}		// Cherry Red
#define DARKORANGE Color{255, 140, 0, 255}		// Dark Orange

typedef enum { HOME, CONVERT, RESULT, END } Gamescreen;
Gamescreen currentScreen = HOME;
Font fontDefault = { 0 };

// Booleans
bool mouseOnText = false;
bool uploadAction = false;
bool cameraAction = false;
bool warningPath = false;
bool yesAction = false;
bool noAction = false;
bool isWarp = false;
bool saveAction = false;
bool extractAction = false;
bool warningCont = false;
bool saveFileInfo = false;
bool extractFileInfo = false;
vector<bool> checkBoolSize{ false, false, true, false, false, false };
vector<bool> checkBoolLang{ true, false, false, false, false, false };

// All int, float, double variables
int menu_barw = 400;
int menu_barh = 50;
int popupw = 450;
int popuph = 350;
int pathTextBoxw = 410;
int pathTextBoxh = 60;
int sizeChoice = 2;
int langChoice = 0;

// All string and const char variables
string pathText;
string defaultImgpath1;
string defaultImgpath2;
const char* path;
const char* resizeImgpath;
const char* scanImgpath;
const char* extractTextpath;
vector<const char*> paperSizesStr{ "A6", "A5", "A4", "A3", "A2", "A1" };
vector<const char*> languangeTextStr{ "Eng", "Tha", "Jpn", "Kor", "Chi", "Math" };
vector<const char*> languangeTextChar{ "eng", "tha", "jpn", "kor", "chi_sim", "equ" };

// Vector Positions
Vector2 mousePosition = { 0.0f, 0.0f };
vector<Vector2> circleMenuBar = { { 75 + 80 * 1, 50 } , { 75 + 80 * 2, 50 }, { 75 + 80 * 3, 50 }, { 75 + 80 * 4, 50 } };

// Rectangle
Rectangle screenWindow{ 0, 0, screenWidth, screenHeight };
Rectangle popupWindow = { (screenWidth - popupw) / 2, (screenHeight - popuph) / 2, popupw, popuph };
Rectangle menu_bar = { (screenWidth - menu_barw) / 2, 25, menu_barw, menu_barh };
Rectangle pathTextBox = { (screenWidth - pathTextBoxw) / 2, (screenHeight - pathTextBoxh) / 2 - 60, pathTextBoxw, pathTextBoxh };
Rectangle uploadButton = { screenWidth - 500, screenHeight - 150, 200, 70 };
Rectangle cameraButton = { screenWidth - 250, screenHeight - 150, 200, 70 };
Rectangle yesButton = { screenWidth - 500, screenHeight - 150, 200, 70 };
Rectangle noButton = { screenWidth - 250, screenHeight - 150, 200, 70 };
Rectangle saveDocButton = { screenWidth - 500, screenHeight - 150, 200, 70 };
Rectangle extractTextButton = { screenWidth - 250, screenHeight - 150, 200, 70 };
Rectangle uploadFileButton = { (screenWidth - 200) / 2, screenHeight - 370, 200, 70 };
Rectangle sureButton = { (screenWidth - 200) / 2, screenHeight - 370, 200, 70 };
Rectangle saveButton = { (screenWidth - 200) / 2, screenHeight - 370, 200, 70 };
Rectangle extractButton = { (screenWidth - 200) / 2, screenHeight - 370, 200, 70 };
Rectangle closeButton = { screenWidth - 100, screenHeight - 570, 24, 24 };
Rectangle homeButton = { (screenWidth - 200) / 2, screenHeight - 150, 200, 70 };
vector<Rectangle> CheckBoxesSize = { { popupWindow.x + 135, popupWindow.y + 90, 20, 20 } ,
									 { popupWindow.x + 255, popupWindow.y + 90, 20, 20 } ,
									 { popupWindow.x + 135, popupWindow.y + 135, 20, 20 } ,
									 { popupWindow.x + 255, popupWindow.y + 135, 20, 20 } ,
									 { popupWindow.x + 135, popupWindow.y + 180, 20, 20 } ,
									 { popupWindow.x + 255, popupWindow.y + 180, 20, 20 } };
vector<Rectangle> CheckBoxesLang = { { popupWindow.x + 135, popupWindow.y + 90, 20, 20 } ,
									 { popupWindow.x + 255, popupWindow.y + 90, 20, 20 } ,
									 { popupWindow.x + 135, popupWindow.y + 135, 20, 20 } ,
									 { popupWindow.x + 255, popupWindow.y + 135, 20, 20 } ,
									 { popupWindow.x + 135, popupWindow.y + 180, 20, 20 } ,
									 { popupWindow.x + 255, popupWindow.y + 180, 20, 20 } };

// Functions
void RESET_VARIABLES();
void DRAW_MENU_BAR();
void My_Project();

