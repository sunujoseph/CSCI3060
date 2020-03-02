#include <iostream>
#include <string>
#include <vector>
#include "getInput.h"
#include "user.h"
#include "FileReader.h"
#include "transactionFileWriter.h"
#include "session.h"
#if(_DEBUG)
#include "main.h"
#include <csetjmp>
//eofbit should never be set when a user is using the program, but 
//since the tests send a file through cin, they set the eofbit.
//So this makes it so that if they set it, the program exits
#define checkTestEnd if (cin.peek() == EOF) { longjmp(testExit, 1); }
#endif

using namespace std;

//logs out of session
void session::logout() {


	string transaction;
	transaction += "00 ";
	transaction += userObject->getUsername();
	transaction += " ";
	transaction += userObject->getUserTypeAsString();
	transaction += " ";
	transaction += pad(userObject->getCreditAsString(), 9, '0', 'r');
	transactionFileWriter::add(transaction);
	transactionFileWriter::writeOut();
}

void session::advertise() {
	if ((userObject->getUserType() & (user::ADMIN | user::FULL_STANDARD | user::SELL_STANDARD)) != userObject->getUserType()) {
		cout << "Error: You Do Not Have Privileges To Perform This Transaction" << endl;
		return;
	}
	string itemName = getInputWithSpaces("Enter Item Name: ", "Error: Invalid Name", 25);
	string minBid = getMonetaryInputAsString("Enter Minimum Bid: ", [](string input) {
		double val = stod(input);
		if (val < 0) {
			cout << "Error: Minimum Bid Cannot Be Negative" << endl;
			return false;
		}
		else if (val > 999.99) {
			cout << "Error: Maximum Price For Item Is 999.99" << endl;
			return false;
		}
		return true;
	});
	string period;
	bool validPeriod = false;
	while (!validPeriod) {
		cout << "Enter Number Of Days Until Auction Ends: ";
#if(_DEBUG)
		checkTestEnd
#endif
		cin >> period;
		if (cin.peek() != '\n') {
			cout << "Error: Invalid Input" << endl;
		}
		else if (stoi(period) < 0) { //assume days until auction starts when item becomes available
			cout << "Error>: Number Of Days Until Auction Ends Cannot Be Negative" << endl;
		}
		else if (stoi(period) > 100) {
			cout << "Error: Maximum Number Of Days Until Auction Ends Is 100" << endl;
		}
		else {
			validPeriod = true;
		}
	}

	string transaction;
	transaction += "03 ";
	transaction += pad(itemName, 25, ' ', 'l');
	transaction += " ";
	transaction += userObject->getUsername();
	transaction += " ";
	transaction += pad(period, 3, '0', 'r');
	transaction += pad(minBid, 6, '0', 'r');
	transactionFileWriter::add(transaction);
}

void session::bid() {
	
	// bid item
	// check user types
	if ((userObject->getUserType() & (user::ADMIN | user::FULL_STANDARD)) != userObject->getUserType()) {
		cout << "Error: You Do Not Have Privileges To Perform This Transaction" << endl;
		return;
	}


	string itemName = getInputWithSpaces("Enter Item: ", "Error: Invalid Name", 25);
	string itemPrice = "";
	vector<string> currentAvailableItems = FileReader::getAvailableItems();
	for (int i = 0; i < currentAvailableItems.size() - 1; i++) {
		string& line = currentAvailableItems[i];
		if (line.substr(0, 21).compare(itemName) == 0) {
			//got item name
			//then get item price
			itemPrice = line.substr(57,63);
		}
	}


	string sellerName = getInputWithSpaces("Enter Seller Username: ", "Error: Invalid Name", 15);




	double preprice = 200.00; // temp value, need to get item price.
	string minBid = getMonetaryInputAsString("Enter Bid: ", [preprice](string input) {
		double val = stod(input);
		if (val < 0) {
			cout << "Error: Minimum Bid Cannot Be Negative" << endl;
			return false;
		}
		else if (val < (preprice + (preprice * 0.05))) {
			// val must be higher by 5% the prev bid
		}
		return true;


		});



	string transaction;
	transaction += "04 ";
	transaction += pad(itemName, 25, ' ', 'l');
	transaction += " ";
	transaction += sellerName;
	transaction += " ";
	transaction += userObject->getUsername();
	transaction += " ";
	transaction += pad(minBid, 6, '0', 'r');
	transactionFileWriter::add(transaction);
}

