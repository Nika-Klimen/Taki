#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//Name: Nika Klimenchuk
//ID: 322997628

//Defines:
#define NAME_LENGTH 21		//max player name length 
#define CARD_TYPE 6			//max card type sign in a string
#define START_NUM_CARD 4	//the umber of cards for aech player
#define PLUS 10				//the card PLUS
#define STOP 11				//the card STOP
#define DIRECTION 12		//the card CHANGE DIRECTION
#define TAKI 13				//the card TAKI
#define COLOR 14			//the card COLOR
#define GREEN 4				//card in color GREEN
#define YELLOW 1			//card in color YELLOW
#define BLUE 3				//card in color BLUE
#define RED 2				//card in color RED
#define NO_COLOR 0			//card has no color
#define FIRST_CARD 1		//choose the furst card
#define TAKE_FROM_DECK 0    //choose to take card from deck


//Struucts:
typedef struct card
{
	char cardType[CARD_TYPE];
	char cardColor;
	int colorInInt;
	int typeInInt;
}CARD;

typedef struct Player
{
	char name[NAME_LENGTH];
	int  numOfCards;
	CARD* cardsArr;
}PLAYER;

//Functions:
int welcome();
void nameOfPlayers(PLAYER** players, int numberOfPlayers);
void printCard(CARD card);
CARD* handFirstCards();
CARD convertToCard(int number, int color);
CARD randomCard();
void printUpper(CARD top);
void printTurn(CARD cards[], int numOfCards, char name[]);
int playersPick(int numOfCards);
int isPickValid(CARD arr[], CARD top, int numOfCards, int pick);
CARD removeCard(PLAYER arrPlayers[], int indexP, int pick);
void newCard(PLAYER arrPlayers[], int indexP);
CARD pickIsTaki(PLAYER players[], int indexP, CARD top);

//Main:
void main()
{
	int numOfPlayers;
	int i, turn = 0;
	PLAYER* players;
	char str[10] = "TAKI";
	bool gameON = true, stillTurn;
	CARD topCard;
	int pick;
	bool directionRight = true;
	int check = 0;

	srand(time(NULL));

	//Wellcome messege and get number of players
	numOfPlayers = welcome();

	//get the players name and create array of player
	nameOfPlayers(&players, numOfPlayers);

	//hand 4 cards to each player
	for (i = 0; i < numOfPlayers; i++)
	{
		players[i].cardsArr = handFirstCards();
		players[i].numOfCards = START_NUM_CARD;
	}

	//top card random selection (if special ReSelect)
	topCard = randomCard();
	while ((topCard.typeInInt == COLOR) || (topCard.typeInInt == TAKI) || (topCard.typeInInt == PLUS) || (topCard.typeInInt == STOP) || (topCard.typeInInt == DIRECTION)) // case the first card on the desk is special
	{
		topCard = randomCard();
	}
	//game loop
	while (gameON)
	{
		//chack direction amd accoring to it know whos turn it is
		if (directionRight)
		{
			if (turn > numOfPlayers)
			{
				turn = turn - numOfPlayers;
			}
			else
				turn = 0;
		}
		else
		{
			if (turn < 0)
			{
				turn = turn + numOfPlayers;
			}
			turn = numOfPlayers - 1;
		}

		//the game
		while ((gameON) && (turn < numOfPlayers) && (turn >= 0))
		{
			printUpper(topCard);
			printTurn(players[turn].cardsArr, players[turn].numOfCards, players[turn].name);

			pick = playersPick(players[turn].numOfCards);
			pick = isPickValid(players[turn].cardsArr, topCard, players[turn].numOfCards, pick);
			if (pick == 0)
			{
				newCard(players, turn);
			}
			else
			{
				if (players[turn].cardsArr[pick - 1].typeInInt == PLUS)
					check ++;
				topCard = removeCard(players, turn, pick);
			}

			//if player finished cards
			if (players[turn].numOfCards == 0)
			{
				if ((topCard.typeInInt == STOP) && (numOfPlayers == 2))
				{
					newCard(players, turn);
				}
				else if ((topCard.typeInInt == PLUS))
				{
					newCard(players, turn);
				}
				else
				{
					gameON = false;
					printf("\nThe winner is... %s! Congratulations!", players[turn].name);
					break;
				}
			}

			//special cards & whos next turn based on direction
			if (topCard.typeInInt == DIRECTION)
			{
				directionRight = !directionRight; // change direction
			}

			if (topCard.typeInInt == TAKI)
			{
				topCard = pickIsTaki(players, turn, topCard);
				if (players[turn].numOfCards == 0)
				{
					if ((topCard.typeInInt == STOP) && (numOfPlayers == 2))
					{
						newCard(players, turn);
					}
					else if ((topCard.typeInInt == PLUS))
					{
						newCard(players, turn);
					}
					else
					{
						gameON = false;
						printf("\nThe winner is... %s! Congratulations!", players[turn].name);
						break;
					}
				}
			}

			if (topCard.typeInInt == PLUS)
			{
				if (check == 1)
				{
					turn = turn;
					check = 0;
					
				}
				else
				{
					if (directionRight)
					{
						turn++;
					}
					else
					{
						turn--;
					}
				}
			}
			else if (topCard.typeInInt == STOP)
			{
				if (directionRight)
					turn = turn + 2;
				else
					turn = turn - 2;
			}
			else if (directionRight)
			{
				turn++;
			}
			else
			{
				turn--;
			}

			
		}
	}
	free(players);
}

