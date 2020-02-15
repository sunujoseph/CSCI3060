#include <iostream>
#include <string>
#include <fstream>
#include <thread>

using namespace std;

int main() {
	string command;
	cin >> command;
	while (command.compare("login") != 0) {
		cout << "Error: Not Logged In" << endl;
		cin >> command;
	}
	thread fileWriterThread(transactionFileWriter::start);
	if (session::login()) {
		session newSession();
	}
	return 0;
}