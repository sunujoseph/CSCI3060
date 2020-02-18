#include <fstream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <string>
#include <chrono>
#include <ctime>
#include <iterator>
#include "transactionFileWriter.h"

using namespace std;

vector<string> transactionFileWriter::dailyTransactionFile;
mutex transactionFileWriter::m;
mutex transactionFileWriter::midnightM;
condition_variable transactionFileWriter::cv;
condition_variable transactionFileWriter::midnightCV;
unique_lock<mutex> transactionFileWriter::lk = unique_lock<mutex>(m);
//only used as blank arg for cv.wait_until
unique_lock<mutex> transactionFileWriter::midnightLock = unique_lock<mutex>(midnightM);

//waits until midnight, then writes out dailyTransactionFile to a file
void transactionFileWriter::run() {
	//setup timer for midnight
	tm midnightTime;
	time_t curTime = time(nullptr); //curTime in seconds
	localtime_s(&midnightTime, &curTime);
	tm fileNameTime = midnightTime;
	midnightTime.tm_sec = 0;
	midnightTime.tm_min = 0;
	midnightTime.tm_hour = 0;
	midnightTime.tm_mday++; //mktime will adjust time accordingly if tm_mday is out of range
	midnightCV.wait_until(midnightLock, chrono::system_clock::from_time_t(mktime(&midnightTime)));
	//at this point, it is midnight, so obtain the lock and write out the dailyTransactionFile
	if (!lk.try_lock()) {
		cv.wait(lk);
	}
	string fileName(to_string(fileNameTime.tm_mon) + "-" + to_string(fileNameTime.tm_mday) + "-" +
		to_string(fileNameTime.tm_year) + ".txt");
	ofstream dTFWriter(fileName);
	ostream_iterator<string> out_it(dTFWriter, "\n");
	copy(dailyTransactionFile.begin(), dailyTransactionFile.end(), out_it);
	dTFWriter.close();
	lk.unlock();
	cv.notify_all();
	midnightLock.unlock();
}

//forced write out of dailyTransactionFile, called in session::logout
void transactionFileWriter::writeOut() {
	tm fileNameTime;
	time_t curTime = time(nullptr); //curTime in seconds
	localtime_s(&fileNameTime, &curTime);

	if (!lk.try_lock()) {
		cv.wait(lk);
	}
	string fileName(to_string(fileNameTime.tm_mon) + "-" + to_string(fileNameTime.tm_mday) + "-" +
		to_string(fileNameTime.tm_year) + ".txt");
	ofstream dTFWriter(fileName);
	ostream_iterator<string> out_it(dTFWriter, "\n");
	copy(dailyTransactionFile.begin(), dailyTransactionFile.end(), out_it);
	dTFWriter.close();
	lk.unlock();
	cv.notify_all();
}

//add transaction to the daily transaction file
void transactionFileWriter::add(string transaction) {
	if (!lk.try_lock()) {
		cv.wait(lk);
	}
	dailyTransactionFile.push_back(transaction);
	lk.unlock();
	cv.notify_all();
}