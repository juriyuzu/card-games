#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <cmath>
#include <Windows.h>
#include <fstream>
#include <limits>
using namespace std;

double cash = 1000;
string name = "username", x;
vector<string> names;
string suits[4] = {"C", "S", "H", "D"};
string faces[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
string stack[52] = {"2C", "2S", "2H", "2D", "3C", "3S", "3H", "3D", "4C", "4S", "4H", "4D", "5C", "5S", "5H", "5D", "6C", "6S", "6H", "6D", "7C", "7S", "7H", "7D", "8C", "8S", "8H", "8D", "9C", "9S", "9H", "9D", "10C", "10S", "10H", "10D", "JC", "JS", "JH", "JD", "QC", "QS", "QH", "QD", "KC", "KS", "KH", "KD", "AC", "AS", "AH", "AD"};
string combo[10] = {"High Card", "One Pair", "Two Pairs", "Triad", "Straight", "Flush", "Full House", "Four of a Kind", "Straight Flush", "Royal Straight Flush"};

string spacer(int n, string s = " ") {
    string ss = "";
	for (int i = 0; i < n; i++) ss += s;
    return ss;
}

int rng(int min, int max) {
    return min + (rand() % (max - min + 1));
}

int rngp(int arr[]) {
	vector<int> nums;
	cout << endl;
	for (int i = 0; i < sizeof(arr); i += 2) {
		for (int j = 0; j < arr[i + 1]; j++) {
			nums.push_back(arr[i]);
//			cout << arr[i] << " ";
		}
	}
	return nums[rng(0, nums.size() - 1)];
}

void gotoxy(int x, int y, bool abs = true) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD newPosition;
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	newPosition.X = x + (abs ? 0 : consoleInfo.dwCursorPosition.X);
	newPosition.Y = y + (abs ? 0 : consoleInfo.dwCursorPosition.Y);
	SetConsoleCursorPosition(hConsole, newPosition);
}

struct Cards {
    string cards[52] = {"2C", "2S", "2H", "2D", "3C", "3S", "3H", "3D", "4C", "4S", "4H", "4D", "5C", "5S", "5H", "5D", "6C", "6S", "6H", "6D", "7C", "7S", "7H", "7D", "8C", "8S", "8H", "8D", "9C", "9S", "9H", "9D", "10C", "10S", "10H", "10D", "JC", "JS", "JH", "JD", "QC", "QS", "QH", "QD", "KC", "KS", "KH", "KD", "AC", "AS", "AH", "AD"};
    string hold;
    vector<string> maxValue;
    int n = sizeof(cards) / sizeof(cards[0]);

    int highCardValue, maxDuplicateCardValue, maxStraightCardValue, maxFlushCardValue;
    int maxStraightCardIndex, maxStraightCount;
    vector<int> sortedFacesIndex,
        sortedFacesIndexValues,
        sortedSuitsIndex,
        sortedSuitsIndexValues;
    int maxFlushCount, maxFlushCardIndex;
    vector<int> duplicateFaceIndexArray, duplicateFacesCountArray;
    int maxDuplicateFacesCount, maxDuplicateFaceIndex, handValue;

    Cards(bool b = false) {
        if (b)
            clear();
    }

    void clear() {
        for (int i = 0; i < n; i++) cards[i] = "";
        n = 0;
        hold = "";
        maxValue.clear();
        highCardValue = 0, 
		maxDuplicateCardValue = 0, 
		maxStraightCardValue = 0, 
		maxFlushCardValue = 0;
        maxStraightCardIndex = 0, 
		maxStraightCount = 0;
        sortedFacesIndex.clear(),
        sortedFacesIndexValues.clear(),
        sortedSuitsIndex.clear(),
        sortedSuitsIndexValues.clear();
        maxFlushCount = 0, 
		maxFlushCardIndex = 0;
        duplicateFaceIndexArray.clear(), 
		duplicateFacesCountArray.clear();
        maxDuplicateFacesCount = 0, 
		maxDuplicateFaceIndex = 0, 
		handValue = 0;
    }

    string getCards() {
        hold = "";
        for (int i = 0; i < n; i++) {
            hold += cards[i];
            if (i != n - 1) hold += " ";
        }
        return hold;
    }

    void info() {
        //cout << "There are currently " << n << " cards.\n";
        for (int i = 0; i < n; i++) cout << cards[i] << " ";
//        cout << endl;
    }

    void toTop(int x) {
        hold = cards[x];
        for (int i = x; i < n - 1; i++) cards[i] = cards[i + 1];
        cards[n - 1] = hold;
    }

    void toBottom(int x) {
        hold = cards[x];
        for (int i = x; i > 0; i--) cards[i] = cards[i - 1];
        cards[0] = hold;
    }

    void shuffle() {
        for (int i = 0; i < 1000000; i++) {
            toTop(rng(0, n - 1));
            toBottom(rng(0, n - 1));
        }
    }

    string pop(bool b = false) {
        hold = cards[n - 1];
        if (b) toTop(0);
        cards[n - 1] = "";
        n--;
        return hold;
    }

    void drop(string x, bool b = false) {
        cards[n] = x;
        if (b)
            toBottom(n); 
        n++;
    }

    void refill() {
        clear();
        for (int i = 0; i < 52; i++) drop(stack[i]);
        n = sizeof(cards) / sizeof(cards[0]);
    }

    int valueToInt(string s, int c) {
        switch (c) {
        case 1:
            for (int i = 0; i < sizeof(stack); i++)
                if (stack[i] == s) return i;
            break;
        case 2:
            for (int i = 0; i < sizeof(faces); i++)
                if (faces[i] == s) return i;
            break;
        case 3:
            for (int i = 0; i < sizeof(suits); i++)
                if (suits[i] == s) return i;
        }

        return -1;
    }

