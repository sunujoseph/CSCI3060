#pragma once
#include <string>
#include <iostream>
#if(_DEBUG)
#include "main.h"
#define checkTestEnd if (std::cin.peek() == EOF) { std::longjmp(testExit, 1); }
#endif

std::string getInputWithSpaces(std::string prompt, std::string errorMsg, int maxLength);
template <typename Callable>
std::string getMonetaryInputAsString(std::string prompt, Callable constraintF) {
	std::string input;
	bool validInput = false;
	while (!validInput) {
		std::cout << prompt;
#if(_DEBUG)
		checkTestEnd
#endif
			std::cin >> input;
		//check if any remaining input in cin
		if (std::cin.peek() != '\n') {
			std::cout << "Error: Invalid Input" << std::endl;
		}
		for (int i = 0; i < input.length(); i++) {
			if ((input[i] == '.') && (input.length() - i > 3)) {
				std::cout << "Error: Monetary Values Can Have Max 2 Decimal Precision" << std::endl;
			}
		}
		if (constraintF(input)) {
			validInput = true;
		}
	}
	return input;
}
double getMonetaryInput(std::string prompt, bool(*constraintF)(std::string));
std::string pad(std::string data, int size, char padding, char side);
