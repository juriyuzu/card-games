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
string stack[52] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD"};

string spacer(int n, char c = ' ') {
	string s = "";
	for (int i = 0; i < n; i++) s += c;
	return s;
}

int rng(int min, int max) {
	return min + (rand() % (max - min));
}

struct Cards {
	string cards[52] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD"};
	string hold;
	vector<string> maxValue;
	int n = sizeof(cards) / sizeof(cards[0]);
	
	int maxCardIndex;
	int maxStraightCardIndex, maxStraightCount;
	vector<int> sortedFacesIndex,
				sortedFacesIndexValues,
				sortedSuitsIndex,
				sortedSuitsIndexValues;
	int maxFlushCount, maxFlushCardIndex;
	vector<int> duplicateFaceIndexArray, duplicateFacesCountArray;
	int	maxDuplicateFacesCount, maxDuplicateFaceIndex, handValue;
	
	Cards(bool b = false) {
		if (b) clear();
	}
	
	void clear() {
		for (int i = 0; i < n; i++) {
			cards[i] = "";
		}
		n = 0;
	}
	
	string get() {
		hold = "";
		for (int i = 0; i < n; i++) {
			hold += cards[i];
			if (i != n - 1) hold += " ";
		}
		return hold;
	}

    void info() {
        //cout << "There are currently " << n << " cards.\n";
		for (int i = 0; i < n; i++) {
            cout << cards[i] << " ";
        }
        cout << endl;
    }
    
    string print() {
    	string s;
    	for (int i = 0; i < n; i++) s += cards[i] + " ";
    	return s;
	}
    
    void toTop(int x) {
    	hold = cards[x];
    	for (int i = x; i < n - 1; i++) {
    		cards[i] = cards[i + 1];
		}
		cards[n - 1] = hold;
	}
	
	void toBottom(int x) {
    	hold = cards[x];
    	for (int i = x; i > 0; i--) {
    		cards[i] = cards[i - 1];
		}
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
		if (b) toBottom(n);
		n++;
	}
	
