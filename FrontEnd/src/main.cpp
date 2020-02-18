#include <iostream>
#include <string>
#include <thread>
#include "session.h"
#include "transactionFileWriter.h"
#include "FileReader.h"
#include <direct.h>

using namespace std;

int main() {
	//get locations of files
	char currentPath[FILENAME_MAX];
	_getcwd(currentPath, sizeof(currentPath));
	string newPath(currentPath);
	string userPath;

	thread fileReaderThread(FileReader::run);
	thread fileWriterThread(transactionFileWriter::run);

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
	delete newSession;
	return 0;
}