#include <iostream>
#include <string>
#include <thread>
#include "getInput.h"
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
#define checkTestEnd if (cin.peek() == EOF) { longjmp(testExit, 1); }
#endif

using namespace std;

int main(int argc, char** argv) {
	
	/*
#if(_DEBUG)
	//get locations of files
	char currentPath[FILENAME_MAX];
	_getcwd(currentPath, sizeof(currentPath));
	int slashes = 0;
	int pathLen;
	for (int i = FILENAME_MAX; i >= 0; i--) {
		if (currentPath[i] == '\\') {
			if (slashes == 0) {
				const char testPath[] = "test_documents\\FrontEnd\\";
				int j;
				for (j = 0; j < sizeof(testPath) / sizeof(char); j++) {
					currentPath[i + j + 1] = testPath[j];

				}
				pathLen = i + j;
				break;
			}
			slashes--;
		}
		currentPath[i] = -52;
	}
	string filePath(currentPath);
	transactionFileWriter::setPath(filePath);

#else
	*/
	
	transactionFileWriter::setPath(argv[3]);
	thread fileReaderThread(FileReader::run, vector<string>{ argv[1], argv[2] });
	thread fileWriterThread(transactionFileWriter::run);

	string command;
#if(_DEBUG)
	if (_setjmp(testExit) == 1) {
		goto testExitL;
	}
#endif
	while (true) {
#if(_DEBUG)
		checkTestEnd
#endif
		command = getInputWithSpaces("", "Error: Invalid Input", 5);
		while (command.compare("login") != 0) {
			cout << "Error: Not Logged In" << endl;
#if(_DEBUG)
			checkTestEnd
#endif
			command = getInputWithSpaces("", "Error: Invalid Input", 5);
		}
		session* newSession;
		if ((newSession = session::login()) != NULL) {
			//cout << "login successful" << endl;
			newSession->sessionLoop();
		}
		if (newSession != NULL) {
			delete newSession;
		}
	}
#if(_DEBUG)
testExitL:
	transactionFileWriter::writeOut();
#endif
	transactionFileWriter::shutdown();
	fileReaderThread.join();
	fileWriterThread.join();
	return 0;
}