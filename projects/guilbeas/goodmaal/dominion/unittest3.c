/*******************************************************************************
* Name: Samantha Guilbeault
* Date: 10-27-2019
* Sources: cardtest4.c example from Module 4 and testUpdateCoins.c from Module 3
* Description: Contains unit tests for ambassadorCardEffect().
*	Test 1: Choose to reveal a Smithy and return 0 to supply
*	Test 2: Choose to reveal village and return 2 to the supply
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
	int choice2 = -1;
	int handPos = -1;
	int currentPlayer = 0;
	int otherPlayer = 1;
	int revealedCard = -1;
	int supplyCountBefore = -1;
	int otherPlayerDiscard = -1;
	int discardCountBefore = -1;
	int handCountBefore = -1;

	// set card array
	int k[10] = { tribute, ambassador, feast, gardens, mine,
	remodel, smithy, village, baron, treasure_map };

	// declare the game state
	struct gameState G;

	// init new game
	initializeGame(numPlayer, k, seed, &G);

	printf("Testing Amassador Card Effects\n");

	/***************************************** TEST 1 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 1: Chooses to reveal a Smithy and return 0 to supply\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	revealedCard = smithy;  // set test card to be smithy
	G.hand[currentPlayer][0] = revealedCard;  // make sure return card is at hand position 0
	choice1 = 0;  // set to be where the smithy card is
	choice2 = 0;  // choosing to return 0 copies of smithy
	G.hand[currentPlayer][1] = ambassador;  // make sure player has ambassador card
	handPos = 1;  // set position of ambassador card
	supplyCountBefore = G.supplyCount[revealedCard];  // track supply of smithy cards
	G.discard[otherPlayer][0] = -1;  // make sure other player's discard pile top is null
	otherPlayerDiscard = G.discardCount[otherPlayer];  // track number of cards discarded by other player
	G.discard[currentPlayer][0] = -1; // make sure current player's discard pile top is null
	discardCountBefore = G.discardCount[currentPlayer];  // track discard count for current player
	handCountBefore = G.handCount[currentPlayer];  // track hand count of current player


	// call function
	ambassadorCard(handPos, currentPlayer, choice1, choice2, &G);

	// supply count of choosen card goes down by number of other players
	printf("- smithy supply: %d, expected: %d\n", G.supplyCount[revealedCard], supplyCountBefore - (numPlayer - 1));
	MY_ASSERT(G.supplyCount[revealedCard] == supplyCountBefore - (numPlayer - 1));

	// other player gains exactly 1 copy of revealed card in discard pile
	printf("- other player discard count: %d, expected: %d\n", G.discardCount[otherPlayer], otherPlayerDiscard + 1);
	MY_ASSERT(G.discardCount[otherPlayer] == otherPlayerDiscard + 1);
	printf("- other player's top discard: %d, expected: %d\n", G.discard[otherPlayer][0], revealedCard);
	MY_ASSERT(G.discard[otherPlayer][0] == revealedCard);

	// only ambassador card gets discarded for current player
	printf("- current player discard count: %d, expected: %d\n", G.discardCount[currentPlayer], discardCountBefore + 1);
	MY_ASSERT(G.discardCount[currentPlayer] == discardCountBefore + 1);
	printf("- current player's discard: %d, expected: %d\n", G.discard[currentPlayer][0], ambassador);
	MY_ASSERT(G.discard[currentPlayer][0] == ambassador);

	// no cards are trashed from current player
	printf("- current player hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore - 1);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore - 1);


	/***************************************** TEST 2 ****************************************/
	printf("\n********************************************************************************\n");
	printf("* Test 2: Chooses to reveal village and returns 2 to the supply\n");
	printf("********************************************************************************\n");

	memset(&G, 23, sizeof(struct gameState)); // clear game
	initializeGame(numPlayer, k, seed, &G);  // init new game

	// set variables for this test
	revealedCard = village;  // set test card to be smithy
	G.hand[currentPlayer][2] = revealedCard;  // make sure player has 2 of the revealed card
	G.hand[currentPlayer][3] = revealedCard;
	choice1 = 2;  // set to be where the village card is
	choice2 = 2;  // choosing to return 2 copies of village
	G.hand[currentPlayer][1] = ambassador;  // make sure player has ambassador card
	handPos = 1;  // set position of ambassador card

	supplyCountBefore = G.supplyCount[revealedCard];  // track supply of village cards
	G.discard[otherPlayer][0] = -1;  // make sure other player's discard pile top is null
	otherPlayerDiscard = G.discardCount[otherPlayer];  // track discrad count of other player
	handCountBefore = G.handCount[currentPlayer];  // track hand count of current player
	discardCountBefore = G.discardCount[currentPlayer];  // track discard count of current player


	// call function
	ambassadorCard(handPos, currentPlayer, choice1, choice2, &G);

	// supply count of choosen card will be +2 - number of other players
	printf("- smithy supply: %d, expected: %d\n", G.supplyCount[revealedCard], supplyCountBefore + choice2 - (numPlayer - 1));
	MY_ASSERT(G.supplyCount[revealedCard] == supplyCountBefore + choice2 - (numPlayer - 1));

	// other player gains exactly 1 copy of revealed card in discard pile
	printf("- other player discard count: %d, expected: %d\n", G.discardCount[otherPlayer], otherPlayerDiscard + 1);
	MY_ASSERT(G.discardCount[otherPlayer] == otherPlayerDiscard + 1);
	printf("- other player's top discard: %d, expected: %d\n", G.discard[otherPlayer][0], revealedCard);
	MY_ASSERT(G.discard[otherPlayer][0] == revealedCard);

	// current player trashes 2 village cards - not in hand, not in discard
	printf("- current player hand count: %d, expected: %d\n", G.handCount[currentPlayer], handCountBefore - 3);
	MY_ASSERT(G.handCount[currentPlayer] == handCountBefore - 3);
	printf(" - current player discard count: %d, expected: %d\n", G.discardCount[currentPlayer], discardCountBefore + 1);
	MY_ASSERT(G.discardCount[currentPlayer] == discardCountBefore + 1);

	printf("\nFinished testing Ambassador Card Effects\n\n\n");
	return 0;
}
