#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <cmath>
using namespace std;

int cash = 1000;
string name = "username", x;
string suits[4] = {"C", "S", "H", "D"};
string faces[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
string stack[52] = {"2C", "2S", "2H", "2D", "3C", "3S", "3H", "3D", "4C", "4S", "4H", "4D", "5C", "5S", "5H", "5D", "6C", "6S", "6H", "6D", "7C", "7S", "7H", "7D", "8C", "8S", "8H", "8D", "9C", "9S", "9H", "9D", "10C", "10S", "10H", "10D", "JC", "JS", "JH", "JD", "QC", "QS", "QH", "QD", "KC", "KS", "KH", "KD", "AC", "AS", "AH", "AD"};

string spacer(int n, char c = ' ') {
    string s = "";
    for (int i = 0; i < n; i++) s += c;
    return s;
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
        cout << endl;
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

struct Player
{
    string name;
    string hold;
    int cash;
    Cards hand;

    Player(string n, int c) : name(n), cash(c) {
        hand.clear();
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

int handCompare(Cards table, vector<Player> players, int value, vector<bool> folds) {
    int highCard = 0;
    for (int i = 0; i < players.size(); i++) players[i].hand.value(table.getCards());
	int index = -1;
	bool tableWins = false;
    do {
		switch (value) {
	    case 0: // High Card
	        tableWins = false;
			for (int i = 0; i < players.size(); i++) {
	            if (players[i].hand.highCardValue > highCard && !folds[i] && players[i].hand.handValue == value) {
					highCard = players[i].hand.highCardValue;
					index = i;
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
	        break;
	    case 5: // Flush
	        for (int i = 0; i < players.size(); i++) {
	            if (players[i].hand.maxFlushCardValue > highCard && !folds[i] && players[i].hand.handValue == value) {
	                highCard = players[i].hand.maxFlushCardValue;
					index = true;
				}
			}
			if (table.maxFlushCardValue > highCard && table.handValue == value) {
	            highCard = table.maxFlushCardValue;
				value = 0;
				tableWins = true;
			}
	    }
	}
	while (tableWins);
	
    return index;
}

void display()
{
    // adfs
}

bool inputChecker(string a, string b)
{
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

int pokerTurn(bool isUser, int &raise)
{
    int choice;
    if (isUser) {
        //return 1;
        cout << "\n1. Check or Call\n2. Raise\n3. Fold\n > ";
        do {
            getline(cin, x);
        } 
		while (!inputChecker(x, "1;2;3;"));
        choice = stoi(x);
    	if (choice == 2) {
    		cout << "\nHow much do you want to Raise?\n > ";
    		while (true) {
	            try {
					getline(cin, x);
					raise = stoi(x);
				}
				catch (exception e) {
					continue;
				}
				break;
	        }
		}
    }
    else {
    	int arr[] = {1, 15, 2, 1, 3, 4};
        choice = rngp(arr);
        raise = rng(1, 5) * 10;
	}
    return choice;
}

int turner(int num, int plus, int max) {
    num += plus;
    return num % max;
}

void poker() {
    int bet = 1,
        cbet, n, pId, dealer, turn, round, deal;
    Cards c;
    //        c.drop("QC KS 5D 4S AH");
    cout << endl
         << c.value(" ");
    cout << endl;
    system("pause");
    system("cls");

    //        vector<Cards> eee;
    //        while (true) {
    //                Cards uh, uhh;
    //                uh.shuffle();
    //                cout << uh.getCards() << endl
    //                         << uh.value() << "\n\n";
    //                uhh.clear();
    //                for (int i = 0; i < 7; i++) uhh.drop(uh.pop());
    //                eee.push_back(uhh);
    //                for (Cards ccc : eee) {
    //                        cout << ccc.getCards() << endl
    //                                  << ccc.value() << "\n\n";
    //                }
    //                if (eee[eee.size() - 1].value() > 4) break;
    ////                system("pause");
    //                system("cls");
    //        }
    //        system("pause");

    //    do {
    //            cout << "Poker!\n"
    //                     << "Enter your name: ";
    //                getline(cin, name);
    //                system("cls");
    //                cout << "Hello, " << name << "!\n"
    //                         << "Is your name correct? [type yes to proceed] ";
    //                getline(cin, x);
    //                system("cls");
    //        }
    //        while (x != "yes");

    //        while (true) {
    //                cout << "Welcome to Poker, " << name << "!\n"
    //                         << "Your current cash is $" << cash << "\n\n"
    //                         << "\t | Play\n"
    //                         << "\t | About\n"
    //                         << "\t | Exit\n\n"
    //                         << " > ";
    //                getline(cin, x);
    //                system("cls");
    //
    //                if (x == "Play") {
    while (true)
    {
        try
        {
            cout << "Create a Room:\n"
                 << "Your current cash is $" << cash << "\n\n"
                 << (bet > cash ? "You do not have enough money!\n" : "")
                 << (bet <= 0 ? "You cannot bet less than $1!\n" : "")
                 << "Input the initial bet: ";
            getline(cin, x);
            bet = stoi(x);
            if (bet <= cash && bet > 0)
                break;
        }
        catch (exception e)
        {
        }
        system("cls");
    }
    system("cls");
    cout << "Initial Bet: " << bet << "\n\n"
         << "creating room...\n\n";
    n = 9; // rng(2, 9);
    pId = rng(0, n - 1);
    vector<Player> player;
    for (int i = 0; i < n; i++)
        player.push_back({(i == pId ? name : "Player_" + to_string(i)), cash});
    for (int i = 0; i < n; i++)
        if (i != pId)
            cout << player[i].name << " joined the room.\n";
    Cards deck;
    deck.shuffle();
    Pot pot;
    Cards table;
    table.clear();
    dealer = 1; //rng(0, n - 1);
    cbet = bet;
    round = 1;
    system("pause");
    system("cls");

    while (round)
    {
        turn = dealer;
        cbet = bet * pow(2, floor(round / 3));
        for (int i = 0; i < n; i++)
        {
            pot.bet(player[i].bet(0), i);
        }
        vector<bool> folds;
        for (int i = 0; i < n; i++)
        {
            folds.push_back(false);
        }
        //pot.info();
        cout << "Round " << round << "\n"
             << "Initial Bet: " << cbet << "\n\n"
             << player[turn].name << " is small blind. -$" << pot.bet(player[turn].bet(cbet / 2), turn) << endl;
        //pot.info();
        turn = turner(turn, 1, n);
        cout << player[turn].name << " is big blind. -$" << pot.bet(player[turn].bet(cbet), turn) << "\n\n";
        //pot.info();
        cout << "Pot: $" << pot.cash << "\n\n"
             << "distributing cards...\n\n";
        for (int j = 0; j < 2; j++)
            for (int i = 0; i < n; i++)
                player[i].hand.drop(deck.pop());
        turn = turner(turn, 1, n);
        pot.bet(player[turn].bet(0), turn);
        deal = 0;
        system("pause");
        system("cls");
        int foldsCount = 0, calls = 0, raise = 0;
        while (deal != 4)
        {
            cout << "Round " << round << "\n";
            switch (deal)
            {
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
            //deck.info();
            table.info();
            cout << "Your cards: " << player[pId].hand.getCards() << endl;
            //for (int i = 0; i < n; i++)
            //    player[i].info();

            calls = 0, raise = 0;
            while (true)
            {
                while (folds[turn])
                    turn = turner(turn, 1, n);
                //                                                cout << turn << " " << calls << "\n";
                //pot.info();
                int choice = pokerTurn(turn == pId, raise);
                cout << player[turn].name << "'s Turn: " << player[turn].name;
                switch (choice)
                {
                case 1: // check, call
                    if (pot.playerBet[turn] < cbet)
                    {
                        pot.bet(player[turn].bet(cbet - pot.playerBet[turn]), turn);
                        cout << " calls to the current bet of " << cbet << ".\n";
                    }
                    else if (pot.playerBet[turn] == cbet)
                        cout << " checks.\n";
                    else
                        cout << "\n\n\nerror\n\n\n";
                    calls++;
                    break;
                case 2: // raise
                    cbet += raise;
                    calls = 1;
                    pot.bet(player[turn].bet(cbet - pot.playerBet[turn]), turn);
                    cout << " raises " << raise << ".\n";
                    break;
                case 3: // fold
                    cout << " folds.\n";
                    folds[turn] = true;
                    foldsCount++;
                }

                turn = turner(turn, 1, n);
                if (calls == n - foldsCount || n - foldsCount == 1)
                    break;
            }

            system("pause");
            system("cls");
            deal++;
            if (n - foldsCount == 1)
                break;
        }

        cout << "Round " << round << "\n"
        	 << "Community Cards:\n"
        	 << table.getCards()
        	 << " : " << table.value() << "\n\n";
        
        // record all rankings in an array
        vector<int> handRanks, handRanksIndex;
        int num = 0;
        int hold = 1;
        while (hold--) 
        {
            num = 0;
            for (int i = 0; i < calls; i++)
            {
                while (folds[num])
                    num++;
                cout << player[num].name << "\n";
                cout << player[num].hand.getCards() << " : ";
//                cout << player[num].hand.value() << " : ";
                cout << player[num].hand.value(table.getCards()) << "\n";
                handRanks.push_back(player[num].hand.value(table.getCards()));
                handRanksIndex.push_back(num);
                num++;
            }
        }
//        for (int n : handRanks)
//            cout << n << " ";
//        cout << endl;

        // sort the array
        for (int i = 0; i < handRanks.size(); i++)
        {
            for (int j = i + 1; j < handRanks.size(); j++)
            {
                if (handRanks[i] <= handRanks[j])
                {
                    int temp = handRanks[i];
                    handRanks[i] = handRanks[j];
                    handRanks[j] = temp;
                }
            }
        }
//        for (int n : handRanks)
//            cout << n << " ";
//        cout << endl;

		// check for winner
		int winnerIndex;
		if (handRanks.size() == 1) winnerIndex = handRanksIndex[0];
		else {
//			vector<int> highs;
//			if (highs)
//			for (int i = 0; i < handRanks.size(); i++) if (handRanks[i] == handRanks[0]) highs.push_back(handRanks[i]);
			winnerIndex = handCompare(table, player, handRanks[0], folds);
		}
		
        cout << "\n\nRound " << round << " End!\n"
        	 << player[winnerIndex].name << " wins!\n\n"
             << "press Enter to proceed to the next round... (type [I give up] to exit to main menu)\n\n"
             << " > ";
        getline(cin, x);
        system("cls");
        if (x == "I give up")
            break;
        round++;
        cout << "reshuffling...\n\n";
        pot.clear();
        table.clear();
        for (int i = 0; i < n; i++)
            player[i].hand.clear();
        deck.refill();
        deck.shuffle();
        dealer = dealer == (n - 1) ? 0 : dealer + 1;
        system("pause");
        system("cls");
    }
    //                }
    //        }
}

int main()
{
    srand(time(0));
    poker();

    return 0;
}
