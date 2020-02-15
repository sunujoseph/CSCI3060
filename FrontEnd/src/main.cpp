#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include "Header.h"

using namespace std;

int main() {
	string command;
	cin >> command;
	while (command.compare("login") != 0) {
		cout << "Error: Not Logged In" << endl;
		cin >> command;
	}
	thread fileWriter()

	return 0;
}