#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include "session.h"
#include "user.h"
#include "transactionFileWriter.h"
#include "FileReader.h"

using namespace std;

int main() {
	thread fileWriterThread(transactionFileWriter::start);
	thread fileReaderThread(FileReader::start);
	while (!FileReader::ready()) {

	}
	string command;
	cin >> command;
	while (command.compare("login") != 0) {
		cout << "Error: Not Logged In" << endl;
		cin >> command;
	}
	session* newSession;
	if ((newSession = session::login()) != NULL) {
		newSession->sessionLoop();
	}
	return 0;
}