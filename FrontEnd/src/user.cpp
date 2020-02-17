#include <string>

using namespace std;

class user {
private:
	static enum userTypes : unsigned int {ADMIN = 0b1000, FULL_STANDARD = 0b0100, BUY_STANDARD = 0b0010, SELL_STANDARD = 0b0001};
	string username;
	unsigned int userType : 4;
	double credit;
public:
	user(string username, string userType, string credit) {
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
	}

	string getUsername() {
		return username;
	}

	double getCredit() {
		return credit;
	}

	unsigned int getUserType() {
		return userType;
	}

	string getUserTypeAsString() {
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
};