#include <fstream>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <iterator>

using namespace std;

template <typename T>
class vecRef : public vector<T> {
public:
	T& addBack() { //increaes the size of the vector by 1, and returns a reference to the last element
		resize(length() + 1);
		return back();
	}
};

class FileReader {
private:
	static vecRef<string> currentUserAccounts;
	static vecRef<string> availableItems;
	static mutex m;
	static condition_variable cv;
	static unique_lock<mutex> lk;

public:
	static void run() {
		if (!lk.try_lock()) {
			cv.wait(lk);
		}
		ifstream userAccountsReader("current user accounts.txt");
		//istream_iterator<string> startCUA(userAccountsReader), endCUA;
		//currentUserAccounts = vector<string>(startCUA, endCUA);
		while (!userAccountsReader.peek() != EOF) {
			getline(userAccountsReader, currentUserAccounts.addBack());
		}
		userAccountsReader.close();

		ifstream availableItemsReader("available items.txt");
		//istream_iterator<string> startAI(availableItemsReader), endAI;
		//availableItems = vector<string>(startAI, endAI);
		while (!availableItemsReader.eof()) {
			getline(availableItemsReader, availableItems.addBack());
		}
		availableItemsReader.close();
		lk.unlock();
		cv.notify_all();
	}

	static vector<string>& getCurrentUserAccounts() {
		if (!lk.try_lock()) {
			cv.wait(lk);
		}
		vector<string> ret = currentUserAccounts;
		lk.unlock();
		cv.notify_all();
		return ret;
	}

	static vector<string>& getAvailableItems() {
		if (!lk.try_lock()) {
			cv.wait(lk);
		}
		vector<string> ret = availableItems;
		lk.unlock();
		cv.notify_all();
		return ret;
	}
	
};

unique_lock<mutex> FileReader::lk = unique_lock<mutex>(FileReader::m);