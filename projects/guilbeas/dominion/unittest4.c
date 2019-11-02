/*******************************************************************************
* Name: Samantha Guilbeault
* Date: 10-31-2019
* Sources: cardtest4.c example from Module 4 and testUpdateCoins.c from Module 3
* Description: Contains unit tests for tributeCardEffect().
*	Test 1: Next player has 0 cards total to reveal
*	Test 2: Next player has 1 treasure card in deck, 0 cards in discard
*	Test 3: Next player has 2 different cards in discard, but 0 in deck
*	Test 4: Next player reveals 2 of the same card
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

// source: http://www.dillonbhuff.com/?p=439
#define MY_ASSERT(x) if (!(x)) { printf("   *Assertion Failed: File %s, Line %d.\n", __FILE__, __LINE__); }

int main()
{
	// set game variables
	int seed = 50;
	int numPlayer = 2;

	// init test variables
	int currentPlayer = 0;
	int nextPlayer = 1;
	int playedCardsBefore = -1;
	int coinsBefore = -1;
	int handCountBefore = -1;
	int actionsBefore = -1;
	int npDeckCountBefore = -1;
	int npDiscardCountBefore = -1;

	// set card array
	int k[10] = { tribute, ambassador, feast, gardens, mine,
	remodel, smithy, village, baron, treasure_map };

	// declare the game state
	struct gameState G;

	// init new game
	initializeGame(numPlayer, k, seed, &G);

	printf("Testing Tribute Card Effects\n");

	/***************************************** TEST 1 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 1: Next player has 0 cards total to reveal\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	G.deckCount[nextPlayer] = 0;  // make sure next player deck count is empty
	G.discardCount[nextPlayer] = 0; // make sure next player discard count is empty
	playedCardsBefore = G.playedCardCount;  // track played card pile
	coinsBefore = G.coins;  // track coins for this turn
	handCountBefore = G.handCount[currentPlayer];  // track hand count of current player
	actionsBefore = G.numActions;  // track actions for this turn

	// call function
	tributeCardEffect(&G, currentPlayer);

	// next player's card counts stay at 0
	printf("- next player deck count: %d, expected: 0\n", G.deckCount[nextPlayer]);
	MY_ASSERT(G.deckCount[nextPlayer] == 0);
	printf("- next players discard count: %d, expected: 0\n", G.discardCount[nextPlayer]);
	MY_ASSERT(G.discardCount[nextPlayer] == 0);

	// no cards are played
	printf("- played cards: %d, expected: %d\n", G.playedCardCount, playedCardsBefore);
	MY_ASSERT(G.playedCardCount == playedCardsBefore);

	// current player doesn't gain anything
	printf("- coins: %d, expected: %d\n", G.coins, coinsBefore);
	MY_ASSERT(G.coins == coinsBefore);
	printf("- hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore);
	printf("- actions: %d, expected: %d\n", G.numActions, actionsBefore);
	MY_ASSERT(G.numActions == actionsBefore);


	/***************************************** TEST 2 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 2: Next player has 1 treasure card in deck, 0 cards in discard\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	G.deck[nextPlayer][0] = silver;  // make sure one treasue in next player deck
	G.deckCount[nextPlayer] = 1;
	G.discardCount[nextPlayer] = 0;  // make sure discard count of next player is 0
	npDeckCountBefore = G.deckCount[nextPlayer];  // track np deck count
	npDiscardCountBefore = G.discardCount[nextPlayer];  // track np discard count
	coinsBefore = G.coins;  // track cp coins
	handCountBefore = G.handCount[currentPlayer];  // track cp hand count
	actionsBefore = G.numActions;  // track cp actions

	// call function
	tributeCardEffect(&G, currentPlayer);

	// next player's deck count decreases by 1 card
	printf("- next player deck count: %d, expected: %d\n", G.deckCount[nextPlayer], npDeckCountBefore - 1);
	MY_ASSERT(G.deckCount[nextPlayer] == npDeckCountBefore - 1);

	// next player's revealed card is discarded
	printf("- next player discard count: %d, expected: %d\n", G.discardCount[nextPlayer], npDiscardCountBefore + 1);
	MY_ASSERT(G.discardCount[nextPlayer] == npDiscardCountBefore + 1);

	// current player gains eactly 2 coins, and nothing else
	printf("- coins: %d, expected: %d\n", G.coins, coinsBefore + 2);
	MY_ASSERT(G.coins == coinsBefore + 2);
	printf("- hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore);
	printf("- actions: %d, expected: %d\n", G.numActions, actionsBefore);
	MY_ASSERT(G.numActions == actionsBefore);



	/***************************************** TEST 3 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 3: Next player has 2 different cards in discard, but 0 in deck\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	G.deckCount[nextPlayer] = 0;  // make sure next player deck count is 0
	G.discardCount[nextPlayer] = 2;  // make sure discard has 2 different cards
	G.discard[nextPlayer][0] = estate;
	G.discard[nextPlayer][1] = smithy;
	npDeckCountBefore = G.deckCount[nextPlayer];  // track np deck count
	npDiscardCountBefore = G.discardCount[nextPlayer];  // track np discard count
	coinsBefore = G.coins;  // track cp coins
	handCountBefore = G.handCount[currentPlayer];  // track cp hand count
	actionsBefore = G.numActions;  // track cp actions

	// call function
	tributeCardEffect(&G, currentPlayer);

	// next player discard count ends up the same - two are taken and returned
	printf("- next player discard count: %d, expected: %d\n", G.discardCount[nextPlayer], npDiscardCountBefore);
	MY_ASSERT(G.discardCount[nextPlayer] == npDiscardCountBefore);

	// next player deck count ends up the same - two add and taken
	printf("- next player deck count: %d, expected: %d\n", G.deckCount[nextPlayer], npDeckCountBefore);
	MY_ASSERT(G.deckCount[nextPlayer] == npDeckCountBefore);

	// current player gains 2 cards and 2 actions, no coins
	printf("- coins: %d, expected: %d\n", G.coins, coinsBefore);
	MY_ASSERT(G.coins == coinsBefore);
	printf("- hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore + 2);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore + 2);
	printf("- actions: %d, expected: %d\n", G.numActions, actionsBefore + 2);
	MY_ASSERT(G.numActions == actionsBefore + 2);


	/***************************************** TEST 4 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 4: Next player reveals 2 of the same card from deck\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	G.deckCount[nextPlayer] = 2;  // make sure next player deck count is 2
	G.deck[nextPlayer][0] = estate;  // make sure cards in deck are the same
	G.deck[nextPlayer][1] = estate;
	npDeckCountBefore = G.deckCount[nextPlayer];  // track np deck count
	npDiscardCountBefore = G.discardCount[nextPlayer];  // track np discard count
	coinsBefore = G.coins;  // track cp coins
	handCountBefore = G.handCount[currentPlayer];  // track cp hand count
	actionsBefore = G.numActions;  // track cp actions

	// call function
	tributeCardEffect(&G, currentPlayer);

	// next player uses 2 cards from deck
	printf("- next player deck count: %d, expected: %d\n", G.deckCount[nextPlayer], npDeckCountBefore - 2);
	MY_ASSERT(G.deckCount[nextPlayer] == npDeckCountBefore - 2);
	printf("- next player discard count: %d, expected: %d\n", G.discardCount[nextPlayer], npDiscardCountBefore + 2);
	MY_ASSERT(G.discardCount[nextPlayer] == npDiscardCountBefore + 2);

	// current player only gains cards for one victory card, coins and actions stay the same
	printf("- coins: %d, expected: %d\n", G.coins, coinsBefore);
	MY_ASSERT(G.coins == coinsBefore);
	printf("- hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore + 2);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore + 2);
	printf("- actions: %d, expected: %d\n", G.numActions, actionsBefore);
	MY_ASSERT(G.numActions == actionsBefore);


	printf("\nFinished testing Tribute Card Effects\n\n\n");
	return 0;
}
