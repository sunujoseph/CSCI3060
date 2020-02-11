#include <iostream>
#include <string>
#include "Header.h"

using namespace std;

class session {
private:
	user* userObject;

public:
	session(user* userObject) {
		this.userObject = userObject;
	}

	void login() {

	}

	void sessionLoop() {
		string command;
		cin >> command;
		if ((user == NULL) && (command.compare("login") != 0)) {
			cout << "Error: Not Logged In" << endl;
		}
		else {
			login();
		}
	}
};