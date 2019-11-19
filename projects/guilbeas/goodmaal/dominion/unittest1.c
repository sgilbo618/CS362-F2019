/*******************************************************************************
* Name: Samantha Guilbeault
* Date: 10-23-2019
* Sources: cardtest4.c example from Module 4 and testUpdateCoins.c from Module 3
* Description: Contains unit tests for baronCardEffect(). 
*	Test 1: Choice = 1 (play estate) and player has an estate card in hand.
*	Test 2: Choice = 1 and player does not have an estate card in hand.
*	Test 3: Choice = 0 (do not play estate)
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
	int choice1 = -1;
	int handPos = -1;
	int *bonus = 0;
	int currentPlayer = 0;
	int coinsBefore = -1;
	int supplyCountBefore = -1;
	int discardCountBefore = -1;
	int numBuysBefore = -1;

	// set card array
	int k[10] = { tribute, council_room, feast, gardens, mine,
	remodel, smithy, village, baron, treasure_map };

	// declare the game state
	struct gameState G;

	// init new game
	initializeGame(numPlayer, k, seed, &G);

	printf("Testing Baron Card Effects\n");

	/***************************************** TEST 1 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 1: Choice = 1 and player has an estate card\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	choice1 = 1;  // play estate for coins
	numBuysBefore = G.numBuys;  // track number of buys
	G.hand[currentPlayer][0] = estate;  // make sure player has an estate to play
	G.hand[currentPlayer][1] = gold;  // make sure card to replace estate in hand is a card that could not be there yet
	G.hand[currentPlayer][2] = baron; // make sure baron card in hand
	handPos = 2;
	coinsBefore = G.coins;  // track coins before playing estate
	discardCountBefore = G.discardCount[currentPlayer];  // track discard pile count
	G.discard[currentPlayer][0] = -1;  // make sure discard pile has no value
	int handCountBefore = G.handCount[currentPlayer];  // track how many cards in player's hand

	// call function
	baronCard(handPos, choice1, currentPlayer, &G, bonus);

	// 1 buy is added
	printf("- buy count: %d, expected: %d\n", G.numBuys, numBuysBefore + 1);
	MY_ASSERT(G.numBuys == numBuysBefore + 1);

	// 4 coins are added
	printf("- coin count: %d, expected: %d\n", G.coins, coinsBefore + 4);
	MY_ASSERT(G.coins == coinsBefore + 4);

	// one card is discarded
	printf("- discard count: %d, expected: %d\n", G.discardCount[currentPlayer], discardCountBefore + 1);
	MY_ASSERT(G.discardCount[currentPlayer] == discardCountBefore + 1);

	// an estate card was discarded
	printf("- discard top: %d, expected: %d\n", G.discard[currentPlayer][G.discardCount[currentPlayer] - 1], estate);
	MY_ASSERT(G.discard[currentPlayer][G.discardCount[currentPlayer] - 1] == estate);

	// the hand is shifted to fill in where estate card was
	printf("- estate card position filled with: %d, expected: %d\n", G.hand[currentPlayer][0], gold);
	MY_ASSERT(G.hand[currentPlayer][0] == gold);

	// the hand has one less card
	printf("- hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore - 1);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore - 1);

	// the slot removed from the hand equals -1
	printf("- last slot in hand: %d, expected: -1\n", G.hand[currentPlayer][G.handCount[currentPlayer]]);
	MY_ASSERT(G.hand[currentPlayer][G.handCount[currentPlayer]] == -1);


	/***************************************** TEST 2 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 2: Choice = 1 and player does not have an estate card\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	choice1 = 1;  // try to play estate for coins
	for (int i = 0; i < G.handCount[currentPlayer]; i++)  // fill hand with coppers so no estate
	{
		G.hand[currentPlayer][i] = copper;
	}
	G.hand[currentPlayer][2] = baron; // make sure baron card in hand
	handPos = 2;
	coinsBefore = G.coins;  // track coins
	supplyCountBefore = G.supplyCount[estate];  // track supply count for estate
	discardCountBefore = G.discardCount[currentPlayer];  // track discard pile count
	G.discard[currentPlayer][0] = -1;  // make sure discard pile has no value


	// call function
	baronCard(handPos, choice1, currentPlayer, &G, bonus);

	// coins stay the same
	printf("- coin count: %d, expected: %d\n", G.coins, coinsBefore);
	MY_ASSERT(G.coins == coinsBefore);

	// supply count for estate goes down one
	printf("- supply count: %d, expected: %d\n", G.supplyCount[estate], supplyCountBefore - 1);
	MY_ASSERT(G.supplyCount[estate] == supplyCountBefore - 1);

	// discard pile count increases by 1
	printf("- discard count: %d, expected: %d\n", G.discardCount[currentPlayer], discardCountBefore + 1);
	MY_ASSERT(G.discardCount[currentPlayer] == discardCountBefore + 1);

	// discarded card is estate
	printf("- discarded card: %d, expected: %d\n", G.discard[currentPlayer][0], estate);
	MY_ASSERT(G.discard[currentPlayer][0] == estate);


	/***************************************** TEST 3 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 3: Choice = 0\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	choice1 = 0;
	numBuysBefore = G.numBuys;  // track number of buys
	G.hand[currentPlayer][0] = estate;  // make sure player has an estate
	G.hand[currentPlayer][2] = baron; // make sure baron card in hand
	handPos = 2;
	coinsBefore = G.coins;  // track coin count
	supplyCountBefore = G.supplyCount[estate];  // track supply count for estate
	discardCountBefore = G.discardCount[currentPlayer];  // track discard pile count
	G.discard[currentPlayer][0] = -1;  // make sure discard pile has no value

	// call function
	baronCard(handPos, choice1, currentPlayer, &G, bonus);

	// 1 buy is added
	printf("- buy count: %d, expected: %d\n", G.numBuys, numBuysBefore + 1);
	MY_ASSERT(G.numBuys == numBuysBefore + 1);

	// coins stay the same
	printf("- coin count: %d, expected: %d\n", G.coins, coinsBefore);
	MY_ASSERT(G.coins == coinsBefore);

	// supply count for estate goes down one
	printf("- supply count: %d, expected: %d\n", G.supplyCount[estate], supplyCountBefore - 1);
	MY_ASSERT(G.supplyCount[estate] == supplyCountBefore - 1);

	// discard pile count increases by 1
	printf("- discard count: %d, expected: %d\n", G.discardCount[currentPlayer], discardCountBefore + 1);
	MY_ASSERT(G.discardCount[currentPlayer] == discardCountBefore + 1);

	// discarded card is estate
	printf("- discarded card: %d, expected: %d\n", G.discard[currentPlayer][0], estate);
	MY_ASSERT(G.discard[currentPlayer][0] == estate);


	printf("\nFinished testing Baron Card Effects\n\n\n");
	return 0;
}