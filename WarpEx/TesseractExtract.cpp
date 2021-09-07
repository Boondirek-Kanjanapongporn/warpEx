/*
ID: 63011117
Name: Boondirek Kanjanapongporn
Date: 2/5/2021
*/
#include "TesseractExtract.h"

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <memory>

using namespace std;

string saveText(string pathInput, char* textInput) {
	ofstream MyFile;
	string pathReal = "Resources/Save/";

	if (!pathInput.empty()) {
		pathReal += pathInput.substr(15, 10);
		pathReal += ".txt";

		MyFile.open(pathReal);
		MyFile << textInput << endl;
		MyFile.close();
	}
	return pathReal;
}

string tesseractExtract(string pathInput, const char* lang) {
	char* extractText;
	char* filep = &pathInput[0];

	tesseract::TessBaseAPI api;
	if (api.Init(NULL, lang)) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		return "";
	}

	//Pix from leptonica library
	Pix* img = pixRead(filep); // "Resources/Temp/test2.png"
	api.SetImage(img);
	extractText = api.GetUTF8Text();

	if (lang == "eng") {
		cout << extractText << endl;
	}

	string pathReal = saveText(pathInput, extractText);

	// Release memory space
	api.End();
	pixDestroy(&img);
	delete[] extractText;
	remove(filep);

	cout << pathReal << endl;
	return pathReal;
}