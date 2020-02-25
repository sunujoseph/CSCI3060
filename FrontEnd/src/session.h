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
	static std::string getInputWithSpaces(std::string prompt, std::string errorMsg, int maxLength);
	template <typename Callable>
	static std::string getMonetaryInputAsString(std::string prompt, Callable constraintF);
	static double getMonetaryInput(std::string prompt, bool(*constraintF)(std::string));
	static std::string pad(std::string data, int size, char padding, char side);

public:
	static session* login();

	void sessionLoop();

	session(user* userObject);
};