/*
ID: 63011117
Name: Boondirek Kanjanapongporn
Date: 2/5/2021
*/
#pragma once

#include <iostream>
#include <fstream>

std::string saveText(std::string pathInput, char* textInput);

std::string tesseractExtract(std::string pathInput, const char* lang);