void session::create() {
	if ((userObject->getUserType() & (user::ADMIN)) != userObject->getUserType()) {
		cout << "Error: You Do Not Have Privileges To Perform This Transaction" << endl;
		return;
	}
	string newUsername;
	bool validName = false;
	while (!validName) {
		newUsername = getInputWithSpaces("Enter Username For New User: ", "Error: Invalid Name", 15);
		newUsername = pad(newUsername, 15, ' ', 'l');
		vector<string> currentUserAccounts = FileReader::getCurrentUserAccounts();
		bool cont = false;
		for (int i = 0; i < currentUserAccounts.size() - 1; i++) {
			string& line = currentUserAccounts[i];
			if (line.substr(0, 15).compare(newUsername) == 0) {
				cout << "Error: User With That Name Already Exists" << endl;
				cont = true;
				break;
			}
		}
		if (cont) continue;
		validName = true;
		break;
	}

	cout << "Enter Type For New User: ";
	string newUserType;
	while (true) {
#if(_DEBUG)
		checkTestEnd
#endif
		cin >> newUserType;
		if (cin.peek() != '\n') {
			cout << "Error: Invalid Input" << endl;
		}
		else if (newUserType.compare("admin") == 0) {
			newUserType = "AA";
			break;
		}
		else if (newUserType.compare("full-standard") == 0) {
			newUserType = "FS";
			break;
		}
		else if (newUserType.compare("buy-standard") == 0) {
			newUserType = "BS";
			break;
		}
		else if (newUserType.compare("sell-standard") == 0) {
			newUserType = "SS";
			break;
		}
		else {
			cout << "Error: New User Type Must Be One Of (admin, full-standard, buy-standard, sell-standard)" << endl;
		}
	}

	string transaction;
	transaction += "01 ";
	transaction += newUsername;
	transaction += " ";
	transaction += newUserType;
	transaction += " ";
	transaction += pad("", 9, '0', 'r');
	transactionFileWriter::add(transaction);
}

void session::addCredit() {
	//only allows admin or full standard users to add credit
	if ((userObject->getUserType() & (user::ADMIN | user::FULL_STANDARD)) != userObject->getUserType()) {
		cout << "Error: You Do Not Have Privileges To Perform This Transaction" << endl;
		return;
	}


	//path for if user is admin type is different than if user is full standard
	//admin path first
	if ((userObject->getUserType() & (user::ADMIN)) == userObject->getUserType()) {
		user* userToAddTo = NULL;
		bool validUser = false;
		while (!validUser) {
			string userName = getInputWithSpaces("Enter User Name To Add Credit To: ", "Error: Invalid User Name", 15);
			userName = pad(userName, 15, ' ', 'l');
			vector<string> currentUserAccounts = FileReader::getCurrentUserAccounts();

			for (int i = 0; i < currentUserAccounts.size() - 1; i++) {
				string& line = currentUserAccounts[i];
				if (line.substr(0, 15).compare(userName) == 0) {
					userToAddTo = new user(line.substr(0, 15), line.substr(16, 2), line.substr(19, 9));
					validUser = true;
					break;
				}
			}
			if (!validUser) {
				cout << "Error: No User With That Name Exists" << endl;
			}
		}
		while (true) {
			double creditAmount = getMonetaryInput("Enter Amount To Add: ", [](string input) {
				double val = stod(input);
				if (val < 0) {
					cout << "Error: Cannot Add Negative Credit" << endl;
					return false;
				}
				else if (val > 1000.00) {
					cout << "Error: Cannot Add More Than 1000.00 Credit" << endl;
					return false;
				}
				return true;
			});
			if (creditAmount + userToAddTo->getCredit() > 999999.0) {
				cout << "Error: Cannot Have More Than 999,999.00 In Your Account." << endl;
				continue;
			}
			for (user* trackedUser : userCreditTracker) {
				if (trackedUser->getUsername().compare(userToAddTo->getUsername()) == 0) {
					if (trackedUser->getCredit() + creditAmount - trackedUser->getStartCredit() > 1000) {
						cout << "Error: Cannot Add More Than 1000.00 Credit To A User In A Single Session" << endl;
						continue;
					}
				}
			}
			userToAddTo->addCredit(creditAmount);
			userCreditTracker.push_back(userToAddTo);
			break;
			
		}

		//creates the transaction line and sends it to writer
		string transaction;
		transaction += "06 ";
		transaction += userToAddTo->getUsername();
		transaction += " ";
		transaction += "AA";
		transaction += " ";
		transaction += pad(userToAddTo->getCreditAsString(), 9, '0', 'r');
		transactionFileWriter::add(transaction);
		transactionFileWriter::writeOut();

		return;
	}


	//Full standard user path
	if ((userObject->getUserType() & (user::FULL_STANDARD)) == userObject->getUserType()) {
		while (true) {
			double creditAmount = getMonetaryInput("Enter Amount To Add To Account: ", [](string input) {
				double val = stod(input);
				if (val < 0) {
					cout << "Error: Cannot add negative credit" << endl;
					return false;
				}
				else if (val > 1000.00) {
					cout << "Error: Maximum Amount To Add Is 1000.00" << endl;
					return false;
				}
				return true;
			});
			if (creditAmount + userObject->getCredit() > 999999.0) {
				cout << "Error: Cannot Have More Than 999,999.00 In Your Account." << endl;
				continue;
			}
			else if (userObject->getCredit() + creditAmount - userObject->getStartCredit() > 1000) {
				cout << "Error: Cannot Add More Than 1000.00 Credit To Your Account In A Single Session" << endl;
				continue;
			}
			else {
				userObject->addCredit(creditAmount);
				break;
			}
		}

		//creates the transaction line and sends it to the writer
		string transaction;
		transaction += "06 ";
		transaction += userObject->getUsername();
		transaction += " ";
		transaction += "FS";
		transaction += " ";
		transaction += pad(userObject->getCreditAsString(), 9, '0', 'r');
		transactionFileWriter::add(transaction);
		transactionFileWriter::writeOut();
		return;
	}

	return;
}

