#include <iostream>
#include <string>
#include <thread>
#include "Header.h"

using namespace std;

class session {
private:
	user* userObject;

public:
	session(user* userObject) {
		this.userObject = userObject;
	}

	void login(thread*& fileWriterThread) {
		
	}

	void sessionLoop() {
		string command;
		cin >> command;
		
	}
};