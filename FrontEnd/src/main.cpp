#include <iostream>
#include <string>
#include <thread>
#include "session.h"
#include "transactionFileWriter.h"
#include "FileReader.h"
#include <direct.h>
#include <mutex>
#include <assert.h>

using namespace std;

int main() {
	//get locations of files
	char currentPath[FILENAME_MAX];
	_getcwd(currentPath, sizeof(currentPath));

#if(_DEBUG)
	int slashes = 2;
	for (int i = FILENAME_MAX; i >= 0; i--) {
		if (currentPath[i] == '\\') {
			if (slashes == 0) {
				const char testPath[] = "test_documents\\FrontEnd\\";
				for (int j = 0; j < sizeof(testPath) / sizeof(char); j++) {
					currentPath[i + j + 1] = testPath[j];
				}
				break;
			}
			slashes--;
		}
		currentPath[i] = -52;
	}
#endif
	string filePath(currentPath);
	cout << filePath << endl;
	transactionFileWriter::setPath(filePath);
	thread fileReaderThread(FileReader::run, filePath);
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
	if (newSession != NULL) {
		delete newSession;
	}

	transactionFileWriter::shutdown();
	fileReaderThread.join();
	fileWriterThread.join();
	return 0;
}