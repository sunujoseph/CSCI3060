#include <iostream>
#include <string>
#include <thread>
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

	static string getUsername() {
		string username;
		string input;
		char temp[17];
		bool validName = false;
		while (!validName) {
			cout << "Enter Username: ";
			cin >> input;
			
			//everything below this point, except for length checking, is to allow usernames to contain spaces
			
			cin.readsome(temp, 16 - input.length());
			if ((cin.gcount() + input.length() > 16) || (cin.gcount() + input.length() < 2)) {
				cout << "Error: Invalid Username" << endl;
			}
			else {
				validName = true;
			}
		}
		char temp2[16];
		const char* inputTemp = input.c_str();
		for (int i = 0; i < input.length(); i++) {
			temp2[i] = inputTemp[i];
		}
		for (int i = 0; i < cin.gcount() - 2; i++) {
			temp2[input.length() + i] = temp[i];
		}
		for (int i = input.length() + cin.gcount() - 1; i < 16; i++) {
			temp2[i] = ' ';  //add spaces for comparisson with CUA usernames
		}
		temp2[16] = '\0';
		username = string(temp2);
		return username;
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
		string username = session::getUsername();

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