/*******************************************************************************
* Name: Samantha Guilbeault
* Date: 11-01-2019
* Sources: cardtest4.c example from Module 4 and testUpdateCoins.c from Module 3
* Description: Contains unit tests for mineCardEffect().
*	Test 1: Choice1 = copper and choice2 = silver
*	Test 2: Error check - choice 1 is not a treasure
*	Test 3: Error check - choice 2 is not a treasure
*	Test 4: Error check - cost of choice1 + 3 is less than cost of choice2
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>

// source: http://www.dillonbhuff.com/?p=439
#define MY_ASSERT(x) if (!(x)) { printf("   *Assertion Failed: File %s, Line %d.\n", __FILE__, __LINE__); }

int main()
{
	// set game variables
	int seed = 50;
	int numPlayer = 2;

	// init test variables
	int currentPlayer = 0;
	int choice1 = -1;
	int choice2 = -1;
	int handPos = -1;
	int errorCheck = 10;
	int handCountBefore = -1;
	int discardCountBefore = -1;
	int deckCountBefore = -1;


	// set card array
	int k[10] = { tribute, ambassador, feast, gardens, mine,
	remodel, smithy, village, baron, treasure_map };

	// declare the game state
	struct gameState G;

	// init new game
	initializeGame(numPlayer, k, seed, &G);

	printf("Testing Mine Card Effects\n");

	/***************************************** TEST 1 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 1: Choice1 = copper and choice2 = silver\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	G.hand[currentPlayer][0] = copper;  // make sure current player has a copper
	choice1 = 0;  // choice1 is the hand position of the card to trash
	choice2 = silver;  // choice2 is the supply code for the card to gain
	G.hand[currentPlayer][1] = mine;  // make sure the mine card is in hand
	handPos = 1;  // location of mine card in hand
	handCountBefore = G.handCount[currentPlayer];  // track hand count
	discardCountBefore = G.discardCount[currentPlayer];  // track discard count
	deckCountBefore = G.deckCount[currentPlayer];  // track deck count

	// call function
	mineCardEffect(&G, currentPlayer, choice1, choice2, handPos);

	// hand count is one less than before - minus copper, minus mine, plus silver
	printf("- hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore - 1);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore - 1);

	// new treasure is added to end of hand
	printf("- last card in hand: %d, expected: %d\n", G.hand[currentPlayer][G.handCount[currentPlayer] - 1], choice2);
	MY_ASSERT(G.hand[currentPlayer][G.handCount[currentPlayer] - 1] == choice2);

	// deck count stays the same
	printf("- deck count: %d, expected: %d\n", G.deckCount[currentPlayer], deckCountBefore);
	MY_ASSERT(G.deckCount[currentPlayer] == deckCountBefore);

	// discard count increases by 1 - for mine card
	printf("- discard count: %d, expected: %d\n", G.discardCount[currentPlayer], discardCountBefore + 1);
	MY_ASSERT(G.discardCount[currentPlayer] == discardCountBefore + 1);



	/***************************************** TEST 2 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 2: Error check - choice1 is not a treasure\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	G.hand[currentPlayer][0] = smithy;  // make sure current player has a non treasure
	choice1 = 0;  // choice1 is the hand position of the card to trash
	choice2 = silver;  // choice2 is the supply code for the card to gain
	G.hand[currentPlayer][1] = mine;  // make sure the mine card is in hand
	handPos = 1;  // location of mine card in hand
	handCountBefore = G.handCount[currentPlayer];  // track hand count
	discardCountBefore = G.discardCount[currentPlayer];  // track discard count
	deckCountBefore = G.deckCount[currentPlayer];  // track deck coun

	// call function
	errorCheck = mineCardEffect(&G, currentPlayer, choice1, choice2, handPos);

	// function short curcuits because choice1 is not a treasure
	printf("- error check: %d, expected: -1\n", errorCheck);
	MY_ASSERT(errorCheck == -1);

	// no cards are gained, discarded, or trashed
	printf("- hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore);
	printf("- discard count: %d, expected: %d\n", G.discardCount[currentPlayer], discardCountBefore);
	MY_ASSERT(G.discardCount[currentPlayer] == discardCountBefore);
	printf("- deck count: %d, expected: %d\n", G.deckCount[currentPlayer], deckCountBefore);
	MY_ASSERT(G.deckCount[currentPlayer] == deckCountBefore);


	/***************************************** TEST 3 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 3: Error check - choice2 is not a treasure\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	G.hand[currentPlayer][0] = copper;  // make sure current player has a treasure
	choice1 = 0;  // choice1 is the hand position of the card to trash
	choice2 = smithy;  // make sure choice2 is invalid
	G.hand[currentPlayer][1] = mine;  // make sure the mine card is in hand
	handPos = 1;  // location of mine card in hand
	handCountBefore = G.handCount[currentPlayer];  // track hand count
	discardCountBefore = G.discardCount[currentPlayer];  // track discard count
	deckCountBefore = G.deckCount[currentPlayer];  // track deck coun

	// call function
	errorCheck = mineCardEffect(&G, currentPlayer, choice1, choice2, handPos);

	// function short curcuits because choice2 is not a treasure
	printf("- error check: %d, expected: -1\n", errorCheck);
	MY_ASSERT(errorCheck == -1);

	// no cards are gained, discarded, or trashed
	printf("- hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore);
	printf("- discard count: %d, expected: %d\n", G.discardCount[currentPlayer], discardCountBefore);
	MY_ASSERT(G.discardCount[currentPlayer] == discardCountBefore);
	printf("- deck count: %d, expected: %d\n", G.deckCount[currentPlayer], deckCountBefore);
	MY_ASSERT(G.deckCount[currentPlayer] == deckCountBefore);



	/***************************************** TEST 4 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 4: Error check - cost of choice1 + 3 is less than cost of choice2\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	G.hand[currentPlayer][0] = copper;  // make sure current player has a treasure
	choice1 = 0;  // choice1 is the hand position of the card to trash
	choice2 = gold;  // make sure choice2 is invalid
	G.hand[currentPlayer][1] = mine;  // make sure the mine card is in hand
	handPos = 1;  // location of mine card in hand
	handCountBefore = G.handCount[currentPlayer];  // track hand count
	discardCountBefore = G.discardCount[currentPlayer];  // track discard count
	deckCountBefore = G.deckCount[currentPlayer];  // track deck coun

	// call function
	errorCheck = mineCardEffect(&G, currentPlayer, choice1, choice2, handPos);

	// function short curcuits because choice2 is not a treasure
	printf("- error check: %d, expected: -1\n", errorCheck);
	MY_ASSERT(errorCheck == -1);

	// no cards are gained, discarded, or trashed
	printf("- hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore);
	printf("- discard count: %d, expected: %d\n", G.discardCount[currentPlayer], discardCountBefore);
	MY_ASSERT(G.discardCount[currentPlayer] == discardCountBefore);
	printf("- deck count: %d, expected: %d\n", G.deckCount[currentPlayer], deckCountBefore);
	MY_ASSERT(G.deckCount[currentPlayer] == deckCountBefore);


	printf("\nFinished testing Mine Card Effects\n");
	return 0;
}