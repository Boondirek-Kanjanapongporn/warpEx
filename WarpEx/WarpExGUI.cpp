/*
ID: 63011117
Name: Boondirek Kanjanapongporn
Date: 2/5/2021
*/
#include "WarpExGUI.h"
#include "ObjectImg.h"
#include "TesseractExtract.h"

// Define Paper Sizes // + 6
vector<Point> paperSize{ Point(626, 880), Point(880, 1246), Point(1246, 1760), Point(1760, 2486), Point(2486, 3514), Point(3514, 4973) };

void RESET_VARIABLES() {
	// bool variables
	mouseOnText = false;
	uploadAction = false;
	cameraAction = false;
	warningPath = false;
	yesAction = false;
	noAction = false;
	isWarp = false;
	saveAction = false;
	extractAction = false;
	warningCont = false;
	saveFileInfo = false;
	extractFileInfo = false;
	checkBoolSize = { false, false, true, false, false, false };
	checkBoolLang = { true, false, false, false, false, false };

	// int variables
	sizeChoice = 2;
	langChoice = 0;
}

void DRAW_MENU_BAR() {
	DrawRectangleRounded(menu_bar, 0.5f, 1, LIGHTGRAY);
	for (int i = 0; i < 4; i++) {
		Color linecolor = DARKGRAY;
		Color outterCirclecolor = DARKGRAY;
		Color innerCirclecolor = LIGHTGRAY;
		Color textcolor = DARKGRAY;
		std::string s = std::to_string(i + 1);
		char const* pchar = s.c_str();

		// Redefine linecolor and draw Line
		if (i != 3) {
			if (currentScreen == CONVERT && i < 1 || currentScreen == RESULT && i < 2 || currentScreen == END && i < 3) {
				linecolor = DARKORANGE;
			}
			DrawLineEx(circleMenuBar[i], circleMenuBar.at(i + 1), 3.0f, linecolor);
		}

		// Redefine outterCirclecolor, innerCirclecolor, and textcolor 
		if (currentScreen == HOME && i < 1 || currentScreen == CONVERT && i < 2 || currentScreen == RESULT && i < 3 || currentScreen == END && i < 4) {
			if (CheckCollisionPointCircle(mousePosition, circleMenuBar[0], 18) && i == 0) {
				innerCirclecolor = CHERRYRED;
				textcolor = RAYWHITE;
			}
			else if (currentScreen == HOME && i == 0 || currentScreen == CONVERT && i == 1 || currentScreen == RESULT && i == 2 || currentScreen == END && i == 3) {
				innerCirclecolor = ORANGE;
				textcolor = RAYWHITE;
			}
			else {
				innerCirclecolor = RAYWHITE;
				textcolor = DARKORANGE;
			}
			outterCirclecolor = DARKORANGE;
		}

		// Draw Circle and Draw Text
		DrawCircleV(circleMenuBar[i], 18, outterCirclecolor);
		DrawCircleV(circleMenuBar[i], 15, innerCirclecolor);
		if (i == 0) {
			DrawText(pchar, circleMenuBar[i].x - 3, circleMenuBar[i].y - 11, 25, textcolor);
		}
		else if (i != 3) {
			DrawText(pchar, circleMenuBar[i].x - 6, circleMenuBar[i].y - 11, 25, textcolor);
		}
		else {
			DrawText(pchar, circleMenuBar[i].x - 7, circleMenuBar[i].y - 11, 25, textcolor);
		}
	}

	if (CheckCollisionPointCircle(mousePosition, circleMenuBar[0], 18)) {
		if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
			RESET_VARIABLES();

			// Go back to Home page
			currentScreen = HOME;
		}
	}
}

