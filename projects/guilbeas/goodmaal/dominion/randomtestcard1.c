/*******************************************************************************
** Name: Samantha Guilbeault
** Date: 11-07-2019
** Description: Random tester for function baronCardEffect()
** Parameters: struct gameState *state, int choice1, int currentPlayer
** Sources: Lecture notes from Lesson: Random Testing 1
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

// source: http://www.dillonbhuff.com/?p=439
#define MY_ASSERT(x, y, z) if (!(x)) { printf("   *Assertion Failed: Test %d - %s\n", z, y); }


void checkBaronEffect(int handPos, int choice1, int cP, struct gameState *post, int *bonus, int hasEstate, int testNum);

int main()
{
	// init test variables
	int choice1, currentPlayer, hasEstate = 0, handPos = 0;
	int bonus = 0;
	// declare the game state
	struct gameState G;

	// seed rngs
	SelectStream(2);
	PutSeed(3);

	printf("Testing baronCardEffect...\n");
	printf("RANDOM TESTS\n");

	// run test n times
	for (int n = 0; n < 25; n++)
	{
		// initialize gameState with random values
		for (int i = 0; i < sizeof(struct gameState); i++)
		{
			((char*)&G)[i] = floor(Random() * 256);
		}

		// refine values that matter to baronCardEffect
		choice1 = floor(Random() * 2);  // make choice1 either true or false
		currentPlayer = floor(Random() * 2); // currentPlayer is a valid player number
		G.deckCount[currentPlayer] = floor(Random() * MAX_DECK);  // deck count has a valid count
		G.discardCount[currentPlayer] = floor(Random() * MAX_DECK); // discard count has a valid count
		G.handCount[currentPlayer] = floor(Random() * 11);  // hand count has valid count
		G.supplyCount[estate] = floor(Random() * 13); // estate supply count is valid and can be 0
		G.coins = floor(Random() * 1000); // make sure coin count is positive
		hasEstate = 0;  // reset boolean
		G.hand[currentPlayer][handPos] = baron;  // make sure baron in hand
		for (int i = 1; i < G.handCount[currentPlayer]; i++)  // each other card in hand is valid and can be estate
		{
			G.hand[currentPlayer][i] = floor(Random() * 11);
			if (G.hand[currentPlayer][i] == estate)
			{
				hasEstate = 1;
			}
		}

		checkBaronEffect(handPos, choice1, currentPlayer, &G, &bonus, hasEstate, n);
	}

	printf("TESTING COMPLETE\n");
	return 0;
}


void checkBaronEffect(int handPos, int choice1, int cP, struct gameState *post, int *bonus, int hasEstate, int testNum)
{
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	int result = baronCard(handPos, choice1, cP, post, bonus);

	pre.numBuys += 1;  // increase number of buys by one

	// discard estate to gain +4 coins
	if (choice1 > 0 && hasEstate)
	{
		pre.coins += 4;  // add 4 coins
		pre.discard[cP][pre.discardCount[cP]] = estate;  // estate ends up in discard pile
		pre.discardCount[cP]++;  // increase discard count by one
		memcpy(pre.hand[cP], post->hand[cP], sizeof(int) * pre.handCount[cP]);  // copy hand because we don't know where to shift hand from
		pre.handCount[cP]--;  // hand count decreases by one
	}
	// no estate, gain estate if there are any left
	else if (pre.supplyCount[estate] > 0)
	{
		pre.discard[cP][pre.discardCount[cP]] = estate;  // add estate to hand
		pre.discardCount[cP]++;  // increase discard count by one
		pre.supplyCount[estate]--;  // decrease supply count by one
	}

	MY_ASSERT(result == 0, "Unexpected return value from baronCardEffect", testNum);
	MY_ASSERT(pre.numBuys == post->numBuys, "Invalid number of buys", testNum);
	MY_ASSERT(pre.coins == post->coins, "Invalid number of coins", testNum);
	MY_ASSERT(pre.discardCount[cP] == post->discardCount[cP], "Invalid number of discards", testNum);
	MY_ASSERT(pre.discard[cP][pre.discardCount[cP] - 1] == post->discard[cP][post->discardCount[cP] - 1], "Invalid card discarded", testNum);
	MY_ASSERT(pre.handCount[cP] == post->handCount[cP], "Invalid hand count", testNum);
	MY_ASSERT(pre.supplyCount[estate] == post->supplyCount[estate], "Invalid supply of Estate cards", testNum);
}