    int value(string x = "") {
        sortedFacesIndex.clear();
        sortedFacesIndexValues.clear();
        sortedSuitsIndex.clear();
        sortedSuitsIndexValues.clear();
        duplicateFaceIndexArray.clear();
        duplicateFacesCountArray.clear();

        if (n == 0 && x == "") return -1;

//		combine this struct's and the parameter's cards and put it into arr[0]
        x = getCards() + " " + x;
        stringstream ss(x);
        vector<vector<string>> arr(3);
        string line;
        
        while (ss >> line) arr[0].push_back(line); 
        int nAll = arr[0].size();
//      cout << "arr[0]: " << arr[0].size() << "\n";
//      for (int i = 0; i < 2; i++) for (string s : arr[i]) cout << s << " ";

// split the strings to two parts: arr[1] for the faces and arr[2] for the suits
        line = "";
        for (int i = 0; i < nAll; i++) {
			int count = 0;
            for (int j = 0; j < arr[0][i].size(); j++) {
				line += arr[0][i][j];
                if (j >= arr[0][i].size() - 2) {
					arr[count == 0 ? 1 : 2].push_back(line);
                    line = "";
                    count++;
                }
            }
        }
//      cout << "\n\narr[1] and arr[2]:";
//     		for (int i = 1; i < 3; i++) {
//               cout << endl;
//               for (string s : arr[i]) cout << s << " ";
//          }
//  	cout << "\n\n" << spacer(50, '-');

// sort the indexes of arr[1] and put it in sortedFacesIndex
        for (int i = 0; i < nAll; i++) sortedFacesIndex.push_back(i);
//      cout << "\n\nsortedFacesIndex:\n";
//      for (int i = 0; i < nAll; i++) cout << sortedFacesIndex[i] << " ";
        for (int i = 0; i < nAll; i++) {
            for (int j = i + 1; j < nAll; j++) {
                if ((valueToInt(arr[1][sortedFacesIndex[j]], 2) > valueToInt(arr[1][sortedFacesIndex[i]], 2)) ||
                   ((valueToInt(arr[1][sortedFacesIndex[j]], 2) == valueToInt(arr[1][sortedFacesIndex[i]], 2)) &&
                    (valueToInt(arr[2][sortedFacesIndex[j]], 3) > valueToInt(arr[2][sortedFacesIndex[i]], 3)))) {
                    int temp = sortedFacesIndex[i];
                    sortedFacesIndex[i] = sortedFacesIndex[j];
                    sortedFacesIndex[j] = temp;
                }
            }
        }
        highCardValue = valueToInt(arr[0][sortedFacesIndex[0]], 1);
//      cout << "\n\nsortedFacesIndex:\n";
//      for (int i = 0; i < nAll; i++) cout << sortedFacesIndex[i] << " ";

// find maxStraightIndex
        for (int i = 0; i < nAll; i++) sortedFacesIndexValues.push_back(valueToInt(arr[1][(sortedFacesIndex[i])], 2));
//      cout << "\n\nsortedFacesIndexValues:\n";
//      for (int i = 0; i < nAll; i++) cout << sortedFacesIndexValues[i] << " ";
        int straightCount = 0, straightCardIndex = 0;
        maxStraightCount = 0, maxStraightCardIndex = 0;
        for (int i = 0; i < nAll; i++) {
            if (sortedFacesIndexValues[i] == sortedFacesIndexValues[i + 1] + 1) {
                straightCount++;
            }
            else if (sortedFacesIndexValues[i] != sortedFacesIndexValues[i + 1]) {
                if (maxStraightCount < straightCount) {
                    maxStraightCount = straightCount;
                    maxStraightCardIndex = straightCardIndex;
                }
                straightCardIndex = i + 1;
                straightCount = 0;
            }
        }
        maxStraightCardValue = valueToInt(arr[0][sortedFacesIndex[maxStraightCardIndex]], 1);
//      cout << "\n\nmaxStraightCount: " << maxStraightCount << "\nmaxStraightCardIndex: " << maxStraightCardIndex;
        vector<int> maxStraightIndex;
        int hold = maxStraightCardIndex;
        maxStraightIndex.push_back(hold);
        for (int i = maxStraightCardIndex; i < nAll; i++) {
            if (sortedFacesIndexValues[hold] - 1 == sortedFacesIndexValues[i + 1]) {
                maxStraightIndex.push_back(i + 1);
                hold = i + 1;
            }
        }
//      cout << "\n\nmaxStraightIndex:\n";
//      for (int i = 0; i < maxStraightCount + 1; i++) cout << maxStraightIndex[i] << " ";
//      cout << "\n\n" << spacer(50, '-');

// sort the indexes of arr[2] and put it in sortedSuitsIndex
        for (int i = 0; i < nAll; i++) sortedSuitsIndex.push_back(i);
        for (int i = 0; i < nAll; i++) {
            for (int j = i + 1; j < nAll; j++) {
                if ((valueToInt(arr[2][sortedSuitsIndex[j]], 3) > valueToInt(arr[2][sortedSuitsIndex[i]], 3)) ||
                   ((valueToInt(arr[2][sortedSuitsIndex[j]], 3) == valueToInt(arr[2][sortedSuitsIndex[i]], 3)) &&
                    (valueToInt(arr[1][sortedSuitsIndex[j]], 2) > valueToInt(arr[1][sortedSuitsIndex[i]], 2)))) {
                    int temp = sortedSuitsIndex[i];
                    sortedSuitsIndex[i] = sortedSuitsIndex[j];
                    sortedSuitsIndex[j] = temp;
                }
            }
        }
//      cout << "\n\nsortedSuitsIndex:\n";
//      for (int i = 0; i < nAll; i++) cout << sortedSuitsIndex[i] << " ";

// find maxFlushCardIndex
        for (int i = 0; i < nAll; i++) sortedSuitsIndexValues.push_back(valueToInt(arr[2][(sortedSuitsIndex[i])], 3));
//      cout << "\n\nsortedSuitsIndexValues:\n";
//      for (int i = 0; i < nAll; i++) cout << sortedSuitsIndexValues[i] << " ";
        int flushCount = 0, flushCardIndex = 0, suitCount = 0;
        maxFlushCount = 0, maxFlushCardIndex;
        bool flag = 1;
        for (int i = 0; i < nAll - 1; i++) {
            if (sortedSuitsIndexValues[i] == sortedSuitsIndexValues[i + 1]) {
                flushCount++;
                if (flag) {
                    suitCount++;
                    flag = false;
                }
            }
            else {
                flag = true;
                flushCardIndex = i + 1;
                flushCount = 0;
            }
            if (maxFlushCount < flushCount) {
                maxFlushCount = flushCount;
                maxFlushCardIndex = flushCardIndex;
            }
        }
//      cout << "\n\nmaxFlushCount: " << maxFlushCount << "\nmaxFlushCardIndex: " << maxFlushCardIndex << "\nsuitCount: " << suitCount;
        maxFlushCardValue = valueToInt(arr[0][sortedSuitsIndex[maxFlushCardIndex]], 1);
        vector<int> flushesIndex;
        hold = 0;
        for (int i = 1; i < nAll; i++) {
            if (sortedSuitsIndexValues[i] == sortedSuitsIndexValues[hold]) {
                if (flushesIndex.empty() || flushesIndex[flushesIndex.size() - 1] != hold) {
                    flushesIndex.push_back(hold);
                }
            }
            else {
                hold = i;
            }
        }
//      cout << "\n\nflushesIndex:\n";
//      for (int i = 0; i < suitCount; i++) cout << flushesIndex[i] << " ";
//      cout << "\n\n" << spacer(50, '-');

// find face duplicates
        int duplicateFacesAmount = 0,
                duplicateFacesCount = 0,
                duplicateFaceIndex = 0;
        do {
            duplicateFacesAmount = 0,
                duplicateFacesCount = 0,
                duplicateFaceIndex = 0;
            maxDuplicateFacesCount = 0, maxDuplicateFaceIndex = 0;
            for (int i = 0; i < nAll; i++) {
                if (sortedFacesIndexValues[duplicateFaceIndex] == sortedFacesIndexValues[i + 1]) {
                    duplicateFacesCount++;
                }
                else if (sortedFacesIndexValues[duplicateFaceIndex] != sortedFacesIndexValues[i + 1]) {
                    if (maxDuplicateFacesCount < duplicateFacesCount) {
                        maxDuplicateFacesCount = duplicateFacesCount;
                        maxDuplicateFaceIndex = duplicateFaceIndex;
                    }
                    if (duplicateFacesCount > 0) {
                        duplicateFacesAmount++;
                        duplicateFaceIndexArray.push_back(duplicateFaceIndex);
                        duplicateFacesCountArray.push_back(duplicateFacesCount);
                    }
                    duplicateFaceIndex = i + 1;
                    duplicateFacesCount = 0;
                }
            }
//        cout << maxDuplicateFaceIndex << endl;
        }
        while (maxDuplicateFaceIndex >= nAll);
//        cout << "check\n";
//        for (int i = 0; i < nAll; i++)
//            cout << arr[0][i] << " ";
//        cout << "\n";
//        for (int i = 0; i < nAll; i++)
//            cout << sortedFacesIndex[i] << " ";
//        cout << "\n"
//             << maxDuplicateFaceIndex << "\n";
//        cout << sortedFacesIndex[maxDuplicateFaceIndex] << "\n";
        maxDuplicateCardValue = valueToInt(arr[0][sortedFacesIndex[maxDuplicateFaceIndex]], 1);
//        cout << "check2\n";
        for (int i = 0; i < duplicateFacesAmount; i++) {
            for (int j = i + 1; j < duplicateFacesAmount; j++) {
                if (duplicateFacesCountArray[i] < duplicateFacesCountArray[j] ||
                   (duplicateFacesCountArray[i] == duplicateFacesCountArray[j] &&
                    sortedFacesIndexValues[duplicateFaceIndexArray[i]] < sortedFacesIndexValues[duplicateFaceIndexArray[j]])) {
                    int temp = duplicateFacesCountArray[i];
                    duplicateFacesCountArray[i] = duplicateFacesCountArray[j];
                    duplicateFacesCountArray[j] = temp;
                    temp = duplicateFaceIndexArray[i];
                    duplicateFaceIndexArray[i] = duplicateFaceIndexArray[j];
                    duplicateFaceIndexArray[j] = temp;
                }
            }
        }
//      cout << "\n\nmaxDuplicateFaceIndex: " << maxDuplicateFaceIndex
//               << "\nmaxDuplicateFacesCount: " << maxDuplicateFacesCount
//                         << "\nduplicateFacesAmount: " << duplicateFacesAmount
//                         << "\n\nduplicateFaceIndexArray and duplicateFacesCountArray:";
//                for (int i = 0; i < duplicateFacesAmount; i++) {
//                        cout << endl << duplicateFaceIndexArray[i] << " " << duplicateFacesCountArray[i];
//                }
//                cout << "\n\n" << spacer(50, '-');

// find handValue
        handValue = -1;
        if (maxStraightCount >= 4) {
            if (maxFlushCount >= 5 && sortedFacesIndex[maxStraightCardIndex] == sortedSuitsIndex[maxFlushCardIndex]) {
                if (valueToInt(arr[1][sortedFacesIndex[maxStraightCardIndex]], 2) == 12)
                    handValue = 9;
                else handValue = 8;
            }
            else handValue = 4;
        }
        else if (handValue < 5) {
            handValue = 0;
            if (maxFlushCount >= 5) handValue = 5;
            if (maxDuplicateFacesCount == 1) {
                bool flag = false;
                for (int i = 1; i < duplicateFacesAmount; i++) {
                    if (duplicateFacesCountArray[i] == 1) flag = true;
                }
                if (flag) handValue = 2;
                else if (handValue < 1) handValue = 1;
            }
            if (maxDuplicateFacesCount == 2) {
                bool flag = false;
                for (int i = 1; i < duplicateFacesAmount; i++) 
                    if (duplicateFacesCountArray[i] == 1) flag = true;
                if (flag) handValue = 6;
                else if (handValue < 3) handValue = 3;
            }
            if (maxDuplicateFacesCount == 3 && handValue < 7) handValue = 7;
        }
        //                cout << "\n" << handValue;

        //                cout << "\n\n";
        return handValue;
    }
};

