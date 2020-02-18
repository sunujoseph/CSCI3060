#pragma once
#include <string>

class user {
private:
	string username;
	unsigned int userType : 4;
	double credit;
public:
	enum userTypes : unsigned int { ADMIN = 0b1000, FULL_STANDARD = 0b0100, BUY_STANDARD = 0b0010, SELL_STANDARD = 0b0001 };

	user(string username, string userType, string credit);

	string getUsername();
	double getCredit();
	unsigned int getUserType();
	string getUserTypeAsString();
};