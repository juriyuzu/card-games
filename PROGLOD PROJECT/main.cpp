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
string values[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
string stack[52] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD"};

int rng(int min, int max) {
	return min + (rand() % (max - min));
}

struct Cards {
	string cards[52] = {"AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "10C", "JC", "QC", "KC", "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "10S", "JS", "QS", "KS", "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "10H", "JH", "QH", "KH", "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "10D", "JD", "QD", "KD"};
	string hold;
	int n = sizeof(cards) / sizeof(cards[0]);
	
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
	
	int valueToInt(string s) {
		for (int i = 0; i < sizeof(values); i++) {
			if (values[i] == s) {
				return i;
			}
		}
		return -1;
	}
	
	int value(string x) {
		x = print() + " " + x;
		stringstream ss(x);
		vector<vector<string>> arr(4);
		vector<int> sameInt;
		string line;
		
		// put the strings into the arr[0]
		while (ss >> line) arr[0].push_back(line);
//		for (int i = 0; i < 2; i++) for (string s : arr[i]) cout << s << " ";

		// split the strings to two parts: arr[1] and arr[2]
		line = "";
		for (int i = 0; i < arr[0].size(); i++) {
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
//		for (int i = 1; i < 3; i++) for (string s : arr[i]) cout << s << " ";

		// count duplicates in arr[1], put their value in arr[3], their count in sameInt
		for (int i = 0; i < arr[1].size(); i++) {
			bool flag = true;
			for (int j = 0; j < i; j++) {
				if (arr[1][i] == arr[1][j] && i != j) {
					flag = false;
					break;
				}
			}
			if (flag) {
				for (int j = i + 1; j < arr[1].size(); j++) {
					if (arr[1][i] == arr[1][j]) {
						if (flag) {
							arr[3].push_back(arr[1][i]);
							sameInt.push_back(1);
							flag = false;
						}
						sameInt[sameInt.size() - 1]++;
					}
				}
			}
		}
		for (int i = 0; i < arr[3].size(); i++) cout << arr[3][i] << " " << sameInt[i] << endl;
		
		// locate the index of the highest value in arr[3]
		int maxInd = 0;
		string maxStr;
		for (int i = 0; i < sameInt.size(); i++) {
			if (sameInt[i] > sameInt[maxInd]) maxInd = i;
			if (sameInt[i] == sameInt[maxInd]) if (valueToInt(arr[3][i]) > valueToInt(arr[3][maxInd])) maxInd = i;
		}
		cout << endl << maxInd;
		
		
		return 0;
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

void poker() {
	int bet = 1,
		cbet, blind, n, pId, dealer, turn, round, deal;
	Cards c;
	c.clear();
	c.drop("10C 10D KS QS");
	c.value("KD 10H KD QS QS");
	cout << endl;
	system("pause");
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
			cout << "\npress Enter to continue...";
			Cards deck;
			deck.shuffle();
			Pot pot;
			Cards table;
			table.clear();
			dealer = 1;//rng(0, n - 1);
			cbet = bet;
			round = 1;
			getline(cin, x);
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
					 << "distributing cards...\n\n"
					 << "press Enter to continue...";
				for (int j = 0; j < 2; j++) for (int i = 0; i < n; i++) player[i].hand.drop(deck.pop());
				turn = turn == (n - 1) ? 0 : turn + 1;
				deal = 1;
				getline(cin, x);
				system("cls");
				
				while (deal != 4) {
					switch (deal) {
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
					deck.print();
					table.print();
					for (int i = 0; i < n; i++) player[i].print();
					getline(cin, x);
					system("cls");
					deal++;
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
				cout << "press Enter to continue...";
				getline(cin, x);
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
