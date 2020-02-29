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
	return this->back();
}

vecRef<string> FileReader::currentUserAccounts;
vecRef<string> FileReader::availableItems;
mutex FileReader::m;
condition_variable FileReader::cv;
//unique_lock<mutex> FileReader::lk(m, defer_lock);
bool FileReader::initialized = false;

void FileReader::run(vector<string> paths) {
	unique_lock<mutex> lk = unique_lock<mutex>(m);
	//lk.lock();
	ifstream userAccountsReader(paths[0]);
	//istream_iterator<string> startCUA(userAccountsReader), endCUA;
	//currentUserAccounts = vector<string>(startCUA, endCUA);
	while (userAccountsReader.peek() != EOF) {
		getline(userAccountsReader, currentUserAccounts.addBack());
	}
	userAccountsReader.close();

	ifstream availableItemsReader(paths[1]);
	//istream_iterator<string> startAI(availableItemsReader), endAI;
	//availableItems = vector<string>(startAI, endAI);
	while (!availableItemsReader.eof()) {
		getline(availableItemsReader, availableItems.addBack());
	}
	availableItemsReader.close();
	initialized = true;
	lk.unlock();
	cv.notify_all();
}

vector<string> FileReader::getCurrentUserAccounts() {
	unique_lock<mutex> lk = unique_lock<mutex>(m);
	if (!initialized) {
		cv.wait(lk, [] {return initialized;});
	}
	vector<string> ret = currentUserAccounts;
	lk.unlock();
	return ret;
}

vector<string> FileReader::getAvailableItems() {
	unique_lock<mutex> lk = unique_lock<mutex>(m);
	if (!initialized) {
		cv.wait(lk, [] {return initialized;});
	}
	vector<string> ret = availableItems;
	lk.unlock();
	return ret;
}