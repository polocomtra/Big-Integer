#include "Application.h"

int main(char argc, char* argv[]) {
	Application app(argv[1], argv[2]);
	app.start();
	return 0;

}