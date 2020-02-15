#pragma once
#include "user.h"

class session {
private:
	user* userObject;

public:
	static session* login();

	void sessionLoop();
};