//this function print the welcome message and the input of the players number.
int welcome()
{
	int numOfPlayers;
	printf("************ Welcome to TAKI game !!! *********");
	printf("\nPlease enter the number of players: ");
	scanf("%d", &numOfPlayers);

	return numOfPlayers;
}

//this function gets a pointer to an array of players and number of players and creates the array. the function will also get the input of each player name.
void nameOfPlayers(PLAYER** players, int numberOfPlayers)
{
	int i;
	PLAYER* playersTemp = NULL;
	playersTemp = (PLAYER*)malloc(numberOfPlayers * sizeof(PLAYER));
	if (playersTemp == NULL)
	{
		printf("\nMemory allocation failed!!!");
		exit(1);
	}
	for (i = 0; i < numberOfPlayers; i++)
	{
		printf("\nPlease enter the first name of player #%d\n", i + 1);
		scanf("%s", playersTemp[i].name);
	}
	*players = playersTemp;
}

//this function hands 4 random cards for each player for the start of the game.
CARD* handFirstCards()
{
	int j;
	CARD* tempCards;
	tempCards = (CARD*)malloc(START_NUM_CARD * sizeof(CARD));
	if (tempCards == NULL)
	{
		printf("\nMemory allocation failed!!!");
		exit(1);
	}
	for (j = 0; j < START_NUM_CARD; j++)
	{
		tempCards[j] = randomCard();
	}
	return tempCards;

}

//this function convert the card fron int to its value in the game (1-9 in number or 10-14 for special cards)
CARD convertToCard(int number, int color)
{
	CARD currentCard;

	currentCard.colorInInt = color;
	currentCard.typeInInt = number;

	switch (number)
	{
	case 1:
		strcpy(currentCard.cardType, "1");
		break;
	case 2:
		strcpy(currentCard.cardType, "2");
		break;
	case 3:
		strcpy(currentCard.cardType, "3");
		break;
	case 4:
		strcpy(currentCard.cardType, "4");
		break;
	case 5:
		strcpy(currentCard.cardType, "5");
		break;
	case 6:
		strcpy(currentCard.cardType, "6");
		break;
	case 7:
		strcpy(currentCard.cardType, "7");
		break;
	case 8:
		strcpy(currentCard.cardType, "8");
		break;
	case 9:
		strcpy(currentCard.cardType, "9");
		break;
	case PLUS:
		strcpy(currentCard.cardType, "+");
		break;
	case STOP:
		strcpy(currentCard.cardType, "STOP");
		break;
	case DIRECTION:
		strcpy(currentCard.cardType, "<->");
		break;
	case TAKI:
		strcpy(currentCard.cardType, "TAKI");
		break;
	default: // COLOR 
		strcpy(currentCard.cardType, "COLOR");
		break;
	}

	switch (color)
	{
	case RED:
		currentCard.cardColor = 'R';
		break;
	case YELLOW:
		currentCard.cardColor = 'Y';
		break;
	case GREEN:
		currentCard.cardColor = 'G';
		break;
	default: //BLUE
		currentCard.cardColor = 'B';
		break;
	}

	return currentCard;
}

