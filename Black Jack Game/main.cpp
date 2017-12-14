/* 
  File:   main.cpp
  Author: Drake Gerger
  Created on November 1, 2017, 1:00 PM
  Final Version on November 5, 2017, 11:38 AM
  Purpose: Project 1: Black Jack Game
 */
 
#include <iostream>
#include <ctime>
#include <cstdlib>

void Shuffle(bool CardsDealt[]);
void PrintCard(int Card);
void PrintHand(int Hand[], const int CardCount);
int GetNextCard(bool CardsDealt[]);
int ScoreHand(int Hand[], const int CardCount);
void PrintScoresAndHands(int HouseHand[], 
const int HouseCardCount, int PlayerHand[], const int PlayerCardCount);

int main() {
	using namespace std;
	// Seed the random number generator
	time_t cTime;
	time(&cTime);
	srand(cTime);

	bool CardsDealt[52];
	int HouseCardCount = 0;
	int HouseHand[12];
	int PlayerCardCount = 0;
	int PlayerHand[12];

	// Loop once for each hand
	while (true) {
		// randomize the cards; set them all to undealt
		Shuffle(CardsDealt);
		// Deal the hands. Get two cards for each
		PlayerHand[0]		= GetNextCard(CardsDealt);
		HouseHand[0]		= GetNextCard(CardsDealt);
		PlayerHand[1]		= GetNextCard(CardsDealt);
		HouseHand[1]		= GetNextCard(CardsDealt);
		HouseCardCount		= 2;
		PlayerCardCount	= 2;

		// Signal a new hand.
		cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
		cout << "$$$                    New Hand                      $$$" << endl;
		cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
                cout << endl;

		char PlayerChoice;
		bool PlayerHits	= true;
		int PlayerScore	= ScoreHand(PlayerHand, PlayerCardCount);
		// Get Player's hits. Calculate the score and redisplay after each hit.
		do {
			// Print the dealt cards, but only the House's second card.
			cout << "House's Hand:" << endl;
			cout << "?? ";
			PrintCard(HouseHand[1]);
			cout << endl;
			cout << "Player's Hand: Score -> " << ScoreHand(PlayerHand, PlayerCardCount) << endl;
			PrintHand(PlayerHand, PlayerCardCount);
                        cout << endl;

			// Ask the Player whether he wants a hit or to stay
			cout << "Would you like to Hit(H) or Stay(S): ";
                        cin >> PlayerChoice;
                        cout <<"---------------------------------------"<<endl;

			if (PlayerChoice == 'H') {
				PlayerHand[PlayerCardCount] = GetNextCard(CardsDealt);
				++PlayerCardCount;
			} else if (PlayerChoice == 'S') {
				PlayerHits = false;
			} else {
				cout << "Please Enter a H or S!" << endl;
                                cout << endl;
			}
			cout << endl;
			// Get the Player's current score to update and check for bust.
			PlayerScore	= ScoreHand(PlayerHand, PlayerCardCount);
		} while (PlayerHits && PlayerScore < 22);

		// Once the player is done taking hits, check whether he busted
		if (PlayerScore > 21) {
			// The Player busted. The House wins.
			cout << "$$$ The HOUSE WINS! $$$" << endl;
                        cout << endl;
			PrintScoresAndHands(HouseHand, HouseCardCount, PlayerHand, PlayerCardCount);
		} else {
			// If the player didn't bust, then the house takes hits below 17
			int HouseScore	= ScoreHand(HouseHand, HouseCardCount);
			while (HouseScore < 17) {
				HouseHand[HouseCardCount] = GetNextCard(CardsDealt);
				++HouseCardCount;
				HouseScore	= ScoreHand(HouseHand, HouseCardCount);
			}
			bool HouseBusts	= (HouseScore > 21);
			if (HouseBusts) {
				// The House busted. Player wins
				cout << "$$$ The PLAYER WINS! $$$" << endl;
                                cout << endl;
				PrintScoresAndHands(HouseHand, HouseCardCount, PlayerHand, PlayerCardCount);
			} else {
				// Compare scores and determine the winner
				if (PlayerScore == HouseScore) {
					// Tie. This is called a "push."
					cout << "$$$ TIE! $$$" << endl;
					PrintScoresAndHands(HouseHand, HouseCardCount, PlayerHand, PlayerCardCount);
				} else if (PlayerScore > HouseScore) {
					// The Player wins
					cout << "$$$ The PLAYER WINS! $$$" << endl;
					PrintScoresAndHands(HouseHand, HouseCardCount, PlayerHand, PlayerCardCount);
				} else {
					// The House wins
					cout << "$$$ The HOUSE WINS! $$$" << endl;
					PrintScoresAndHands(HouseHand, HouseCardCount, PlayerHand, PlayerCardCount);
				}
			}
		}
	}
	return EXIT_SUCCESS;
}

void Shuffle(bool CardsDealt[]) {
	for (int Index = 0; Index < 52; ++Index) {
		CardsDealt[Index] = false;
	}
}

void PrintCard(int Card) {
	using namespace std;
	// Print Rank
	const int Rank = (Card % 13);
	if (Rank == 0) {
		cout << 'A';
	} else if (Rank < 9) {
		cout << (Rank + 1);
	} else if (Rank == 9) {
		cout << 'T';
	} else if (Rank == 10) {
		cout << 'J';
	} else if (Rank == 11) {
		cout << 'Q';
	} else {
		cout << 'K';
	}
	// Print Suit
	const int Suit = (Card/13);
	if (Suit == 0) {
		cout << 'C';
	} else if (Suit == 1) {
		cout << 'D';
	} else if (Suit == 2) {
		cout << 'H';
	} else {
		cout << 'S';
	}
}

void PrintHand(int Hand[], const int CardCount) {
	using namespace std;
	for (int CardIndex = 0; CardIndex < CardCount; ++CardIndex) {
		const int NextCard = Hand[CardIndex];
		PrintCard(NextCard);
		cout << " ";
	}
	cout << endl;
}

int GetNextCard(bool CardsDealt[]) {
	bool CardDealt	= true;
	int NewCard	= -1;
	do {
		NewCard = (rand() % 52);
		if (!CardsDealt[NewCard]) {
			CardDealt = false;
		}
	} while (CardDealt);
	return NewCard;
}

int ScoreHand(int Hand[], const int CardCount) {
	int AceCount	= 0;
	int Score		= 0;
	for (int CardIndex = 0; CardIndex < CardCount; ++CardIndex) {
		const int NextCard = Hand[CardIndex];
		const int Rank = (NextCard % 13);
		if (Rank == 0) {
			++AceCount;
			++Score;
		} else if (Rank < 9) {
			Score = Score + (Rank + 1);
		} else {
			Score = Score + 10;
		}
	}
	while (AceCount > 0 && Score < 12) {
		--AceCount;
		Score = Score + 10;
	}
	return Score;
}

void PrintScoresAndHands(int HouseHand[], const int HouseCardCount, int PlayerHand[], const int PlayerCardCount) {
	using namespace std;
	cout << "House's Hand: Score -> " << ScoreHand(HouseHand, HouseCardCount) << endl;
	PrintHand(HouseHand, HouseCardCount);
	cout << "Player's Hand: Score -> " << ScoreHand(PlayerHand, PlayerCardCount) << endl;
	PrintHand(PlayerHand, PlayerCardCount);
	cout << endl;
}