	void refill() {
		clear();
		for (int i = 0; i < 52; i++) drop(stack[i]);
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

	int value(string x) {
		x = print() + " " + x;
		stringstream ss(x);
		vector<vector<string>> arr(3);
		string line;
		
// put the strings into arr[0]
		while (ss >> line) arr[0].push_back(line);
		int nAll = arr[0].size();
		cout << "arr[0]:\n";
		for (int i = 0; i < 2; i++) for (string s : arr[i]) cout << s << " ";

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
		cout << "\n\narr[1] and arr[2]:";
		for (int i = 1; i < 3; i++) {
			cout << endl;
			for (string s : arr[i]) cout << s << " ";
		}
		cout << "\n\n" << spacer(50, '-');
		
// sort the indexes of arr[1] and put it in sortedFacesIndex
		for (int i = 0; i < nAll; i++) sortedFacesIndex.push_back(i);
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
		maxCardIndex = sortedFacesIndex[0];
		cout << "\n\nsortedFacesIndex:\n";
		for (int i = 0; i < nAll; i++) cout << sortedFacesIndex[i] << " ";
		
// find maxStraightIndex
		for(int i = 0; i < nAll; i++) sortedFacesIndexValues.push_back(valueToInt(arr[1][(sortedFacesIndex[i])], 2));
		cout << "\n\nsortedFacesIndexValues:\n";
		for (int i = 0; i < nAll; i++) cout << sortedFacesIndexValues[i] << " ";
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
		cout << "\n\nmaxStraightCount: " << maxStraightCount << "\nmaxStraightCardIndex: " << maxStraightCardIndex;
		vector<int> maxStraightIndex;
		int hold = maxStraightCardIndex;
		maxStraightIndex.push_back(hold);
		for (int i = maxStraightCardIndex; i < nAll; i++) {
			if (sortedFacesIndexValues[hold] - 1 == sortedFacesIndexValues[i + 1]) {
				maxStraightIndex.push_back(i + 1);
				hold = i + 1;
			}
		}
		cout << "\n\nmaxStraightIndex:\n";
		for (int i = 0; i < maxStraightCount + 1; i++) cout << maxStraightIndex[i] << " ";
		cout << "\n\n" << spacer(50, '-');

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
		cout << "\n\nsortedSuitsIndex:\n";
		for (int i = 0; i < nAll; i++) cout << sortedSuitsIndex[i] << " ";

// find maxFlushCardIndex
		for(int i = 0; i < nAll; i++) sortedSuitsIndexValues.push_back(valueToInt(arr[2][(sortedSuitsIndex[i])], 3));
		cout << "\n\nsortedSuitsIndexValues:\n";
		for (int i = 0; i < nAll; i++) cout << sortedSuitsIndexValues[i] << " ";
		int flushCount = 0, flushCardIndex = 0, suitCount = 0;
		maxFlushCount = 0 , maxFlushCardIndex;
		for (int i = 0; i < nAll; i++) {
			if (sortedSuitsIndexValues[i] == sortedSuitsIndexValues[i + 1]) {
				flushCount++;
			}
			else if (sortedSuitsIndexValues[i] != sortedSuitsIndexValues[i + 1]) {
				if (maxFlushCount < flushCount) {
					maxFlushCount = flushCount;
					maxFlushCardIndex = flushCardIndex;
					suitCount++;
				}
				flushCardIndex = i + 1;
				flushCount = 0;
				
			}
		}
		cout << "\n\nmaxFlushCount: " << maxFlushCount << "\nmaxFlushCardIndex: " << maxFlushCardIndex << "\nsuitCount: " << suitCount;
		vector<int> flushesIndex;
		hold = 0;
		flushesIndex.push_back(hold);
		for (int i = 0; i < nAll; i++) {
			if (sortedSuitsIndexValues[hold] - 1 == sortedSuitsIndexValues[i + 1]) {
				flushesIndex.push_back(i + 1);
				hold = i + 1;
			}
		}
		cout << "\n\nflushesIndex:\n";
		for (int i = 0; i < suitCount; i++) cout << flushesIndex[i] << " ";
		cout << "\n\n" << spacer(50, '-');
		
// find face duplicates
		int duplicateFacesAmount = 0, 
			duplicateFacesCount = 0, 
			duplicateFaceIndex = 0;
		maxDuplicateFacesCount = 0, maxDuplicateFaceIndex;
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
		cout << "\n\nmaxDuplicateFaceIndex: " << maxDuplicateFaceIndex 
			 << "\nmaxDuplicateFacesCount: " << maxDuplicateFacesCount
			 << "\nduplicateFacesAmount: " << duplicateFacesAmount
			 << "\n\nduplicateFaceIndexArray and duplicateFacesCountArray:";
		for (int i = 0; i < duplicateFacesAmount; i++) {
			cout << endl << duplicateFaceIndexArray[i] << " " << duplicateFacesCountArray[i];
		}
		cout << "\n\n" << spacer(50, '-');
		
// find handValue
		handValue = -1;
		if (maxStraightCount == 4) {
			if (maxFlushCount == 5 && sortedFacesIndex[maxStraightCardIndex] == sortedSuitsIndex[maxFlushCardIndex]) {
				if (valueToInt(arr[1][sortedFacesIndex[maxStraightCardIndex]], 2) == 12) handValue = 9;
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
				for (int i = 1; i < duplicateFacesAmount; i++) {
					if (duplicateFacesCountArray[i] == 1) flag = true;
				}
				if (flag) handValue = 6;
				else if (handValue < 3) handValue = 3;
			}
			if (maxDuplicateFacesCount == 3) handValue = 7;
		}
		
		return handValue;
	}

};

struct Player {
	string name;
	string hold;
	int cash;
	Cards hand;
	
	Player(string n, int c) : name(n), cash(c) {
		hand.clear();
	}
	
