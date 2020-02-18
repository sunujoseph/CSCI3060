#pragma once
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>

class transactionFileWriter {
private:
	static std::vector<std::string> dailyTransactionFile;
	static std::mutex m;
	static std::mutex midnightM;
	static std::condition_variable cv;
	static std::condition_variable midnightCV;
	static std::unique_lock<std::mutex> midnightLock;
	static std::unique_lock<std::mutex> lk;
	static std::string filePath;

public:
	static void run(std::string path);
	static void writeOut();
	static void add(std::string transaction);
};