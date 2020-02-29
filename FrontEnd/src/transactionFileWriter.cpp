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
//condition_variable transactionFileWriter::cv;
condition_variable transactionFileWriter::midnightCV;
unique_lock<mutex> transactionFileWriter::lk(m, defer_lock);
string transactionFileWriter::filePath;
bool transactionFileWriter::shutdownF = false;
mutex transactionFileWriter::shutdownM;

//waits until midnight, then writes out dailyTransactionFile to a file
//if shutdownLock is obtained, then function returns
void transactionFileWriter::run() {
	//lk = unique_lock<mutex>(m);
	//filePath = path;
	//lk.unlock();
	unique_lock<mutex> shutdownLock(shutdownM);
	while (true) {
		//setup timer for midnight
		tm midnightTime;
		time_t curTime = time(nullptr); //curTime in seconds
		localtime_s(&midnightTime, &curTime);
		tm fileNameTime = midnightTime;
		midnightTime.tm_sec = 0;
		midnightTime.tm_min = 0;
		midnightTime.tm_hour = 0;
		midnightTime.tm_mday++; //mktime will adjust time accordingly if tm_mday is out of range
		if (midnightCV.wait_until(shutdownLock, chrono::system_clock::from_time_t(mktime(&midnightTime)), [] {return shutdownF;})) {
			shutdownLock.unlock();
			return;
		}
		//at this point, it is midnight, so obtain the lock and write out the dailyTransactionFile
		lk.lock();
		/*
		string fileName(filePath + to_string(fileNameTime.tm_mon) + "-" + to_string(fileNameTime.tm_mday) + "-" +
			to_string(fileNameTime.tm_year) + ".txt");
		ofstream dTFWriter(fileName);
		*/
#if(_DEBUG)
		ofstream dTFWriter(filePath);
#else
		/*
		store transaction file in folder with name of current date, stored
		in folder called Daily Transaction Files which is located at the
		diretory given for the daily transaction file filename
		*/
#endif
		ostream_iterator<string> out_it(dTFWriter, "\n");
		copy(dailyTransactionFile.begin(), dailyTransactionFile.end(), out_it);
		dTFWriter.close();
		lk.unlock();
	}
}

//forced write out of dailyTransactionFile, called in session::logout
void transactionFileWriter::writeOut() {
	tm fileNameTime;
	time_t curTime = time(nullptr); //curTime in seconds
	localtime_s(&fileNameTime, &curTime);

	lk.lock();
	/*
	string fileName(filePath + to_string(fileNameTime.tm_mon) + "-" + to_string(fileNameTime.tm_mday) + "-" +
		to_string(fileNameTime.tm_year) + ".txt");
	ofstream dTFWriter(fileName);
	*/
#if(_DEBUG)
	ofstream dTFWriter(filePath);
#else
	/*
	store transaction file in folder with name of current date, stored
	in folder called Daily Transaction Files which is located at the
	diretory given for the daily transaction file filename
	*/
#endif
	ostream_iterator<string> out_it(dTFWriter, "\n");
	copy(dailyTransactionFile.begin(), dailyTransactionFile.end(), out_it);
	dTFWriter.close();
	lk.unlock();
}

//add transaction to the daily transaction file
void transactionFileWriter::add(string transaction) {
	lk.lock();
	dailyTransactionFile.push_back(transaction);
	lk.unlock();
}

void transactionFileWriter::shutdown() {
	shutdownF = true;
	midnightCV.notify_one();
}

void transactionFileWriter::setPath(string path) {
	filePath = path;
}