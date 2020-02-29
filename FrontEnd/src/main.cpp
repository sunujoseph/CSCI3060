#include <iostream>
#include <string>
#include <thread>
#include "session.h"
#include "transactionFileWriter.h"
#include "FileReader.h"
#include <direct.h>
#include <mutex>
#include <assert.h>
#if(_DEBUG)
#include "main.h"
#include <csetjmp>
std::jmp_buf testExit;
#endif

using namespace std;

int main() {
	//get locations of files
	char currentPath[FILENAME_MAX];
	_getcwd(currentPath, sizeof(currentPath));

#if(_DEBUG)
	int slashes = 0;
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
#if(_DEBUG)
	if (_setjmp(testExit) == 1) {
		goto testExitL;
	}
#endif
	if ((newSession = session::login()) != NULL) {
		//cout << "login successful" << endl;
		newSession->sessionLoop();
	}
	if (newSession != NULL) {
		delete newSession;
	}
#if(_DEBUG)
testExitL:
#endif
	transactionFileWriter::shutdown();
	fileReaderThread.join();
	fileWriterThread.join();
	return 0;
}