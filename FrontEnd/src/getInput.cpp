#include <iostream>
#include <string>
#include "getInput.h"
#if(_DEBUG)
#include "main.h"
#define checkTestEnd if (cin.peek() == EOF) { longjmp(testExit, 1); } else if (prompt.compare("") != 0) {std::cout << std::endl;}
#endif

using namespace std;

string getInputWithSpaces(string prompt, string errorMsg, int maxLength) {
	char* input = new char[maxLength + 1];
	bool validInput = false;
	while (!validInput) {
		cout << prompt;
#if(_DEBUG)
		checkTestEnd
#endif
		cin.getline(input, maxLength + 1);
		if ((cin.rdstate() & ios_base::failbit) != 0) { //if input was longer than maxLength
			bool eof = cin.eof();
			bool bad = cin.bad();
			cin.clear((eof) ? (bad) ? istream::eofbit | istream::badbit : istream::eofbit : (bad) ? istream::badbit : istream::goodbit);
			//newline was not extracted so ignore remaining characters in line
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << errorMsg << endl;
		}
		else if (cin.gcount() < 2) {
			cout << errorMsg << endl;
		}
		else {
			validInput = true;
		}
	}

	string retInput = string(input);
	delete[] input;
	return retInput;
}

double getMonetaryInput(string prompt, bool(*constraintF)(string)) {
	return stod(getMonetaryInputAsString(prompt, constraintF));
}

string pad(string data, int size, char padding, char side) {
	char* paddedData = new char[size + 1];
	const char* dataCStr = data.c_str();
	if (side == 'r') {
		for (int i = 0; i < size - data.length(); i++) {
			paddedData[i] = padding;
		}
		for (int i = 0; i < data.length(); i++) {
			paddedData[i + size - data.length()] = dataCStr[i];
		}
	}
	else {
		for (int i = 0; i < data.length(); i++) {
			paddedData[i] = dataCStr[i];
		}
		for (int i = data.length(); i < size; i++) {
			paddedData[i] = padding;
		}
	}
	paddedData[size] = '\0';
	string retData(paddedData);
	delete[] paddedData;
	return retData;
}