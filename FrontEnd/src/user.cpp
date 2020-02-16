#include <string>

using namespace std;

class user {
private:
	string username;
	unsigned int userType : 4;
	double credit;
public:
	user(string username, string userType, string credit) {
		this->username = username;
		if (userType.compare("AA") == 0) {
			this->userType = 0b1000;
		}
		else if (userType.compare("FS") == 0) {
			this->userType = 0b0100;
		}
		else if (userType.compare("BS") == 0) {
			this->userType = 0b0010;
		}
		else {
			this->userType = 0b0001;
		}
		this->credit = stod(credit);
	}
};