//this function prints a card with a given string and char.
void printCard(CARD card)
{
	int length;
	length = strlen(card.cardType);
	printf("\n*********");
	printf("\n*       *");
	printf("\n*");
	switch (length)
	{
	case 1:
		printf("   %s   ", card.cardType);
		break;
	case 3:
		printf("  %s  ", card.cardType);
	case 4:
		printf("  %s ", card.cardType);
		break;
	default: //5
		printf(" %s ", card.cardType);
	}
	printf("*");
	printf("\n*   %c   *", card.cardColor);
	printf("\n*       *");
	printf("\n*********");
}

//this function picks a random card and returns it as a struct of card.
CARD randomCard()
{
	CARD card;
	card.colorInInt = 1 + rand() % 4;
	card.typeInInt = 1 + rand() % 14;

	card = convertToCard(card.typeInInt, card.colorInInt);

	if (card.typeInInt == COLOR) //in case the card is "change color"
	{
		card.colorInInt = NO_COLOR;
		card.cardColor = ' ';
	}

	return card;
}

//this function prints the upper card in the game.
void printUpper(CARD top)
{
	printf("\nUpper card:");
	printCard(top);
}

//this function print the player turn (name + numbered cards).
void printTurn(CARD cards[], int numOfCards, char name[])
{
	int i;
	printf("\n%s's turn:\n", name);
	for (i = 0; i < numOfCards; i++)
	{
		printf("\ncard #%d:", i + 1);
		printCard(cards[i]);
	}
}

//this function gets the player choise.
int playersPick(int numOfCards)
{
	int pick;
	printf("\nPlease enter %d if you want to take a card from deck \n or %d-%d if you want to put one of your cards in the middle:\n", TAKE_FROM_DECK, FIRST_CARD, numOfCards);

	scanf("%d", &pick);
	return pick;
}

//chacks if pick is valid - in case not , gets a new pick. if pick is color gets the desired color
int isPickValid(CARD arr[], CARD top, int numOfCards, int pick)
{
	if (arr[pick - 1].typeInInt == COLOR)
	{
		printf("\nPlease enter your color choice: \n");
		printf("%d - Yellow \n%d - Red \n%d - Blue \n%d - Green\n", YELLOW, RED, BLUE, GREEN);
		scanf("%d", &arr[pick - 1].colorInInt);
		arr[pick - 1] = convertToCard(arr[pick - 1].typeInInt, arr[pick - 1].colorInInt);
		return pick;
	}
	if (pick == 0)
		return pick;
	while ((pick > numOfCards) || (arr[pick - 1].colorInInt != top.colorInInt))
	{
		printf("\nInvalid card! Try again.");
		pick = playersPick(numOfCards);
		if ((arr[pick - 1].typeInInt == COLOR))
			break; //just to get out of loop
		if (pick == 0)
			break;
	}
	if (arr[pick - 1].typeInInt == COLOR)
	{
		printf("\nPlease enter your color choice: \n");
		printf("%d - Yellow \n%d - Red \n%d - Blue \n%d - Green\n", YELLOW, RED, BLUE, GREEN);
		scanf("%d", &arr[pick - 1].colorInInt);
		arr[pick - 1] = convertToCard(arr[pick - 1].typeInInt, arr[pick - 1].colorInInt);
		return pick;
	}
	return pick;
}