void session::refund() {
	// privileged transaction done by admins only
	if ((userObject->getUserType() & (user::ADMIN)) != userObject->getUserType()) {
		cout << "Error: You Do Not Have Privileges To Perform This Transaction" << endl;
		return;
	}
	user* buyerObject = NULL;
	user* sellerObject = NULL;
	//buyer exists
	string buyerUsername = getInputWithSpaces("Enter Buyer Username: ", "Error: Invalid Username", 15);
	vector<string> currentUserAccounts = FileReader::getCurrentUserAccounts();
	for (int i = 0; i < currentUserAccounts.size() - 1; i++) {
		string& line = currentUserAccounts[i];
		if (line.substr(0, 15).compare(buyerUsername) == 0) {
			buyerObject = new user(line.substr(0, 15), line.substr(16, 2), line.substr(19, 9));
		}
	}
	//seller exists
	string sellerUsername = getInputWithSpaces("Enter Seller Username: ", "Error: Invalid Username", 15);
	currentUserAccounts = FileReader::getCurrentUserAccounts();
	for (int i = 0; i < currentUserAccounts.size() - 1; i++) {
		string& line = currentUserAccounts[i];
		if (line.substr(0, 15).compare(sellerUsername) == 0) {
			sellerObject = new user(line.substr(0, 15), line.substr(16, 2), line.substr(19, 9));
		}
	}
	// checks correct bid amount
	// refund doesn't have a limit
	auto abc = [sellerUsername](string input) {
		return false;
	};
	string refund = getMonetaryInputAsString("Enter Minimum Bid: ", [sellerObject](string input) {
		double val = stod(input);
		if (val < 0) {
			cout << "Error: Minimum Refund Cannot Be Negative" << endl;
			return false;
		}
		else if (val >= (sellerObject->getCredit())) {
			cout << "Error: Seller's Credit is too low for this refund" << endl;
		}
		return true;
	});
	//send to transaction
	string transaction;
	transaction += "05 ";
	transaction += buyerObject->getUsername();
	transaction += " ";
	transaction += sellerObject->getUsername();
	transaction += " ";
	transaction += pad(refund, 9, '0', 'r');
	transactionFileWriter::add(transaction);
}

//deletes user
void session::deleteUser() {
	//only allows admins to delete users
	if ((userObject->getUserType() & (user::ADMIN)) != userObject->getUserType()) {
		cout << "Error: You Do Not Have Privileges To Perform This Transaction" << endl;
		return;
	}

	//gets user to be deleted
	string deletedUser = getInputWithSpaces("Enter Username of User to Delete: ", "Error: Invalid Username", 15);


	//send to transaction
	string transaction;
	transaction += "02 ";
	transaction += deletedUser;
	transaction += " ";
	transaction += "AA";
	transaction += " ";
	transaction += "000000000";
	transactionFileWriter::add(transaction);


}

/*
reads the userAccounts file and finds the user details with the
given account name
if name is found in userAccounts file, instantiates userObject
and returns a new session
if name is not found returns null
*/
session* session::login() {
	string username = getInputWithSpaces("Enter Username: ", "Error: Invalid Username", 15);
	username = pad(username, 15, ' ', 'l');
	vector<string> currentUserAccounts = FileReader::getCurrentUserAccounts();
	
	for (int i = 0; i < currentUserAccounts.size() - 1; i++) {
		string& line = currentUserAccounts[i];
		if (line.substr(0, 15).compare(username) == 0) {
			return new session(new user(line.substr(0, 15), line.substr(16, 2), line.substr(19, 9)));
		}
	}
	return NULL;
}

void session::sessionLoop() {
	string command;
	while (true) {
#if(_DEBUG)
		checkTestEnd
#endif
		command = getInputWithSpaces("", "Error: Invalid Input", 20);

		if (command.compare("login") == 0) {
			cout << "Error: Already Logged In" << endl;
		}
		else if (command.compare("logout") == 0) {
			logout();
			break;
		}
		else if (command.compare("advertise") == 0) {
			advertise();
		}
		else if (command.compare("bid") == 0) {
			bid();
		}
		else if (command.compare("create") == 0) {
			create();
		}
		else if (command.compare("addcredit") == 0) {
			addCredit();
		}
		else if (command.compare("refund") == 0) {
			refund();
		}
		else if (command.compare("delete") == 0) {
			deleteUser();
		}
		else {
			cout << "Error: Invalid Command" << endl;
		}
	}
}

session::session(user* userObject) {
	this->userObject = userObject;
}

session::~session() {
	for (user* user : userCreditTracker) {
		delete user;
	}
	delete userObject;
}