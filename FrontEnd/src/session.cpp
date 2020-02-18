#include <iostream>
#include <string>
#include "user.h"
#include "FileReader.h"
#include "transactionFileWriter.h"
#include "session.h"

using namespace std;

void session::logout() {
	string transaction;
	transaction += "00 ";
	transaction += userObject->getUsername();
	transaction += " ";
	transaction += userObject->getUserTypeAsString();
	transaction += " ";
	transaction += to_string(userObject->getCredit());
	transactionFileWriter::add(transaction);
	transactionFileWriter::writeOut();
}

void session::advertise() {
	if ((userObject->getUserType() & (user::ADMIN | user::FULL_STANDARD | user::SELL_STANDARD)) != userObject->getUserType()) {
		cout << "Error: You Do Not Have Privileges To Perform This Transaction" << endl;
		return;
	}
	string itemName = getInputWithSpaces("Enter Item Name: ", "Error: Invalid Name", 25);
	string minBid = getMonetaryInputAsString("Enter Minimum Bid: ", [](string input) {
		double val = stod(input);
		if (val < 0) {
			cout << "Error: Minimum Bid Cannot Be Negative" << endl;
			return false;
		}
		else if (val > 999.99) {
			cout << "Error: Maximum Price For Item Is 999.99" << endl;
			return false;
		}
		return true;
	});
	string period;
	bool validPeriod = false;
	while (!validPeriod) {
		cout << "Enter Number Of Days Until Auction Ends: ";
		cin >> period;
		if (cin.peek() != '\n') {
			cout << "Error: Invalid Input" << endl;
		}
		else if (stoi(period) < 0) { //assume days until auction starts when item becomes available
			cout << "Error>: Number Of Days Until Auction Ends Cannot Be Negative" << endl;
		}
		else if (stoi(period) > 100) {
			cout << "Error: Maximum Number Of Days Until Auction Ends Is 100" << endl;
		}
		else {
			validPeriod = true;
		}
	}

	string transaction;
	transaction += "03 ";
	transaction += pad(itemName, 25, ' ', 'l');
	transaction += " ";
	transaction += userObject->getUsername();
	transaction += " ";
	transaction += pad(period, 3, '0', 'r');
	transaction += pad(minBid, 6, '0', 'r');
	transactionFileWriter::add(transaction);
}

void session::bid() {

}

void session::create() {

}

void session::addCredit() {

}

void session::refund() {

}

void session::deleteUser() {

}

string session::getInputWithSpaces(string prompt, string errorMsg, int maxLength) {
	string input;
	char* temp = new char[maxLength + 2];
	bool validInput = false;
	while (!validInput) {
		cout << prompt;
		//using ">>" blocks until user enters new line character, but only retrieves input
		//up to first whitespace character
		cin >> input;
		//get remaining input contained in cin
		cin.readsome(temp, maxLength + 1 - input.length());
		if ((cin.gcount() + input.length() > maxLength + 1) || (cin.gcount() + input.length() < 2)) {
			cout << errorMsg << endl;
		}
		else {
			validInput = true;
		}
	}
	char* combinedInput = new char[maxLength + 1];
	const char* inputTemp = input.c_str();
	for (int i = 0; i < input.length(); i++) {
		combinedInput[i] = inputTemp[i];
	}
	for (int i = 0; i < cin.gcount() - 2; i++) {
		combinedInput[input.length() + i] = temp[i];
	}
	for (int i = input.length() + cin.gcount() - 1; i < maxLength + 1; i++) {
		combinedInput[i] = ' ';  //add spaces for padding
	}
	combinedInput[maxLength] = '\0';
	string retInput = string(combinedInput);
	delete[] temp;
	delete[] combinedInput;
	return retInput;
}

string session::getMonetaryInputAsString(string prompt, bool (*constraintF)(string)) {
	string input;
	bool validInput = false;
	while (!validInput) {
		cout << prompt;
		cin >> input;
		//check if any remaining input in cin
		if (cin.peek() != '\n') {
			cout << "Error: Invalid Input" << endl;
		}
		for (int i = 0; i < input.length(); i++) {
			if ((input[i] == '.') && (input.length() - i > 3)) {
				cout << "Error: Monetary Values Can Have Max 2 Decimal Precision" << endl;
			}
		}
		if (constraintF(input)) {
			validInput = true;
		}
	}
	return input;
}

double session::getMonetaryInput(string prompt, bool (*constraintF)(string)) {
	return stod(getMonetaryInputAsString(prompt, constraintF));
}

string session::pad(string data, int size, char padding, char side) {
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
		for (int i = size - data.length(); i < size; i++) {
			paddedData[i] = padding;
		}
	}
	paddedData[size] = '\0';
	string retData(paddedData);
	delete[] paddedData;
	delete[] dataCStr;
	return retData;
}

/*
reads the userAccounts file and finds the user details with the
given account name

if name is found in userAccounts file, instantiates userObject
and returns a new session

if name is not found returns null
*/
session* session::login() {
	string username = getInputWithSpaces("Enter Username: ", "Error: Invalid Username", 15);

	vector<string> currentUserAccounts = FileReader::getCurrentUserAccounts();
	for (int i = 0; i < currentUserAccounts.size() - 1; i++) {
		string& line = currentUserAccounts[i];
		if (line.substr(0, 15).compare(username) == 0) {
			return new session(new user(line.substr(0, 15), line.substr(16, 2), line.substr(19, 9)));
		}
	}
	return NULL;
}

void session::sessionLoop() {
	string command;
	while (true) {
		cin >> command;

		if (command.compare("login") == 0) {
			cout << "Error: Already Logged In" << endl;
		}
		else if (command.compare("logout") == 0) {
			logout();
			break;
		}
		else if (command.compare("advertise") == 0) {

		}
		else if (command.compare("bid") == 0) {

		}
		else if (command.compare("create") == 0) {

		}
		else if (command.compare("addcredit") == 0) {

		}
		else if (command.compare("refund") == 0) {

		}
		else if (command.compare("delete") == 0) {

		}
		else {
			cout << "Error: Invalid Command" << endl;
		}
	}
}

session::session(user* userObject) {
	this->userObject = userObject;
}