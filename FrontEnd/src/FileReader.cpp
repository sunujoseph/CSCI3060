#include <fstream>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <iterator>

using namespace std;

class FileReader {
private:
	static vector<string> currentUserAccounts;
	static vector<string> availableItems;
	static mutex m;
	static condition_variable cv;
	static unique_lock<mutex> lk;

public:
	static void run() {
		lk.lock();
		ifstream userAccountsReader("current user accounts.txt");
		//istream_iterator<string> startCUA(userAccountsReader), endCUA;
		//currentUserAccounts = vector<string>(startCUA, endCUA);
		{
			char line[29]; //lines in UA file are 28 characters long, +1 for terminating null character
			userAccountsReader.getline(line, 29);
			while (!userAccountsReader.eof()) {
				userAccountsReader.getline(line, 29);
				currentUserAccounts.push_back(string(line));
			}
			userAccountsReader.close();
		}

		ifstream availableItemsReader("available items.txt");
		//istream_iterator<string> startAI(availableItemsReader), endAI;
		//availableItems = vector<string>(startAI, endAI);
		{
			char line[63]; //lines in AI file are 62 characters long, +1 for terminating null character
			availableItemsReader.getline(line, 63);
			while (!availableItemsReader.eof()) {
				availableItemsReader.getline(line, 63);
				availableItems.push_back(line);
			}
			availableItemsReader.close();
		}
		lk.unlock();
		cv.notify_all();
	}

	static vector<string>& getCurrentUserAccounts() {
		if (!lk.try_lock()) {
			cv.wait(lk);
		}
		vector<string> ret = currentUserAccounts;
		return ret;
	}

	static vector<string>& getAvailableItems() {
		if (!lk.try_lock()) {
			cv.wait(lk);
		}
		vector<string> ret = availableItems;
		return ret;
	}
	
};

unique_lock<mutex> FileReader::lk = unique_lock<mutex>(FileReader::m);