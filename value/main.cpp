#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <windows.h>
#include <conio.h>

using namespace std;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

string x;

int value(string str) {
	stringstream ss(str);
	vector<vector<string>> arr(3);
	string line;
	while (ss >> line) arr[0].push_back(line);
//	for (string &s : arr[0]) {
//		cout << s << endl;
//		Sleep(500);
//	}
	int part;
	for (int i = 0; i < arr[0].size(); i++) {
		line = "";
		part = 1;
		for (int j = 0; j < arr[0][i].size(); j++) {
			line += arr[0][i][j];
			if (j >= arr[0][i].size() - 2) {
				arr[part++].push_back(line);
				line = "";
			}
		}
	}
	for (int i = 0; i < arr[1].size(); i++) {
		cout << arr[1][i] << " " << arr[2][i] << endl;
		Sleep(100);
	}

	vector<int> sameInt;
	vector<string> sameStr;
//	for (int i = 0; i < arr[1].size(); i++) {
//		for (int j = 0; j < arr[1].size(); j++) {
//			if (i != j) {
//				if (arr[1][i] == arr[1][j]) {
//					sameStr.push_back(arr[1][i]);
//					break;
//				}
//			}
//		}
//	}
	for (string s : arr[1]) sameStr.push_back(s);
	cout << endl;
	for (string s : arr[1]) cout << s << endl;

	for (int i = 0 ;; i++) {
		for (int j = 0; j < i; j++) {
			if (i != j && sameStr[i] == sameStr[j]) {
				sameStr.erase(sameStr.begin() + j);
				
			}
		}
		if (i == sameStr.size()) break;
	}
	cout << endl;
	for (int i = 0; i < sameStr.size(); i++) cout << sameStr[i] << " " << endl;
	
//	int max = -1;
//	for (int i = 0; i < sameInt.size(); i++) {
//		if (sameInt[i] > sameInt[max]) {
//			max = i;
//		}
//	}
//	cout << max;
//
//	if (sameInt[max] == 1) {
//		cout << "f";
//	}
//	else {
//		cout << "s";
//	}
}

int main() {
	cout << "Helloe\n\n";
	value("1D 1H 10C 9D 1D 10H");
	
//	for (int i = 0 ;; i++) {
//		for (int j = 0; j < 10; j++) printf("mmngh %d", i);
//		cout << endl;
//		SetConsoleTextAttribute(hConsole, i);
//		Sleep(10);
//	}
	
//    char key;
//    while (true) {
//        if (_kbhit()) { 
//            key = _getch(); 
//            if (key == 72) {  
//                cout << "Escape key pressed. Exiting..." << std::endl;
//                break;
//            } else {
//                cout << "You pressed the key with ASCII value " << static_cast<int>(key) << std::endl;
//            }
//        }
//    }
	return 0;
}
