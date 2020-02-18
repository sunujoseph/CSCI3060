#include <iostream>
#include <string>
#include "user.h"
#include "FileReader.h"
#include "transactionFileWriter.h"

using namespace std;

class session {
private:
	user* userObject;

	void logout() {
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

	void advertise() {
		if ((userObject->getUserType & (user::ADMIN | user::FULL_STANDARD | user::SELL_STANDARD)) != userObject->getUserType) {
			cout << "Error: You Do Not Have Privileges To Perform This Transaction" << endl;
			return;
		}
		string name = getInputWithSpaces("Enter Item Name: ", "Error: Invalid Name", 25);
		double minBid = getMonetaryInput("Enter Minimum Bid: ", [](string input) {
			double val = stod(input);
			if ((val < 0) || (val > 999.99)) {
				return false;
			}
			return true;
		});
	}

	void bid() {

	}

	void create() {

	}

	void addCredit() {

	}

	void refund() {

	}

	void deleteUser() {

	}

	static string getInputWithSpaces(string prompt, string errorMsg, int maxLength) {
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
		combinedInput[maxLength + 1] = '\0';
		string retInput = string(combinedInput);
		delete temp;
		delete combinedInput;
		delete inputTemp;
		return retInput;
	}

	static double getMonetaryInput(string prompt, bool (*constraintF)(string)) {
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
			else {
				validInput = false;
			}
		}
		
		return stod(input);
	}

public:
	/*
	reads the userAccounts file and finds the user details with the
	given account name

	if name is found in userAccounts file, instantiates userObject
	and returns a new session

	if name is not found returns null
	*/
	static session* login() {
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

	void sessionLoop() {
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

	session(user* userObject) {
		this->userObject = userObject;
	}

};