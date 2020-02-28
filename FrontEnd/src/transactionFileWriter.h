#pragma once
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>

class transactionFileWriter {
private:
	static std::vector<std::string> dailyTransactionFile;
	static std::mutex m;
	//static std::condition_variable cv;
	static std::condition_variable midnightCV;
	static std::unique_lock<std::mutex> lk;
	static std::string filePath;
	static bool shutdownF;
	static std::mutex shutdownM;

public:
	static void run();
	static void writeOut();
	static void add(std::string transaction);
	static void shutdown();
	static void setPath(std::string path);
};