#pragma once
#include <string>

class user {
private:
	static enum userTypes : unsigned int;
	string username;
	unsigned int userType : 4;
	double credit;
public:
	user(string username, string userType, string credit);

	string getUsername();
	double getCredit();
	unsigned int getUserType();
	string getUserTypeAsString();
};