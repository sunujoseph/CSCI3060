#include <string>
#include "user.h"
#include <sstream>
#include <iomanip>

using namespace std;

user::user(string username, string userType, string credit) {
	this->username = username;
	if (userType.compare("AA") == 0) {
		this->userType = ADMIN;
	}
	else if (userType.compare("FS") == 0) {
		this->userType = FULL_STANDARD;
	}
	else if (userType.compare("BS") == 0) {
		this->userType = BUY_STANDARD;
	}
	else {
		this->userType = SELL_STANDARD;
	}
	this->credit = stod(credit);
	this->startingCredit = stod(credit);
}

string user::getUsername() {
	return username;
}

double user::getCredit() {
	return credit;
}

string user::getCreditAsString() {
	stringstream ss;
	ss << fixed << setprecision(2) << credit;
	return ss.str();
}

unsigned int user::getUserType() {
	return userType;
}

string user::getUserTypeAsString() {
	if (userType == ADMIN) {
		return "AA";
	}
	else if (userType == FULL_STANDARD) {
		return "FS";
	}
	else if (userType == BUY_STANDARD) {
		return "BS";
	}
	return "SS";
}

void user::addCredit(double credit) {
	this->credit += credit;
}

double user::getStartCredit() {
	return this->startingCredit;
}