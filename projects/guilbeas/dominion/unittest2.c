/*******************************************************************************
* Name: Samantha Guilbeault
* Date: 10-23-2019
* Sources: cardtest4.c example from Module 4 and testUpdateCoins.c from Module 3
* Description: Contains unit tests for minionCardEffect().
*	Test 1: choice1 = 1 and choice2 = 0 (gain action and take +2 coins)
*	Test 2: choice1 = 0 and choice2 = 1 (gain action and take redraw)
*	Test 3: choice1 = 0 and choice2 = 0 (gain action and take no action)
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
	int numPlayer = 3;

	// init test variables
	int currentPlayer = 0;
	int secondPlayer = 1;
	int thirdPlayer = 2;
	int choice1 = -1;
	int choice2 = -1;
	int handPos = 0;
	int actionsBefore = -1;
	int coinsBefore = -1;
	int playedCardCountBefore = -1;
	int discardCountBefore = -1;
	int handCountBefore = -1;

	// set card array
	int k[10] = { tribute, minion, feast, gardens, mine,
	remodel, smithy, village, baron, treasure_map };

	// declare the game state
	struct gameState G;

	// init new game
	initializeGame(numPlayer, k, seed, &G);

	printf("Testing Minion Card Effects\n");

	/***************************************** TEST 1 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 1: Choice1 = 1 and Choice2 = 0\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	choice1 = 1;
	choice2 = 0;
	G.hand[currentPlayer][handPos] = minion;  // make sure minion card is being played
	actionsBefore = G.numActions;  // track number of actions
	playedCardCountBefore = G.playedCardCount;  // track number of cards played
	coinsBefore = G.coins;  // track coins before
	discardCountBefore = G.discardCount[currentPlayer];  // track number of cards in discard pile
	for (int i = 0; i < 5; i++)
	{
		drawCard(secondPlayer, &G);  // make sure second player has 5 cards
	}
	for (int i = 0; i < 3; i++)
	{
		drawCard(thirdPlayer, &G);  // make sure third player has 4 cards
	}

	// call function
	minionCardEffect(&G, choice1, choice2, currentPlayer, handPos);

	// actions increase by one
	printf("- actions count: %d, expected: %d\n", G.numActions, actionsBefore + 1);
	MY_ASSERT(G.numActions == actionsBefore + 1);

	// exactly one minion card played
	printf("- cards played: %d, expected: %d\n", G.playedCardCount, playedCardCountBefore + 1);
	MY_ASSERT(G.playedCardCount == playedCardCountBefore + 1);
	printf("- card played: %d, expected: %d\n", G.playedCards[G.playedCardCount - 1], minion);
	MY_ASSERT(G.playedCards[G.playedCardCount - 1] == minion);

	// exactly +2 coins gained
	printf("- coin count: %d, expected: %d\n", G.coins, coinsBefore + 2);
	MY_ASSERT(G.coins == coinsBefore + 2);

	// no cards are discarded
	printf("- current player discards: %d, expected: %d\n", G.discardCount[currentPlayer], discardCountBefore);
	MY_ASSERT(G.discardCount[currentPlayer] == discardCountBefore);

	// other players do not redraw 4
	printf("- player2 card count: %d, expected: %d\n", G.handCount[secondPlayer], 5);
	MY_ASSERT(G.handCount[secondPlayer] == 5);
	printf("- player3 card count: %d, expected: %d\n", G.handCount[thirdPlayer], 3);
	MY_ASSERT(G.handCount[thirdPlayer] == 3);


	/***************************************** TEST 2 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 2: Choice1 = 0 and Choice2 = 1\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	choice1 = 0;
	choice2 = 1;
	G.hand[currentPlayer][handPos] = minion;  // make sure minion card is being played
	coinsBefore = G.coins;  // track coins before
	handCountBefore = G.handCount[currentPlayer];  // track hand count
	discardCountBefore = G.discardCount[currentPlayer];  // track number of cards in discard pile
	for (int i = 0; i < 5; i++)
	{
		drawCard(secondPlayer, &G);  // make sure second player has 5 cards
	}
	for (int i = 0; i < 4; i++)
	{
		drawCard(thirdPlayer, &G);  // make sure third player has 4 cards
		G.hand[thirdPlayer][i] = gold;  // make third player's hand all gold
	}

	// call function
	minionCardEffect(&G, choice1, choice2, currentPlayer, handPos);

	// coins stay the same
	printf("- coin count: %d, expected: %d\n", G.coins, coinsBefore);
	MY_ASSERT(G.coins == coinsBefore);

	// current player hand discarded and 4 redrawn
	printf("- current player cards discarded: %d, expected: %d\n", G.discardCount[currentPlayer], handCountBefore);
	MY_ASSERT(G.discardCount[currentPlayer] == handCountBefore);
	printf("- current player hand count: %d, expected: 4\n", G.handCount[currentPlayer]);
	MY_ASSERT(G.handCount[currentPlayer] == 4);

	// players with 5 or more cards discard and redraw 4
	printf("- player2 hand count: %d, expected: 4\n", G.handCount[secondPlayer]);
	MY_ASSERT(G.handCount[secondPlayer] == 4);

	// players with 4 or less do not redraw
	printf("- contents of player3's hand: ");
	for (int i = 0; i < 4; i++) { printf("%d, ", G.hand[thirdPlayer][i]); }
	printf("expected: %d, %d, %d, %d\n", gold, gold, gold, gold);
	for (int i = 0; i < 4; i++) { MY_ASSERT(G.hand[thirdPlayer][i] == gold); }

	/***************************************** TEST 3 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 3: Choice1 = 0 and Choice2 = 0\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	choice1 = 0;
	choice2 = 0;
	G.hand[currentPlayer][handPos] = minion;  // make sure minion card is being played
	actionsBefore = G.numActions;  // track number of actions
	coinsBefore = G.coins;  // track coins before
	handCountBefore = G.handCount[currentPlayer];  // track hand count

	// call function
	minionCardEffect(&G, choice1, choice2, currentPlayer, handPos);

	// actions increase by one
	printf("- actions count: %d, expected: %d\n", G.numActions, actionsBefore + 1);
	MY_ASSERT(G.numActions == actionsBefore + 1);

	// coins stay the same
	printf("- coin count: %d, expected: %d\n", G.coins, coinsBefore);
	MY_ASSERT(G.coins == coinsBefore);

	// hand count stays the same
	printf("- hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore);


	printf("\nFinished testing Baron Card Effects\n");

	return 0;
}