//this functions gets the player pick and remove it from the cards array. the function will also return the picked card.
CARD removeCard(PLAYER arrPlayers[], int indexP, int pick)
{
	CARD chosen;
	int i;
	CARD* newArr;
	chosen = arrPlayers[indexP].cardsArr[pick - 1];
	for (i = pick - 1; i < arrPlayers[indexP].numOfCards - 1; i++)
	{
		arrPlayers[indexP].cardsArr[i] = arrPlayers[indexP].cardsArr[i + 1];
	}
	arrPlayers[indexP].numOfCards--;
	if (arrPlayers[indexP].numOfCards == 0)
		return chosen;
	newArr = realloc(arrPlayers[indexP].cardsArr, arrPlayers[indexP].numOfCards * sizeof(CARD));
	if (newArr == NULL)
	{
		printf("\nMemory allocation failed!!!");
		exit(1);
	}
	arrPlayers[indexP].cardsArr = newArr;

	return chosen;
}

//this function adds a new card from the deck to the player cards.
void newCard(PLAYER arrPlayers[], int indexP)
{
	CARD* tempCards;
	int numCards;

	arrPlayers[indexP].numOfCards++;
	numCards = arrPlayers[indexP].numOfCards;
	tempCards = realloc(arrPlayers[indexP].cardsArr, numCards * sizeof(CARD));
	if (tempCards == NULL)
	{
		printf("\nMemory allocation failed!!!");
		exit(1);
	}
	arrPlayers[indexP].cardsArr = tempCards;

	arrPlayers[indexP].cardsArr[numCards - 1] = randomCard();
}

//this function gives the player more turns if he picked card TAKI
CARD pickIsTaki(PLAYER players[], int indexP, CARD top)
{
	int pick = 1;
	while ((players[indexP].numOfCards != 0) && (pick != 0))
	{
		printCard(top);
		printTurn(players[indexP].cardsArr, players[indexP].numOfCards, players[indexP].name);
		printf("\nPlease enter 0 if you want to finish your turn \nor 1-%d if you want to put one of your cards in the middle:\n", players[indexP].numOfCards);
		scanf("%d", &pick);
		while ((pick != 0) && (players[indexP].numOfCards > 0))
		{
			if (players[indexP].cardsArr[pick - 1].typeInInt == COLOR)
			{
				printf("\nPlease enter your color choice: \n");
				printf("%d - Yellow \n%d - Red \n%d - Blue \n%d - Green\n", YELLOW, RED, BLUE, GREEN);
				scanf("%d", &players[indexP].cardsArr[pick - 1].colorInInt);
				players[indexP].cardsArr[pick - 1] = convertToCard(players[indexP].cardsArr[pick - 1].typeInInt, players[indexP].cardsArr[pick - 1].colorInInt);
				top = removeCard(players, indexP, pick);
				return top;
			}
			while ((pick > players[indexP].numOfCards) || (players[indexP].cardsArr[pick - 1].colorInInt != top.colorInInt))
			{
				printf("\nInvalid card! Try again.");
				printf("\nPlease enter 0 if you want to finish your turn \nor 1-%d if you want to put one of your cards in the middle:\n");
				scanf("%d", &pick);
				if (players[indexP].cardsArr[pick - 1].typeInInt == COLOR)
					break;
				if (pick == 0)
					break;
			}
			if (players[indexP].cardsArr[pick - 1].typeInInt == COLOR)
			{
				printf("\nPlease enter your color choice: \n");
				printf("%d - Yellow \n%d - Red \n%d - Blue \n%d - Green\n", YELLOW, RED, BLUE, GREEN);
				scanf("%d", &players[indexP].cardsArr[pick - 1].colorInInt);
				players[indexP].cardsArr[pick - 1] = convertToCard(players[indexP].cardsArr[pick - 1].typeInInt, players[indexP].cardsArr[pick - 1].colorInInt);
				top = removeCard(players, indexP, pick);
				return top;
			}
			top = removeCard(players, indexP, pick);

		}
		return top;

	}
}