void My_Project() {
	// Initialization -----------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "WarpEX");

	// Define Objects //
	unique_ptr<ObjectImg> DefaultImg, ResizeImg;
	Mat imgTemp;

	// Load Textures
	Texture2D WarpEx_logo = LoadTexture("Resources/logo.png");
	Texture2D upload_icon = LoadTexture("Resources/upload.png");
	Texture2D camera_icon = LoadTexture("Resources/camera.png");
	Texture2D close_icon = LoadTexture("Resources/close.png");
	Texture2D thankyou_icon = LoadTexture("Resources/thank-you.png");
	Texture2D DisplayImage;

	// Load Texture Vectors
	Vector2 logo_vector = { (screenWidth - WarpEx_logo.width) / 2, (screenHeight - WarpEx_logo.height) / 2 };
	Vector2 upload_vector = { screenWidth - 500 + 70, screenHeight - 150 + 5 };
	Vector2 upload1_vector = { screenWidth - 500, screenHeight - 150 };
	Vector2 camera_vector = { screenWidth - 250 + 73, screenHeight - 150 + 13 };
	Vector2 close_vector = { screenWidth - 100, screenHeight - 570 };
	Vector2 thankyou_vector = { (screenWidth - thankyou_icon.width) / 2, (screenHeight - thankyou_icon.height) / 2 };

	SetTargetFPS(60);					// Set our game to run at 60 frames-per-second
	// --------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose()) {		// Detect window close button or ESC key
		// Update----------------------------------------------------------------
		mousePosition = GetMousePosition();

		// Draw------------------------------------------------------------------
		BeginDrawing();

		// Display background----------------------------------------------------
		ClearBackground(RAYWHITE);

		// Draw Menu-Bar---------------------------------------------------------
		DRAW_MENU_BAR();

		switch (currentScreen) {

		case HOME:
			// Draw Upload and Camera Buttons
			DrawRectangleRounded(uploadButton, 0.2f, 1, CHERRYRED);
			DrawRectangleRounded(cameraButton, 0.2f, 1, CHERRYRED);

			// Draw logos and icons
			DrawTextureEx(WarpEx_logo, logo_vector, 0.0f, 1.0f, WHITE);
			DrawTextureEx(upload_icon, upload_vector, 0.0f, 0.3f, WHITE);
			DrawTextureEx(camera_icon, camera_vector, 0.0f, 0.043f, WHITE);

			// Draw Text
			DrawText("warp", menu_bar.x + 28, menu_bar.y + 65, 95, RED);
			DrawText("E", menu_bar.x + 253, menu_bar.y + 65, 95, BLACK);
			DrawText("X", menu_bar.x + 318, menu_bar.y + 65, 95, ORANGE);

			// Check collision of upload and camera button
			if (uploadAction == false && cameraAction == false) {
				if (CheckCollisionPointRec(mousePosition, uploadButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						uploadAction = true;
					}
				}
				if (CheckCollisionPointRec(mousePosition, cameraButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						cameraAction = true;
					}
				}
			}

			// Check UploadAction is True
			if (uploadAction == true) {
				// Close Button
				if (CheckCollisionPointRec(mousePosition, closeButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						pathText = "";
						warningPath = false;
						uploadAction = false;
					}
					//cout << pathText << endl;
				}
				// Upload File Button
				if (CheckCollisionPointRec(mousePosition, uploadFileButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						if (fileCheck(pathText)) {
							uploadAction = false;
							warningPath = false;

							// Assign value to Object
							DefaultImg = make_unique<ObjectImg>(path);
							ResizeImg = make_unique<ObjectImg>(path, "resize");

							resizeImgpath = &(ResizeImg->getdirectPath())[0];
							DisplayImage = LoadTexture(resizeImgpath);

							// Continue to Preview Page
							currentScreen = CONVERT;
						}
						else {
							warningPath = true;
						}
					}
				}

				// MouseOnText detection
				mouseOnText = false;
				if (CheckCollisionPointRec(mousePosition, pathTextBox)) {
					mouseOnText = true;
				}
				if (mouseOnText) {
					// Get pressed key (character) on the queue
					int key = GetKeyPressed();
					while (key > 0) {
						if (key >= 32 && key <= 122) {
							pathText += (char)key;
						}
						key = GetKeyPressed();
					}
					// Check backspace key
					if (IsKeyPressed(KEY_BACKSPACE) && !pathText.empty()) {
						pathText.erase(prev(pathText.end()));
					}
					path = &pathText[0];
				}

				// Draw relevent rectangles
				DrawRectangleRounded(screenWindow, 0.0f, 1, BLACKTRANS85);
				DrawRectangleRounded(popupWindow, 0.1f, 1, RAYWHITE);
				DrawRectangleRounded(pathTextBox, 0.2f, 1, LIGHTGRAY);
				DrawRectangleRounded(uploadFileButton, 0.2f, 1, CHERRYRED);
				// Draw close button
				DrawTextureEx(close_icon, close_vector, 0.0f, 1.0f, WHITE);
				// Draw texts
				DrawText("Add Path: ", popupWindow.x + 20, popupWindow.y + 45, 30, BLACK);
				DrawText(path, pathTextBox.x + 10, pathTextBox.y + 20, 20, BLACK);
				DrawText("UPLOAD", uploadFileButton.x + 43, uploadFileButton.y + 22, 30, WHITE);

				// Draw Red Border on path TextBox
				if (mouseOnText) {
					DrawRectangleRoundedLines(pathTextBox, 0.2f, 1, 2, RED);
				}

				// Warning Notice
				if (warningPath) {
					DrawText("*File path not exist", pathTextBox.x + 10, pathTextBox.y + pathTextBox.height + 10, 25, RED);
				}
			}

			// Check CameraAction is True
			else if (cameraAction == true) {
				// Open Camera totcapture image
				if (OpenCVCamera(imgTemp)) {

					// Assign value to Object
					DefaultImg = make_unique<ObjectImg>(imgTemp);
					ResizeImg = make_unique<ObjectImg>(imgTemp, "resize");

					resizeImgpath = &(ResizeImg->getdirectPath())[0];
					DisplayImage = LoadTexture(resizeImgpath);

					// Continue to Preview Page
					currentScreen = CONVERT;
				}
				cameraAction = false;
			}
			break;

		case CONVERT:
			// Draw DisplayImage and Buttons
			DrawTexture(DisplayImage, (screenWidth - ResizeImg->getimgWidth()) / 2, 130, WHITE);
			DrawRectangleRounded(yesButton, 0.2f, 1, CHERRYRED);
			DrawRectangleRounded(noButton, 0.2f, 1, CHERRYRED);
			// Draw Texts
			DrawText("Need Warping?", menu_bar.x + 72, menu_bar.y + 63, 35, CHERRYRED);
			DrawText("YES", yesButton.x + 70, yesButton.y + 22, 30, RAYWHITE);
			DrawText("NO", noButton.x + 79, noButton.y + 22, 30, RAYWHITE);

			// Check collision of yes and no button
			if (yesAction == false && noAction == false) {
				if (CheckCollisionPointRec(mousePosition, yesButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						yesAction = true;
					}
				}
				if (CheckCollisionPointRec(mousePosition, noButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						noAction = true;
					}
				}
			}

			// Check YesAction is True
			if (yesAction == true) {
				// Draw relevent rectangles
				DrawRectangleRounded(screenWindow, 0.0f, 1, BLACKTRANS85);
				DrawRectangleRounded(popupWindow, 0.1f, 1, RAYWHITE);
				DrawRectangleRounded(sureButton, 0.2f, 1, CHERRYRED);
				// Draw close button
				DrawTextureEx(close_icon, close_vector, 0.0f, 1.0f, WHITE);
				// Draw texts
				DrawText("Are you Sure?", popupWindow.x + 110, popupWindow.y + 85, 30, BLACK);
				DrawText("SURE", sureButton.x + 61, sureButton.y + 22, 30, WHITE);
				// Draw Warning Notice
				if (warningCont) {
					DrawText("*Can't Find Contours", popupWindow.x + 92, popupWindow.y + 170, 25, RED);
				}

				// CloseButton Process
				if (CheckCollisionPointRec(mousePosition, closeButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						yesAction = false;
						warningCont = false;
					}
				}
				// SureButton Process
				if (CheckCollisionPointRec(mousePosition, sureButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						// Display Preprocessing
						ResizeImg->setimgPreprocess(ResizeImg->preProcessing(ResizeImg->getimgOriginal()));
						// Find Biggest Contour
						ResizeImg->setmainCont(ResizeImg->processBiggestContourPoints(ResizeImg->getimgPreprocess(), ResizeImg->getimgOriginal()));

						if (!(ResizeImg->getmainCont()).empty()) {
							yesAction = false;
							warningCont = false;
							isWarp = true;

							// Reorder Contour Points
							ResizeImg->setmainCont(ResizeImg->reorderContourPoints());
							// Warp image
							ResizeImg->setimgWarp(ResizeImg->processWarp(ResizeImg->getimgOriginal(), ResizeImg->getimgWidth() + 6, ResizeImg->getimgHeight() + 6));
							// Crop image
							ResizeImg->setimgWarp(ResizeImg->processCrop(ResizeImg->getimgWarp()));
							// Scan image
							ResizeImg->setimgScan(ResizeImg->processScan(ResizeImg->getimgWarp()));

							// Overload Temp Resize img with Scan Temp Resize img
							imwrite(ResizeImg->getdirectPath(), ResizeImg->getimgScan());
							resizeImgpath = &(ResizeImg->getdirectPath())[0];
							DisplayImage = LoadTexture(resizeImgpath);

							// Continue to Preview Page
							currentScreen = RESULT;
						}
						else {
							warningCont = true;
						}
					}
				}
			}

			// Check NoAction is True
			if (noAction == true) {
				// Draw relevent rectangles
				DrawRectangleRounded(screenWindow, 0.0f, 1, BLACKTRANS85);
				DrawRectangleRounded(popupWindow, 0.1f, 1, RAYWHITE);
				DrawRectangleRounded(sureButton, 0.2f, 1, CHERRYRED);
				// Draw close button
				DrawTextureEx(close_icon, close_vector, 0.0f, 1.0f, WHITE);
				// Draw texts
				DrawText("Are you Sure?", popupWindow.x + 110, popupWindow.y + 85, 30, BLACK);
				DrawText("SURE", sureButton.x + 61, sureButton.y + 22, 30, WHITE);

				// Close Button Process
				if (CheckCollisionPointRec(mousePosition, closeButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						noAction = false;
					}
				}
				// SureButton Process
				if (CheckCollisionPointRec(mousePosition, sureButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						noAction = false;
						// Scan image
						ResizeImg->setimgScan(ResizeImg->processScan(ResizeImg->getimgOriginal()));

						// Overload Temp Resize img with Scan Resize img
						imwrite(ResizeImg->getdirectPath(), ResizeImg->getimgScan());
						resizeImgpath = &(ResizeImg->getdirectPath())[0];
						DisplayImage = LoadTexture(resizeImgpath);

						// Continue to Preview Page
						currentScreen = RESULT;
					}
				}
			}

			break;

		case RESULT:
			// Draw Display Image and revelant rectangles
			DrawTexture(DisplayImage, (screenWidth - ResizeImg->getimgWidth()) / 2, 130, WHITE);
			DrawRectangleRounded(saveDocButton, 0.2f, 1, CHERRYRED);
			DrawRectangleRounded(extractTextButton, 0.2f, 1, CHERRYRED);
			// Draw Texts
			DrawText("Scan Document & Extract Text", menu_bar.x - 59, menu_bar.y + 63, 33, CHERRYRED);
			DrawText("SCAN", saveDocButton.x + 60, saveDocButton.y + 22, 30, RAYWHITE);
			DrawText("EXTRACT", extractTextButton.x + 27, extractTextButton.y + 22, 30, RAYWHITE);
			DrawText("Press [ENTER] to 4th Page", saveDocButton.x + 13, saveDocButton.y + 90, 30, RED);

			// Check collision of Save and Extract button
			if (saveAction == false && extractAction == false) {
				if (CheckCollisionPointRec(mousePosition, saveDocButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						saveAction = true;
					}
				}
				if (CheckCollisionPointRec(mousePosition, extractTextButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						extractAction = true;
					}
				}
			}

			// Check saveAction is True
			if (saveAction == true) {
				// Draw relevent rectangles
				DrawRectangleRounded(screenWindow, 0.0f, 1, BLACKTRANS85);
				DrawRectangleRounded(popupWindow, 0.1f, 1, RAYWHITE);
				DrawRectangleRounded(extractButton, 0.2f, 1, CHERRYRED);
				// Draw CheckBoxes
				for (int i = 0; i < CheckBoxesSize.size(); i++) {
					// Draw CheckBox lines
					DrawRectangleRoundedLines(CheckBoxesSize[i], 0.0f, 1, 3.0f, BLACK);
					if (CheckCollisionPointRec(mousePosition, CheckBoxesSize[i])) {
						if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
							for (int j = 0; j < checkBoolSize.size(); j++) {
								if (j == i)
									checkBoolSize[j] = true;
								else
									checkBoolSize[j] = false;
							}
						}
					}
					if (checkBoolSize[i] == true) {
						sizeChoice = i;
						DrawRectangleRounded(CheckBoxesSize[i], 0.0f, 1, RED);
						DrawText(paperSizesStr[i], CheckBoxesSize[i].x + 30, CheckBoxesSize[i].y - 3, 30, RED);
					}
					else {
						DrawText(paperSizesStr[i], CheckBoxesSize[i].x + 30, CheckBoxesSize[i].y - 3, 30, BLACK);
					}
				}
				// Draw close button
				DrawTextureEx(close_icon, close_vector, 0.0f, 1.0f, WHITE);
				// Draw texts
				DrawText("Choose Size:", popupWindow.x + 20, popupWindow.y + 45, 30, BLACK);
				DrawText("SAVE", saveButton.x + 58, saveButton.y + 22, 30, WHITE);
				// Draw saveFile info
				if (saveFileInfo) {
					DrawText("*Saved to:", popupWindow.x + 20, saveButton.y + 71, 25, RED);
					DrawText(scanImgpath, popupWindow.x + 20, saveButton.y + 95, 25, RED);
				}

				// Close Button process
				if (CheckCollisionPointRec(mousePosition, closeButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						saveAction = false;
						saveFileInfo = false;
						for (int i = 0; i < checkBoolSize.size(); i++) {
							if (i == 2)
								checkBoolSize[i] = true;
							else
								checkBoolSize[i] = false;
						}
					}
				}
				// SaveButton process
				if (CheckCollisionPointRec(mousePosition, saveButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						if (isWarp == true) {
							// Preprocess
							DefaultImg->setimgPreprocess(DefaultImg->preProcessing(DefaultImg->getimgOriginal()));
							// Find Biggest Contour
							DefaultImg->setmainCont(DefaultImg->processBiggestContourPoints(DefaultImg->getimgPreprocess(), DefaultImg->getimgOriginal()));
							if (!(DefaultImg->getmainCont()).empty()) {
								// Reorder Contour
								DefaultImg->setmainCont(DefaultImg->reorderContourPoints());
								// Wrap image
								DefaultImg->setimgWarp(DefaultImg->processWarp(DefaultImg->getimgOriginal(), paperSize[sizeChoice].x, paperSize[sizeChoice].y));
								// Crop image
								DefaultImg->setimgWarp(DefaultImg->processCrop(DefaultImg->getimgWarp()));
								// Scan image
								DefaultImg->setimgScan(DefaultImg->processScan(DefaultImg->getimgWarp()));
								// Save img path
								defaultImgpath1 = saveFile(DefaultImg->getimgScan(), "Save");
								scanImgpath = &defaultImgpath1[0];
								saveFileInfo = true;
							}
						}
						else {
							// Scan image
							DefaultImg->setimgScan(DefaultImg->processScan(DefaultImg->getimgOriginal()));
							// Save img path
							defaultImgpath1 = saveFile(DefaultImg->getimgScan(), "Save");
							scanImgpath = &defaultImgpath1[0];
							saveFileInfo = true;
						}
					}
				}
			}
			// Check ExtractAction is True
			if (extractAction == true) {
				// Draw relevent rectangles
				DrawRectangleRounded(screenWindow, 0.0f, 1, BLACKTRANS85);
				DrawRectangleRounded(popupWindow, 0.1f, 1, RAYWHITE);
				DrawRectangleRounded(extractButton, 0.2f, 1, CHERRYRED);
				// Draw CheckBoxes
				for (int i = 0; i < CheckBoxesLang.size(); i++) {
					// Draw CheckBox lines
					DrawRectangleRoundedLines(CheckBoxesLang[i], 0.0f, 1, 3.0f, BLACK);
					if (CheckCollisionPointRec(mousePosition, CheckBoxesLang[i])) {
						if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
							for (int j = 0; j < checkBoolLang.size(); j++) {
								if (j == i)
									checkBoolLang[j] = true;
								else
									checkBoolLang[j] = false;
							}
						}
					}
					if (checkBoolLang[i] == true) {
						langChoice = i;
						DrawRectangleRounded(CheckBoxesLang[i], 0.0f, 1, RED);
						DrawText(languangeTextStr[i], CheckBoxesLang[i].x + 30, CheckBoxesLang[i].y - 3, 30, RED);
					}
					else {
						DrawText(languangeTextStr[i], CheckBoxesLang[i].x + 30, CheckBoxesLang[i].y - 3, 30, BLACK);
					}
				}
				// Draw close button
				DrawTextureEx(close_icon, close_vector, 0.0f, 1.0f, WHITE);
				// Draw texts
				DrawText("Choose Language:", popupWindow.x + 20, popupWindow.y + 45, 30, BLACK);
				DrawText("EXTRACT", extractButton.x + 27, extractButton.y + 22, 30, WHITE);
				// Draw extractFile info
				if (extractFileInfo) {
					DrawText("*Extracted to:", popupWindow.x + 20, saveButton.y + 73, 25, RED);
					DrawText(extractTextpath, popupWindow.x + 20, saveButton.y + 98, 25, RED);
				}

				// Close Button
				if (CheckCollisionPointRec(mousePosition, closeButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						extractAction = false;
						extractFileInfo = false;
						for (int i = 0; i < checkBoolLang.size(); i++) {
							if (i == 0)
								checkBoolLang[i] = true;
							else
								checkBoolLang[i] = false;
						}
					}
				}
				// ExtractButton
				if (CheckCollisionPointRec(mousePosition, extractButton)) {
					if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
						if (isWarp == true) {
							// Preprocess
							DefaultImg->setimgPreprocess(DefaultImg->preProcessing(DefaultImg->getimgOriginal()));
							// Find Biggest Contour
							DefaultImg->setmainCont(DefaultImg->processBiggestContourPoints(DefaultImg->getimgPreprocess(), DefaultImg->getimgOriginal()));
							if (!(DefaultImg->getmainCont()).empty()) {
								// Reorder Contour
								DefaultImg->setmainCont(DefaultImg->reorderContourPoints());
								// Wrap image
								DefaultImg->setimgWarp(DefaultImg->processWarp(DefaultImg->getimgOriginal(), paperSize[sizeChoice].x, paperSize[sizeChoice].y));
								// Crop image
								DefaultImg->setimgWarp(DefaultImg->processCrop(DefaultImg->getimgWarp()));

								// Save Temp img path
								defaultImgpath2 = saveFile(DefaultImg->getimgWarp(), "Temp");

								// Extract Text
								extractTextpath = &tesseractExtract(defaultImgpath2, languangeTextChar[langChoice])[0];
								extractFileInfo = true;
							}
						}
						else {
							// Save Temp img path
							defaultImgpath2 = saveFile(DefaultImg->getimgOriginal(), "Temp");

							// Extract Text
							extractTextpath = &tesseractExtract(defaultImgpath2, languangeTextChar[langChoice])[0];
							extractFileInfo = true;
						}
					}
				}
			}
			// Capture Enter key
			if (IsKeyPressed(KEY_ENTER) && saveAction == false && extractAction == false) {
				currentScreen = END;
			}

			break;

		case END:
			// Draw thank you icon
			DrawTexture(thankyou_icon, thankyou_vector.x, thankyou_vector.y + 30, WHITE);
			// Draw relevant rectangles
			DrawRectangleRounded(homeButton, 0.2f, 1, CHERRYRED);
			// Draw texts
			DrawText("THANK", menu_bar.x + 8, menu_bar.y + 65, 110, RED);
			DrawText("YOU", menu_bar.x + 204, menu_bar.y + 155, 95, BLACK);
			DrawText("HOME", homeButton.x + 60, homeButton.y + 22, 30, RAYWHITE);

			// Check collision HomeButton
			if (CheckCollisionPointRec(mousePosition, homeButton)) {
				if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
					RESET_VARIABLES();

					// Go back to Home page
					currentScreen = HOME;
				}
			}

			break;
		}

		EndDrawing();
		//-----------------------------------------------------------------------
	}
	// De-Initialization
	remove(resizeImgpath);
	//---------------------------------------------------------------------------
	// Unload textures
	UnloadTexture(WarpEx_logo);
	UnloadTexture(upload_icon);
	UnloadTexture(camera_icon);
	UnloadTexture(close_icon);
	UnloadTexture(thankyou_icon);
	UnloadTexture(DisplayImage);

	// Close window and OpenGL context
	CloseWindow();
	//---------------------------------------------------------------------------
}


int main() {
	My_Project();
	return 1;
}
