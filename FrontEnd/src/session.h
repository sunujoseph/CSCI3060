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

public:
	static session* login();

	void sessionLoop();

	session(user* userObject);
};