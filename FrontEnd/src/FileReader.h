#pragma once
#include <condition_variable>
#include <mutex>
#include <vector>
#include <string>

class FileReader {
private:
	static vector<string> currentUserAccounts;
	static vector<string> availableItems;
	static mutex m;
	static condition_variable cv;
	static const unique_lock<mutex> lk;

public:
	static void run();

	static vector<string> getCurrentUserAccounts();

	static vector<string> getAvailableItems();
};