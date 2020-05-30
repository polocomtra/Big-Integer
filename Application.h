
#include <fstream>
#include <string>
#include "QInt.h"

using namespace std;
class Application{
private:
    fstream inFile;
    fstream outFile;
    
    string readLine();
    void writeLine(string line);
    void solve(string request);
    
public:
    Application();
    ~Application();
    
    void start();
};