	void print() {
		hold = hand.get();
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
	
	int bet(int x, int id) {
		cash += x;
		if (id >= playerBet.size()) playerBet.push_back(x);
		else playerBet[id] = x;
		return x;
	}
	
	int clear() {
		int x = cash;
		cash = 0;
		playerBet.clear();
		return x;
	}
};

void display() {
	// adfs
}

bool inputChecker(string x, string y) {
	bool b = false;
	vector<string> array;
	int start = 0, count = 0;
	for (int i = 0; i < y.size(); i++) {
		if (y[i] == ';') {
			for (int j = start; j < i; j++) {
				if (count >= array.size()) array.push_back("");
				array[count] += y[j];
			}
			count++;
			start = i + 1;
		}
	}
	for (string s : array) {
	    cout << s << " ";
	    if (x == s) b = true;
	}
	cout << "\n\n";
	return b;
}

int pokerTurn(bool isUser) {
	int choice;
	if (isUser) {
		do {
			cout << "1. Check or Call\n2. Raise\n3. Fold";
			getline(cin, x);
		}
		while (inputChecker(x, "1;2;3;"))
		
	}
	return choice;
}

void poker() {
	int bet = 1,
		cbet, n, pId, dealer, turn, round, deal;
	Cards c;
	c.clear();
	c.drop("10H QS AC 10S JS");
	cout << endl << c.value("QH KS AS QS 10H");
	cout << endl;
//	system("pause");
	system("cls");
//    do {
//	    cout << "Poker!\n"
//	    	 << "Enter your name: ";
//		getline(cin, name);
//		system("cls");
//		cout << "Hello, " << name << "!\n"
//			 << "Is your name correct? [type yes to proceed] ";
//		getline(cin, x);
//		system("cls");
//	}
//	while (x != "yes");
	
//	while (true) {
//		cout << "Welcome to Poker, " << name << "!\n"
//			 << "Your current cash is $" << cash << "\n\n"
//			 << "\t | Play\n"
//			 << "\t | About\n"
//			 << "\t | Exit\n\n"
//			 << " > ";
//		getline(cin, x);
//		system("cls");
//		
//		if (x == "Play") {
			while (true) {
				try {
					cout << "Create a Room:\n"
						 << "Your current cash is $" << cash << "\n\n"
					 	 << (bet > cash ? "You do not have enough money!\n" : "")
					 	 << (bet <= 0 ? "You cannot bet less than $1!\n" : "")
						 << "Input the initial bet: ";
					getline(cin, x);
					bet = stoi(x);
					if (bet <= cash && bet > 0) break;
				}
				catch (exception e) {}
				system("cls");
			}
			system("cls");
			cout << "Initial Bet: " << bet << "\n\n"
				 << "creating room...\n\n";
			n = 2;//rng(2, 11);
			pId = rng(0, n - 1);
			vector<Player> player;
			for (int i = 0; i < n; i++) player.push_back({(i == pId ? name : "Player_" + to_string(i)), cash});
			for (int i = 0; i < n; i++) if (i != pId) cout << player[i].name << " joined the room.\n";
			Cards deck;
			deck.shuffle();
			Pot pot;
			Cards table;
			table.clear();
			dealer = 1;//rng(0, n - 1);
			cbet = bet;
			round = 1;
			system("pause");
			system("cls");
			
			while (round) {
				turn = dealer;
				cbet = bet * pow(2, floor(round / 3));
				cout << "Round " << round << "\n"
					 << "Initial Bet: " << cbet << "\n\n"
					 << player[turn].name << " is small blind. -$" << pot.bet(player[turn].bet(cbet / 2), turn) << endl;
				turn = turn == (n - 1) ? 0 : turn + 1;
				cout << player[turn].name << " is big blind. -$" << pot.bet(player[turn].bet(cbet), turn) << "\n\n";
				cout << "Pot: $" << pot.cash << "\n\n"
					 << "distributing cards...\n\n";
				for (int j = 0; j < 2; j++) for (int i = 0; i < n; i++) player[i].hand.drop(deck.pop());
				turn = turn == (n - 1) ? 0 : turn + 1;
				deal = 0;
				system("pause");
				system("cls");
				
				while (deal != 4) {
					switch (deal) {
						case 0:
							cout << "Betting...\n";
							break;
						case 1:
							cout << "Flop\n";
							for (int i = 0; i < 3; i++) table.drop(deck.pop());
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
					deck.info();
					table.info();
					for (int i = 0; i < n; i++) player[i].print();
					
					int calls = 0, raise = 0, folds;
					while (true) {
						cout << player[turn].name << "'s Turn: " << player[turn].name;
						int choice;
						switch (choice) {
							case 0: // check, call
								if (pot.playerBet[turn] < cbet) {
									pot.bet(player[turn].bet(cbet), turn);
									cout << " calls.\n";
								}
								else cout << " checks.\n";
								calls++;
								break;
							case 1: // raise
								cbet += raise;
								pot.bet(player[turn].bet(cbet), turn)
								cout << " raises.\n";
								break;
							case 2: // fold
								cout << " folds.\n";
								folds++;
						}
						if (calls == n - folds || n - folds == 1) break;
						else turn++;
					}
					
					system("pause");
					system("cls");
					deal++;
					if (n - folds == 1) break;
				}
				
				cout << "Round " << round << " End!\n"
					 << "press Enter to proceed to the next round... (type [I give up] to exit to main menu)\n\n"
					 << " > ";
				getline(cin, x);
				system("cls");
				if (x == "I give up") break;
				round++;
				cout << "reshuffling...\n\n";
				pot.clear();
				table.clear();
				for (int i = 0; i < n; i++) player[i].hand.clear();
				deck.refill();
				deck.shuffle();
				dealer = dealer == (n - 1) ? 0 : dealer + 1;
				system("pause");
				system("cls");
			}
//		}
//	}
}

int main() {
	srand(time(0));
	poker();
	
	return 0;
}

