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
	int slashes = 1;
	for (int i = FILENAME_MAX; i >= 0; i--) {
		if (currentPath[i] == '\\') {
			if (slashes == 0) {
				currentPath[i + 1] = 's';
				currentPath[i + 2] = 'r';
				currentPath[i + 3] = 'c';
				currentPath[i + 4] = '\\';
				currentPath[i + 5] = '\0';
				break;
			}
			slashes--;
		}
		currentPath[i] = -52;
	}
	string filePath(currentPath);

	thread fileReaderThread(FileReader::run, filePath);
	thread fileWriterThread(transactionFileWriter::run, filePath);

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
	if (newSession != NULL) {
		delete newSession;
	}
	return 0;
}