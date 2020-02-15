#pragma once
#include "user.h"

class session {
private:
	user* userObject;

public:
	static void login();

	void sessionLoop();
};