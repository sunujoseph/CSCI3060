#pragma once

class user {
private:
	string username;
	unsigned int userType : 4;
	double credit;
public:
	user(string username, string userType, string credit);
};