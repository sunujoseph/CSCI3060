#pragma once
#include <condition_variable>
#include <mutex>
#include <vector>
#include <string>

template <typename T>
class vecRef : public std::vector<T> {
public:
	T& addBack();
};

class FileReader {
private:
	static vecRef<std::string> currentUserAccounts;
	static vecRef<std::string> availableItems;
	static std::mutex m;
	static std::condition_variable cv;
	static std::unique_lock<std::mutex> lk;
	static bool initialized;

public:
	static void run(std::string path);

	static std::vector<std::string> getCurrentUserAccounts();

	static std::vector<std::string> getAvailableItems();
};