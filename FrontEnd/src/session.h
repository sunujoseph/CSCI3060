#pragma once
#include "user.h"
#include <string>
#include <vector>

class session {
private:
	user* userObject;
	std::vector<user*> userCreditTracker;

	void logout();
	void advertise();
	void bid();
	void create();
	void addCredit();
	void refund();
	void deleteUser();

public:
	static session* login();

	void sessionLoop();

	session(user* userObject);
	~session();
};