#include "Application.h"

int main(char argc, char* argv[]) {
	/*
	Application app(argv[1], argv[2]);
	app.start();
	return 0;*/

	QInt a("100", "10");
	cout << a.toBin() << endl;
	cout << a.toDec() << endl;
}