struct Player {
    string name;
    string hold;
    int cash;
    Cards hand;
    int chance[3];

    Player(string n, int c) : cash(c) {
        hand.clear();
    	string temp = "";
    	for (int i = 0; i < 26; i++) {
			if (i < 23) temp += n[i];
    		else temp += '.';
    		if (n[i] == '\0') break;
		}
		name = temp;
		chance[0] = rng(1, 1000);
        chance[1] = rng(1, 500);
        chance[2] = rng(1, 500);
    }

    void info() {
        hold = hand.getCards();
        cout << name << endl
             << "\tcash:  " << cash << endl
             << "\tcards: " << hold << endl;
    }

    int bet(int x) {
        cash -= x;
        return x;
    }
};

struct Pot {
    int cash = 0;
    vector<int> playerBet;

    void info() {
        for (int n : playerBet)
            cout << n << " ";
    }

    int bet(int x, int id) {
        cash += x;
        if (id >= playerBet.size())
            playerBet.push_back(x);
        else
            playerBet[id] += x;
        return x;
    }

    int clear() {
        int x = cash;
        cash = 0;
        playerBet.clear();
        return x;
    }
};

int handCompare(Cards table, vector<Player> players, int value, vector<bool> folds, int &sideBetWinnerIndex) {
    int highCard = 0;
    for (int i = 0; i < players.size(); i++) players[i].hand.value(table.getCards());
	int index = -1;
	bool tableWins = false;
    do {
    	highCard = 0;
		switch (value) {
	    case 0: // High Card
	    	for (int i = 0; i < players.size(); i++) players[i].hand.value();
	        tableWins = false;
			for (int i = 0; i < players.size(); i++) {
	            if (players[i].hand.highCardValue > highCard && !folds[i] && players[i].hand.handValue == value) {
					highCard = players[i].hand.highCardValue;
					index = i;
				}
			}
			highCard = 0;
			for (int i = 0; i < players.size(); i++) {
	            if (players[i].hand.highCardValue > highCard && !folds[i] && players[i].hand.handValue == value && i != index) {
					highCard = players[i].hand.highCardValue;
					sideBetWinnerIndex = i;
				}
			}
	        break;
	    case 1: // One Pair
	    case 2: // Two Pair
	    case 3: // Triad
	    case 6: // Full House
	    case 7: // Four of a Kind
	        for (int i = 0; i < players.size(); i++) {
	            if (players[i].hand.maxDuplicateCardValue > highCard && !folds[i] && players[i].hand.handValue == value) {
	                highCard = players[i].hand.maxDuplicateCardValue;
					index = i;
				}
			}
			if (table.maxDuplicateCardValue > highCard && table.handValue == value) {
	            highCard = table.maxDuplicateCardValue;
				value = 0;
				tableWins = true;
			}
			highCard = 0;
			for (int i = 0; i < players.size(); i++) {
	            if (players[i].hand.maxDuplicateCardValue > highCard && !folds[i] && players[i].hand.handValue == value && i != index) {
	                highCard = players[i].hand.maxDuplicateCardValue;
					sideBetWinnerIndex = i;
				}
			}
	        break;
	    case 4: // Straight
	    case 8: // Straight Flush
	    case 9: // Royal Flush
	        for (int i = 0; i < players.size(); i++){
	            if (players[i].hand.maxStraightCardValue > highCard && !folds[i] && players[i].hand.handValue == value) {
	                highCard = players[i].hand.maxStraightCardValue;
					index = i;
				}
			}
			if (table.maxStraightCardValue > highCard && table.handValue == value) {
	            highCard = table.maxStraightCardValue;
				value = 0;
				tableWins = true;
			}
			highCard = 0;
	        for (int i = 0; i < players.size(); i++){
	            if (players[i].hand.maxStraightCardValue > highCard && !folds[i] && players[i].hand.handValue == value) {
	                highCard = players[i].hand.maxStraightCardValue;
					sideBetWinnerIndex = i;
				}
			}
	        break;
	    case 5: // Flush
	        for (int i = 0; i < players.size(); i++) {
	            if (players[i].hand.maxFlushCardValue > highCard && !folds[i] && players[i].hand.handValue == value) {
	                highCard = players[i].hand.maxFlushCardValue;
					index = i;
				}
			}
			if (table.maxFlushCardValue > highCard && table.handValue == value) {
	            highCard = table.maxFlushCardValue;
				value = 0;
				tableWins = true;
			}
			highCard = 0;
			for (int i = 0; i < players.size(); i++) {
	            if (players[i].hand.maxFlushCardValue > highCard && !folds[i] && players[i].hand.handValue == value && i != index) {
	                highCard = players[i].hand.maxFlushCardValue;
					sideBetWinnerIndex = i;
				}
			}
	    }
	}
	while (tableWins);
	
    return index;
}

