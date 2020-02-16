#include <fstream>
#include <vector>
#include <mutex>
#include <condition_variable>

using namespace std;

class transactionFileWriter {
private:
	static vector<string> dailyTransactionFile;
	static mutex m;
	static condition_variable cv;
	static unique_lock<mutex> lk;
public:
	static void run() {
		cv.wait(lk);

	}

	static void writeOut() {

	}
};