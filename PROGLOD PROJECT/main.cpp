#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
using namespace std;

string name, x;
string suits[4] = {"C", "S", "H", "D"};
string values[13] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};

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

    void print() {
        //cout << "There are currently " << n << " cards.\n";
		for (int i = 0; i < n; i++) {
            cout << cards[i] << " ";
        }
        cout << endl;
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
    	for (int i = 0; i < 52 * rng(1, 100); i++) {
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
};

struct Player {
	string name;
	string hold;
	int cash;
	Cards cards;
	
	Player(string n, int c) : name(n), cash(c) {
		cards.clear();
	}
	
	void print() {
		hold = cards.get();
		cout << name << endl
			 << "\tcash:  " << cash << endl
			 << "\tcards: " << hold << endl;
	}
	
	void gain(int x) {
		cash += x;
	}
};

void poker() {
	Cards cards;
	cards.shuffle();
	int bet = 50;
	int blind[2];
    
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
//	
//	do {
//		cout << "Welcome to Poker, " << name << "!\n\n"
//			 << "\t | Play\n\n"
//			 << " > ";
//		getline(cin, x);
//		system("cls");
//	}
//	while (x != "Play");
	
	Player player[3] = {Player("dude", 1000), Player("dode", 1000), Player(name, 1000)};
	
	for (int i = 0; i < 3; i++) player[i].print();
	cout << endl;
	
	blind[0] = 0;
	blind[1] = 1;
	cout << "Current Bet: " << bet << endl
		 << "Small Blind: " << player[blind[0]].name << " -" << bet/2 << endl
		 << "Big Blind:   " << player[blind[1]].name << " -" << bet << "\n\n";	 
	player[blind[0]].gain(-bet/2);
	player[blind[1]].gain(-bet);
	
	for (int i = 0; i < 3; i++) player[i].print();
	cout << endl;
	
	cout << "Distributing the cards...\n\n";
	for (int j = 0; j < 2; j++) for (int i = 0; i < 3; i++) player[i].cards.drop(cards.pop());
	getline(cin, x);
	system("cls");
	
	while (true) {
		cout << "Your cards: " << player[2].cards.get() << "\n\n"
			 << "\t | Fold\n"
			 << "\t | Call\n"
			 << "\t | Raise #\n\n"
			 << " > ";
		getline(cin, x);
		istringstream ss(x);
		vector<string> input;
		int count = 0;
		string line;
		
		while (getline(x, line)) {
			if (count >= 2) 
			if (!line.empty()) input.push_back(line);
			count++;
		}
		
		if (x == "Fold")
		if (x == "Fold")
		if (x == "Fold")		
	}
}

int main() {
	srand(time(0));
	poker();
	
	return 0;
}