void fileReader(string fileName, vector<string> &arr) {
	ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        system("cls");
		std::cerr << "Failed to open the input file: " << fileName << std::endl;
    }
    string text, line;
    while (getline(inputFile, line)) arr.push_back(line);
	inputFile.close();
}

void fileWriter(string fileName, vector<string> &arr) {
	ofstream outputFile(fileName, ios::trunc);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open the input file: " << fileName << std::endl;
    }
    for (string s : arr) outputFile << s + '\n';
	outputFile.close();
}

bool inputChecker(string a, string b) {
    bool result = false;
    vector<string> array;
    int start = 0, count = 0;
    for (int i = 0; i < b.size(); i++)
    {
        if (b[i] == ';')
        {
            for (int j = start; j < i; j++)
            {
                if (count >= array.size())
                    array.push_back("");
                array[count] += b[j];
            }
            count++;
            start = i + 1;
        }
    }
    for (string s : array)
    {
        if (a == s)
            result = true;
    }
    return result;
}

int pokerTurn(bool isUser, int &raise, Player player, int prevBet, int currBet, int initBet, vector<string> buttonDisp) {
    int choice, choice2;
    bool allInOnly = false;
    if (player.cash < currBet - prevBet) allInOnly = true;
    if (isUser) {
        int buttPos[3] = {49, 77, 105};
		choice = 0;
        bool enterKey = false, spaceKey = false;
		while (GetAsyncKeyState(VK_RETURN) & 0x8000);
		gotoxy(buttPos[choice], 43);
		while (true) {
			choice2 = choice * 2;
			for (string s : buttonDisp) {
				if (s == "#```-divider-```#") choice2--;
				else if (choice2 == 0) {
//						Sleep(1);
					cout << s << "\n";
					gotoxy(buttPos[choice], 0, false);
				}
				else if (choice2 < 0) break;
			}
			gotoxy(buttPos[choice], 43);
			while (true) {
	        	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !spaceKey) {
	        		spaceKey = true;
					break;
				}
				else if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) spaceKey = false;
				if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
	        		enterKey = true;
					break;
				}
			}
			choice2 = choice * 2 + 1;
			for (string s : buttonDisp) {
				if (s == "#```-divider-```#") choice2--;
				else if (choice2 == 0) {
//						Sleep(1);
					cout << s << "\n";
					gotoxy(buttPos[choice], 0, false);
				}
				else if (choice2 < 0) break;
			}
			if (enterKey) break;
			if (spaceKey) {
				choice = (choice + 1) % 3;
				gotoxy(buttPos[choice], 43);
			}
		}
    	cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		gotoxy(47, 30);
    	if (choice == 1) {
    		cout << "How much do you want to Raise?\n";
    		gotoxy(47, 32);
    		cout << " > ";
    		while (true) {
	            try {
					getline(cin, x);
					raise = stoi(x);
				}
				catch (exception e) {
					continue;
				}
				if (raise > player.cash - (currBet - prevBet)) {
					cout << "\n\t\tYou dont have enough money\n\t\t > ";
					continue;
				}
				break;
	        }
		}
    }
    else {
    	int arr[] = {1, player.chance[0], 2, player.chance[1], 3, player.chance[2]};
    	if (prevBet == currBet || allInOnly) arr[3] = 0;
		if (prevBet == currBet || prevBet == 0 || currBet <= initBet) arr[5] = 0;
        choice = rngp(arr);
//        cout << player.name << ": "
//			 << "\n\tPrevious Bet: " << prevBet
//			 << "\n\tCurrent Bet: " << currBet
//			 << "\n\tCash: " << player.cash;
		int arr2[] = {player.cash - (currBet - prevBet), 1, (player.cash - (currBet - prevBet)) / rng(2, 10), 2};
        raise = rng(1, rngp(arr2));
	}
    return choice;
}

