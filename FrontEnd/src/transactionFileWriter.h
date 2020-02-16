#pragma once
#include <mutex>
#include <condition_variable>
#include <vector>

class transactionFileWriter {
private:
	static vector<string> dailyTransactionFile;
	static mutex m;
	static condition_variable cv;
	static unique_lock<mutex> lk;
public:
	static void run();
	static void writeOut();
};