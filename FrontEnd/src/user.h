#pragma once
#include <string>

class user {
private:
	std::string username;
	unsigned int userType : 4;
	double credit;
public:
	enum userTypes : unsigned int { ADMIN = 0b1000, FULL_STANDARD = 0b0100, BUY_STANDARD = 0b0010, SELL_STANDARD = 0b0001 };

	user(std::string username, std::string userType, std::string credit);

	std::string getUsername();
	double getCredit();
	unsigned int getUserType();
	std::string getUserTypeAsString();
};