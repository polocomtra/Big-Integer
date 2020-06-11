#include "Application.h"

//int main(char argc, char* argv[]) {
//	
//	
//	Application app(argv[1], argv[2]);
//	app.start();
//	return 0;
//
//}

int main() {
	//10 -4647 * 4302371896916613314517792614108589
	QInt q1("-4647", "10");
	QInt q2("4302371896916613314517792614108589", "10");
	QInt q3;
	q3 = q1 * q2;
	cout << q3.toDec();
	return 0;
}