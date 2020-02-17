#pragma once
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>

class transactionFileWriter {
private:
	static vector<string> dailyTransactionFile;
	static mutex m;
	static mutex midnightM;
	static condition_variable cv;
	static condition_variable midnightCV;
	static unique_lock<mutex> midnightLock;
	static unique_lock<mutex> lk;
public:
	static void run();
	static void writeOut();
	static void add(string transaction);
};