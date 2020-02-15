#include <iostream>
#include <string>
#include <thread>
#include "user.h"

using namespace std;

class session {
private:
	user* userObject;

	void logout() {

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


public:
	/*
	reads the userAccounts file and finds the user details with the
	given account name

	if name is found in userAccounts file, instantiates userObject
	and returns a new session

	if name is not found returns null
	*/
	static session* login() {
		
	}

	void sessionLoop() {
		string command;
		cin >> command;
		
	}

};