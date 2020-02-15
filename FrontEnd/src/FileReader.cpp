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
		istream_iterator<string> startCUA(userAccountsReader), endCUA;
		currentUserAccounts = vector<string>(startCUA, endCUA);
		userAccountsReader.close();

		ifstream availableItemsReader("available items.txt");
		istream_iterator<string> startAI(availableItemsReader), endAI;
		availableItems = vector<string>(startAI, endAI);
		availableItemsReader.close();
		cv.notify_one();
	}
	
};

unique_lock<mutex> FileReader::lk = unique_lock<mutex>(FileReader::m);