int turner(int num, int plus, int max, vector<bool> bankruptIndex) {
    do num += plus;
    while (bankruptIndex[num]);
    return num % max;
}

string nameGenerator() {
	if (names.empty()) fileReader("nameList.txt", names);
	int index = rng(0, names.size() - 1);
	string name = names[index];
	names.erase(names.begin() + index);
	return name;
}

void cardDisplay(vector<string> &format, int card) {
	for (string &s : format) {
		string hold = "";
		for (int i = 0; i < 11; i++) {
			if (s[i] == '@') {
				if (card >= 36 && card <= 39) {
					s[i] = '1';
					s[i + 1] = '0';
					break;
				}
				else s[i] = faces[static_cast<int>(floor(card / 4))][0];
			}
			else if (s[i] == '#') {
				if (card >= 36 && card <= 39) {
					s[i] = '1';
					s[i + 1] = '0';
					break;
				}
				else {
					s[i] = ' ';
					s[i + 1] = faces[static_cast<int>(floor(card / 4))][0];
				}
			}
			else if (s[i] == '$') s[i] = suits[card % 4][0];
		}
	}
}

void poker() {
    HANDLE hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD newPosition;
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	int bet = 10;
    cout << "Controls:\n"
		 << "\t[SPACE]: switching between options\n"
		 << "\t[ENTER]: selecting an option\n\n"
		 << "Press [F11] to toggle fullscreen then enter to continue\n\n";
	getline(cin, x);
    system("cls");


	// prologue
	vector<string> display;
	vector<int> displayIndex;
	fileReader("display.txt", display);
	int sleepFlag = 1;
    while (true) { break;
		// name input
		for (int i = 0; display[i] != "#```-divider-```#"; i++) {
			Sleep(50 * sleepFlag);
			cout << display[i] << "\n";
		}
		Sleep(1000 * sleepFlag);
		cout << "\tWelcome to C++ Poker!\n\n";
		Sleep(1000 * sleepFlag);
		cout << "\tPlease state your name: ";
		getline(cin, name);
        cout << "\n\tHello, " << name << "!\n\n";
		Sleep(1000 * sleepFlag);
    	cout << "\tIs your name correct?\n\n";
		int choice = 0;
		bool enterKey = false, spaceKey = false;
		while (GetAsyncKeyState(VK_RETURN) & 0x8000);
		while (true) {
	        switch (choice) {
	        	case 0:
	        		cout << "\t > Yes\n"
	        			 << "\t   No";
	        		break;
	        	case 1:
	        		cout << "\t   Yes\n"
	        			 << "\t > No";
	        		break;
			}
			while (true) {
	        	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !spaceKey) {
	        		spaceKey = true;
					GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
					newPosition.X = consoleInfo.dwCursorPosition.X - 9;
					newPosition.Y = consoleInfo.dwCursorPosition.Y - 1;
					SetConsoleCursorPosition(hConsole, newPosition);
					break;
				}
				else if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) spaceKey = false;
				if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
	        		enterKey = true;
					break;
				}
			}
			if (enterKey) break;
			if (spaceKey) choice = (choice + 1) % 2;
		}
		if (choice == 1) {
        	cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			sleepFlag = 0;
			continue;
		}
	    
	    
	    // save file checking
	    bool hasSave = false;
	    vector<string> saves;
	    fileReader("saveFile.txt", saves);
	    for (int i = 0; i < saves.size(); i += 2) {
			if (saves[i] == name) {
	    		hasSave = true;
	    		cash = stod(saves[i + 1]);
	    		break;
			}
		}
		if (hasSave) {
			cout << "\n\n\tWelcome back, " << name << "!\n";
			Sleep(1000 * sleepFlag);
		}
		else {
			cout << "\n\n\tWould you like to create a save file?\n\n";
			enterKey = false, spaceKey = false, choice = 0;
			while (GetAsyncKeyState(VK_RETURN) & 0x8000);
			while (true) {
		        switch (choice) {
		        	case 0:
		        		cout << "\t > Yes\n"
		        			 << "\t   No, I am only a guest";
		        		break;
		        	case 1:
		        		cout << "\t   Yes\n"
		        			 << "\t > No, I am only a guest";
		        		break;
				}
				while (true) {
		        	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !spaceKey) {
		        		spaceKey = true;
						GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
						newPosition.X = consoleInfo.dwCursorPosition.X - 28;
						newPosition.Y = consoleInfo.dwCursorPosition.Y - 1;
						SetConsoleCursorPosition(hConsole, newPosition);
						break;
					}
					else if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) spaceKey = false;
					if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
		        		enterKey = true;
						break;
					}
				}
				if (enterKey) break;
				if (spaceKey) choice = (choice + 1) % 2;
			}
			if (choice == 0) {
				saves.push_back(name);
				saves.push_back(to_string(cash));
				fileWriter("saveFile.txt", saves);
				cout << "\n\n\tSave File successfully created.\n";
			}
			else cout << "\n";
		}
		cout << "\n\t";
		system("pause");
		system("cls");
		break;
	}
	// prologue end
    
    
    // body
    while (true) {
        // intro
		int choice = 0, choice2 = 0;
		bool enterKey = false, spaceKey = false;
		while (GetAsyncKeyState(VK_RETURN) & 0x8000);
		while (true) { break;
			choice = choice2 + 1;
			int skip = 1;
			for (string s : display) {
				if (s == "#```-divider-```#") choice--;
				else if (choice == 0) {
					if (skip == 1) skip = 0;
					else {
//						Sleep(1);
						cout << s << "\n";
					}
				}
				else if (choice < 0) break;
			}	
			GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
			newPosition.X = 0;
			newPosition.Y = 0;
			SetConsoleCursorPosition(hConsole, newPosition);
			while (true) {
	        	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !spaceKey) {
	        		spaceKey = true;
					break;
				}
				else if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) spaceKey = false;
				if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
	        		enterKey = true;
					break;
				}
			}
        	cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			system("cls");
			if (enterKey) break;
			if (spaceKey) choice2 = (choice2 + 1) % 4;
		}
        // intro end
        
        
        // play loop
        if (choice2 == 0) {
			// initializing initial bet
			while (true) {
				int i = 5, skip = 1;
		        for (string s : display) {
		        	if (s == "#```-divider-```#") i--;
					else if (i == 0) {
						if (skip == 1) skip = 0;
						else {
//							Sleep(1);
							cout << s << "\n";
						}
					}
					else if (i < 0) break;
				}
				GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
				newPosition.X = 0;
				newPosition.Y = 0;
				SetConsoleCursorPosition(hConsole, newPosition);
				try {
		            cout << "\n\n\n\n\tCreate a Room:\n\n"
		                 << "\tYour current cash is $" << cash << "\n\n"
		                 << (bet > cash ? "\tYou do not have enough money!\n" : "")
		                 << (bet <= 0 ? "\tYou cannot bet less than $1!\n" : "")
		                 << "\n\n\tInput the initial bet: ";
		            getline(cin, x);
		            bet = stoi(x);
		            if (bet <= cash && bet > 0)
		                break;
		        }
		        catch (exception e) {}
		        system("cls");
		    }
		    // initializing initial bet end
			
			// initialize variables
			cout << "\n\n\n\tInitial Bet: " << bet
		         << "\n\n\tcreating room...\n\n\n";
		    int n = rng(2, 10),
		    	pId = rng(0, n - 1),
		    	dealer = rng(0, n - 1);
		    Cards deck;
		    deck.shuffle();
		    Pot pot;
		    Cards table(true);
		    
		    
		    // initialize vectors
			vector<Player> player;
	        vector<bool> bankruptIndex, folds, allIns, preAllIns;
			vector<int> playerDisPos;
			for (int i = 0; i < n; i++) {
				player.push_back({(i == pId ? name : nameGenerator()), static_cast<int>(cash)});
				pot.bet(player[i].bet(0), i);
				bankruptIndex.push_back(0);
	            bankruptIndex.push_back(false);
	            folds.push_back(false);
	            allIns.push_back(false);
	            preAllIns.push_back(false);
				if (i != pId) cout << "\n\t\t" << player[i].name << " joined the room.\n";
			}
			const int arrayPos[18] = {7, 4, 41, 4, 75, 4, 109, 4, 143, 4, 143, 16, 143, 28, 7, 28, 7, 16};
			int posIndex = (pId - 2) % n;
			int posIndexHold = posIndex;
			for (int i = 0; i < 18; i += 2) {
				playerDisPos.push_back(arrayPos[i]);
				playerDisPos.push_back(arrayPos[i + 1]);
				posIndex = (posIndex - 1 + n) % n;
				if (posIndex == posIndexHold) break;
				if (posIndex == pId) continue;
			}
			cout << "\n\n\n\t";
			system("pause");
		    system("cls");
		
		    
			// round loop
		    int round = 1;
			while (round) { 
		        // initialize variables
				int turn = dealer,
		        	cbet = bet * pow(2, floor(round / 3));
		        
		        
		        // re-initialize vectors
		        for (int i = 0; i < n; i++) {
					pot.bet(player[i].bet(0), i);
		            folds[i] = false;
		            allIns[i] = false;
		            preAllIns[i] = false;
		        }
		        
		        
		        // pre-round blinds initialization
		        int i = 6, skip = 1;
		        for (string s : display) {
		        	if (s == "#```-divider-```#") i--;
					else if (i == 0) {
						if (skip == 1) skip = 0;
						else {
//							Sleep(1);
							cout << s << "\n";
						}
					}
					else if (i < 0) break;
				}
				int temp = 0;
				for (int i = 0; i < n; i++) {
					if (i == pId) continue;
					gotoxy(playerDisPos[temp], playerDisPos[temp + 1]);
					cout << player[i].name << ":\n\n";
					gotoxy(playerDisPos[temp], 0, false);
					cout << "    Cash: " << player[i].cash;
					temp += 2;
				}
				GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
				newPosition.X = 0;
				newPosition.Y = 0;
				SetConsoleCursorPosition(hConsole, newPosition);
				int startAllIns = 0, space = 6;
		        cout << spacer(18, "\n") << spacer(space, "\t") << "Round " << round << "\n"
			         << spacer(space, "\t") << "Initial Bet: " << cbet << "\n\n";
		        if (player[turn].cash <= cbet / 2) {
		        	cout << spacer(space, "\t") << player[turn].name << " All-ins! -$" << pot.bet(player[turn].bet(player[turn].cash), turn) << endl;
		        	startAllIns++;
		            preAllIns[turn] = true;
				}
				else {
			        cout << spacer(space, "\t") << player[turn].name << " is small blind. -$" << pot.bet(player[turn].bet(cbet / 2), turn) << endl;
				}
		        turn = turner(turn, 1, n, bankruptIndex);
		        if (player[turn].cash <= cbet) {
		        	cout << spacer(space, "\t") << player[turn].name << " All-ins! -$" << pot.bet(player[turn].bet(player[turn].cash), turn) << "\n\n";
		        	startAllIns++;
		            preAllIns[turn] = true;
				}
				else {
			        cout << spacer(space, "\t") << player[turn].name << " is big blind. -$" << pot.bet(player[turn].bet(cbet), turn) << "\n\n";
				}
		        
		        
		        // card distribution
//		        cout << spacer(space, "\t") << "Pot: $" << pot.cash << "\n\n";
		        for (int j = 0; j < 2; j++)
		            for (int i = 0; i < n; i++)
		                player[i].hand.drop(deck.pop());
		        cout << spacer(space, "\t");
		        vector<string> cardFormat;
				fileReader("card.txt", cardFormat);
				cardDisplay(cardFormat, player[pId].hand.valueToInt(player[pId].hand.cards[0], 1));
				system("pause");
				cout << "\n" << spacer(space, "\t") << "Distributing Cards...\n";
				cout << spacer(space, "\t");
				gotoxy(105, 30);
				for (string s : cardFormat) {
					cout << s << "\n";
					gotoxy(105, 0, false);
				}
				cardFormat.clear();
				fileReader("card.txt", cardFormat);
				cardDisplay(cardFormat, player[pId].hand.valueToInt(player[pId].hand.cards[1], 1));
				gotoxy(120, 30);
				for (string s : cardFormat) {
					cout << s << "\n";
					gotoxy(120, 0, false);
				}
				gotoxy(48, 28);
				system("pause");
				gotoxy(48, 18);
				for (int i = 0; i < 11; i++) {
					cout << spacer(50) << "\n";
					gotoxy(48, 0, false);
				}
				cardFormat.clear();
				fileReader("cardBack.txt", cardFormat);
				for (int i = 47; i < 120; i += 18) {
					gotoxy(i, 16);
					for (string s : cardFormat) {
						cout << s << "\n";
						gotoxy(i, 0, false);
					}
				}
				gotoxy(48, 28);
//		        system("cls");
		        
		        
		        // dealing loop, main gameplay
		        int deal = 0, calls;
		        while (deal != 4) {
		            // dealing loop intro
		            gotoxy(47, 28);
		            cout << spacer(30);
		            gotoxy(47, 28);
					cout << "Round " << round << ":    ";
					switch (deal) {
		            case 0:
		                cout << "Betting...\n";
		                break;
		            case 1:
		                cout << "Flop\n";
		                for (int i = 0; i < 3; i++)
		                    table.drop(deck.pop());
		                break;
		            case 2:
		                cout << "Turn\n";
		                table.drop(deck.pop());
		                break;
		            case 3:
		                cout << "River\n";
		                table.drop(deck.pop());
		                break;
		            }
//		            table.info();
//		            cout << "\n";
		            
		            
		            // player turn loop
					int raise = 0, foldsCount = 0;
					calls = 0;
					vector<string> buttonDisp;
					fileReader("turnButton.txt", buttonDisp);
		            while (true) {
		                // player turn skipping
		                for (int i = 0; i < n; i++) {
		                	if (folds[turn] || allIns[turn] || preAllIns[turn]) {
		                		if (allIns[turn]) calls++;
								turn = turner(turn, 1, n, bankruptIndex);
							}
							else break;
						}
						if (calls == n - foldsCount - startAllIns) break;

						
						// player turn decision
		                int choice = pokerTurn(turn == pId, raise, player[turn], pot.playerBet[turn], cbet, bet * pow(2, floor(round / 3)), buttonDisp);
//						Sleep(500);
						gotoxy(playerDisPos[turn * 2] + 4, playerDisPos[turn * 2 + 1] + 4);
						cout << spacer(20);
						gotoxy(playerDisPos[turn * 2] + 4, playerDisPos[turn * 2 + 1] + 4);
//						if (turn != pId) cout << player[turn].name << " ";
		                switch (choice) {
			                case 1: // check, call
			                    calls++;
			                    if (pot.playerBet[turn] < cbet) {
			                        if (cbet - pot.playerBet[turn] >= player[turn].cash) {
			                        	cout << "Call All-in!";
			                        	pot.bet(player[turn].bet(player[turn].cash), turn);
			                    		allIns[turn] = true;
									}
									else {
										cout << "Call";
				                        pot.bet(player[turn].bet(cbet - pot.playerBet[turn]), turn);
									}
			                    }
			                    else if (pot.playerBet[turn] == cbet)
			                        cout << "Check";
			                    else
			                        cout << "\n\n\nerror\n\n\n";
			                    break;
			                case 2: // raise
			                    cbet += raise;
			                    calls = 1;
			                    if (cbet == player[turn].cash + pot.playerBet[turn]) {
			                    	cout << "Raise All-in!";
			                    	pot.bet(player[turn].bet(player[turn].cash), turn);
			                    	allIns[turn] = true;
								}
								else {
									cout << "Raise " << raise;
			                    	pot.bet(player[turn].bet(cbet - pot.playerBet[turn]), turn);
								}
			                    break;
			                case 3: // fold
			                    cout << "Fold";
			                    folds[turn] = true;
			                    foldsCount++;
		                }
						
						
						// player turn increment
						turn = turner(turn, 1, n, bankruptIndex);
						
						
						// player turn loop end checking
						if (calls == n - foldsCount - startAllIns || n - foldsCount - startAllIns == 1) break;
		            }
		            // player turn loop end
//		            cout << "\n";
					gotoxy(47, 30);
					system("pause");
//		            system("cls");
		            
		            
		            // dealing loop increment 
		            deal++;
		            if (n - foldsCount == 1) {
		            	calls = 1;
		                break;
					}
		        }
		        // dealing loop, main gameplay end
		        
		        
		    	// round cleanup opening
		        cout << "Round " << round << "\n"
		        	 << "Community Cards:\n"
		        	 << table.getCards() << "\n\n";
		        		        

		        // record all called rankings in an array
		        calls += startAllIns;
		        vector<int> handRanks, handRanksIndex;
		        int num = 0;
		        for (int i = 0; i < calls; i++) {
		            while (folds[num])
		                num++;
		            cout << player[num].name << "\n";
		            cout << player[num].hand.getCards() << ": "
						 << combo[player[num].hand.value(table.getCards())] << "\n"
						 << "\n";
		            handRanks.push_back(player[num].hand.value(table.getCards()));
		            handRanksIndex.push_back(num);
		            num++;
		        }
		        
		
		        // sort the array
		        for (int i = 0; i < handRanks.size(); i++) {
		            for (int j = i + 1; j < handRanks.size(); j++) {
		                if (handRanks[i] <= handRanks[j]) {
		                    int temp = handRanks[i];
		                    handRanks[i] = handRanks[j];
		                    handRanks[j] = temp;
		                }
		            }
		        }
		        
		
				// check for the winnerIndex
				int winnerIndex, sideBetWinnerIndex = 0;
				if (handRanks.size() == 1) winnerIndex = handRanksIndex[0];
				else winnerIndex = handCompare(table, player, handRanks[0], folds, sideBetWinnerIndex);
				
				
		        // check for side bets
				int potHold = pot.cash;
				bool sideBetFlag = false;
		        for (int i = 0; i < player.size(); i++) {
		        	if (!folds[i] && i != winnerIndex && pot.playerBet[i] > pot.playerBet[winnerIndex]) {
		        		potHold -= cbet - pot.playerBet[winnerIndex];
		        		sideBetFlag = true;
					}
				}
		        
		        
		        // winner declaration
		        cout << "\n\nRound " << round << " End!\n"
		        	 << player[winnerIndex].name << " wins!\n"
		        	 << player[winnerIndex].name << " gets " << potHold << "\n";
		        player[winnerIndex].cash += potHold;
		        
		        
		        // side bet winner declaration
		    	if (sideBetFlag) {
		    		cout << player[sideBetWinnerIndex].name << " wins the side bet!\n"
		        		 << player[sideBetWinnerIndex].name << " gets " << pot.cash - potHold << "\n";
		        	player[sideBetWinnerIndex].cash += pot.cash - potHold;
				}
				
				
				// loser exiting
				for (int i = 0; i < n; i++) {
					if (player[i].cash <= 0) {
						bankruptIndex[i] = true;
						cout << player[i].name << " went bankrupt...\n\tgoodbye " << player[i].name << "\n";
						player.erase(player.begin() + i);
					}
				}
				n = player.size();
				cout << "\n\n\n";
				
		        
		        // round closing
//		        cout << pId << " " << player[pId].name << " " << bankruptIndex[pId] << "\n";
		        if (player[pId].cash <= 0) {
		        	cout << "Better luck next time...\n\n";
		        	cash = 0;
		        	system("pause");
		        	system("cls");
		        	break;
				}
				else if (n == 1) {
					double temp = rng(100, 200) / 100;
					cout << "CONGRATULATIONS\n"
						 << "You defeated all your opponents!\n\n"
						 << "Bonus Reward: Total Cash * " << temp << "\n\n";
					cash = player[pId].cash * temp;
					system("pause");
		        	system("cls");
					break;
				}
				else {
			        cout << "press Enter to proceed to the next round... (type [I give up] to exit to main menu)\n\n"
			             << " > ";
			        getline(cin, x);
			        system("cls");
			        if (x == "I give up") break;
				}
		        
				
				// next round preparation
				round++;
		        cout << "reshuffling...\n\n";
		        pot.clear();
		        table.clear();
		        for (int i = 0; i < n; i++) player[i].hand.clear();
		        deck.refill();
		        deck.shuffle();
		        while (rng(0, 9) == 0) {
		        	n++;
					player.push_back({nameGenerator(), static_cast<int>(cash)});
					pot.bet(player[n].bet(0), n);
					bankruptIndex.push_back(0);
		            bankruptIndex.push_back(false);
		            folds.push_back(false);
		            allIns.push_back(false);
		            preAllIns.push_back(false);
					cout << player[n].name << " joined the room.\n";
				}
		        dealer = dealer == (n - 1) ? 0 : dealer + 1;
		        cout << "\n";
				system("pause");
		        system("cls");
		    }
		    // round loop end
        }
    	// play loop end
	}
	// body end
}

int main() {
    srand(time(0));
    poker();

//	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
//	for (int i = 1; i < 255; i += 15) {
//		SetConsoleTextAttribute(hConsole, i);
//		cout << "                              \n";
//	}
	
//	time_t currentTime = std::time(nullptr);
//
//    std::tm* timeInfo = std::localtime(&currentTime);
//
//    if (timeInfo) {
//        int day = timeInfo->tm_mday;     // Day of the month (1-31)
//        int month = timeInfo->tm_mon + 1; // Month (0-11, so add 1 for 1-12)
//        int year = timeInfo->tm_year + 1900; // Year (since 1900)
//
//        // Print the day, month, and year
//        std::cout << "Day: " << day << std::endl;
//        std::cout << "Month: " << month << std::endl;
//        std::cout << "Year: " << year << std::endl;
//    } else {
//        std::cerr << "Failed to get current time information." << std::endl;
//    }

    return 0;
}
