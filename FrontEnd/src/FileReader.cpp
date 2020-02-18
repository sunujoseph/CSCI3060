#include <fstream>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <iterator>
#include "FileReader.h"

using namespace std;

template <typename T>
T& vecRef<T>::addBack() { //increases the size of the vector by 1, and returns a reference to the last element
	this->resize(this->size() + 1);
	vector<int> abc;
	return this->back();
}

vecRef<string> FileReader::currentUserAccounts;
vecRef<string> FileReader::availableItems;
mutex FileReader::m;
condition_variable FileReader::cv;
unique_lock<mutex> FileReader::lk;
bool FileReader::initialized = false;

void FileReader::run(string path) {
	lk = unique_lock<mutex>(m);
	if (initialized && !lk.try_lock()) {
		cv.wait(lk);
	}
	else {
		initialized = true;
	}

	ifstream userAccountsReader(path + "current_user_accounts.txt");
	//istream_iterator<string> startCUA(userAccountsReader), endCUA;
	//currentUserAccounts = vector<string>(startCUA, endCUA);
	while (userAccountsReader.peek() != EOF) {
		getline(userAccountsReader, currentUserAccounts.addBack());
	}
	userAccountsReader.close();

	ifstream availableItemsReader(path + "available items.txt");
	//istream_iterator<string> startAI(availableItemsReader), endAI;
	//availableItems = vector<string>(startAI, endAI);
	while (!availableItemsReader.eof()) {
		getline(availableItemsReader, availableItems.addBack());
	}
	availableItemsReader.close();
	lk.unlock();
	cv.notify_all();
}

vector<string> FileReader::getCurrentUserAccounts() {
	if (!lk.try_lock()) {
		cv.wait(lk);
	}
	vector<string> ret = currentUserAccounts;
	lk.unlock();
	cv.notify_all();
	return ret;
}

vector<string> FileReader::getAvailableItems() {
	if (!lk.try_lock()) {
		cv.wait(lk);
	}
	vector<string> ret = availableItems;
	lk.unlock();
	cv.notify_all();
	return ret;
}