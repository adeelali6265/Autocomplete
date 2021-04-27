#include <iostream>

using namespace std;

int main() {
	string s = "tea";
	string temp = s.substr(0, 0); //originally j+1
	if (temp == "") {
		cout << "my theory is correct." << endl;
	}
	
	return 0;
}
