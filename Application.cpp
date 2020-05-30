
#include "Application.h"
#include <iostream>
#include <sstream>

Application::Application(){
    inFile = fstream("input.txt", ios::in);
    outFile = fstream("output.txt",ios::out);
}

Application::~Application(){
    inFile.close();
    outFile.close();
}

string Application::readLine(){
    string line = "";
    getline(inFile, line);
    return line;
}

void Application::writeLine(string line){
    outFile << line << endl;
}

void Application::solve(string req){
    stringstream inStream(req);
    string p1 = "";
    getline(inStream, p1, ' ');
    string p2 = "";
    getline(inStream, p2, ' ');
    
    if (p2 == "2" | p2 == "10" | p2 == "16") {
        //convert base p1 into base p2
        string p3 = "";
		
        getline(inStream, p3, ' ');
        p3.erase(p3.end() - 1);
        QInt a(p3, p1);
		
        writeLine(a.toString(p2));
    } else if (p2 == "~") {
        // operator ~
        writeLine("*****");
    } else if (p2 == "^") {
		//operator ^
        writeLine("*****");
    } else if (p2 == "ror") {
        //operator ror
        string p3 = "";
        getline(inStream, p3, ' ');
        p3.erase(p3.end() - 1);
        QInt a(p3, p1);
        a.ror();
        writeLine(a.toString(p1));
    } else if (p2 == "rol") {
        //operator rol
        string p3 = "";
        getline(inStream, p3, ' ');
        p3.erase(p3.end() - 1);
        QInt a(p3, p1);
        a.rol();
        writeLine(a.toString(p1));
    } else {
        //operator + - * / >> <<
        string p3 = "";
        string p4 = "";
        getline(inStream, p3, ' ');
        getline(inStream, p4, ' ');
        p4.erase(p4.end() - 1);
        if (p3 == "<<") {
            //
            writeLine("*****");
        } else if (p3 == ">>") {
            //
            writeLine("*****");
        } else {
            QInt a(p2,p1);
            QInt b(p4,p1);
            QInt result;
            if (p3 == "+") result = a+b;
            else if (p3 == "-") result = a-b;
            else if (p3 == "*") result = a*b;
            else if (p3 == "/") result = a/b;
            
            writeLine(result.toString(p1));

        }
    } 
}

void Application::start(){
	
	if(inFile == NULL || outFile == NULL){
		cout << "File not found" << endl;
		return;
	}
	
    string req = readLine();
	
    while (!req.empty()) {
        solve(req);
        req = readLine();
    }
}

