#define _CRT_SECURE_NOWARNINGS
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <iostream>

#include <fstream>

using namespace std;

int main()
{
    char* outText;
    string Text;

    tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
    // Initialize tesseract-ocr with English, without specifying tessdata path
    if (api->Init(NULL, "tha")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        exit(1);
    }

    // Open input image with leptonica library
    Pix* image = pixRead("C:/Users/boond/OneDrive/Desktop/Capture.JPG"); // C:/Users/boond/OneDrive/Desktop/Capture.JPG Resources/test.png
    api->SetImage(image);
    // Get OCR result
    outText = api->GetUTF8Text();

    /*ofstream MyFile;
    MyFile.open("test.txt");
    MyFile << outText << endl;
    MyFile.close();*/

    //printf("OCR output:\n%s", outText);

    // Destroy used object and release memory
    api->End();
    delete[] outText;
    pixDestroy(&image);

    return 0;
}