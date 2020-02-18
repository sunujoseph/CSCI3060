#pragma once
#include "user.h"
#include <string>

class session {
private:
	user* userObject;

	void logout();
	void advertise();
	void bid();
	void create();
	void addCredit();
	void refund();
	void deleteUser();
	static string getInputWithSpaces(string prompt, string errorMsg, int maxLength);
	static string getMonetaryInputAsString(string prompt, bool(*constraintF)(string));
	static double getMonetaryInput(string prompt, bool(*constraintF)(string));
	static string pad(string data, int size, char side);

public:
	static session* login();

	void sessionLoop();

	session(user* userObject);
};