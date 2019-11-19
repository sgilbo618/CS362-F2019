/*******************************************************************************
** Name: Samantha Guilbeault
** Date: 11-14-2019
** Description: Random tester for function minionCardEffect()
** Parameters: struct gameState *state, int choice1, int currentPlayer, int handPos
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


void checkMinionEffect(int choice1, int cP, struct gameState *post, int handPos, int testNum);

int main()
{
	// init test variables
	int choice1, currentPlayer, handPos;
	struct gameState G;

	// seed rngs
	SelectStream(2);
	PutSeed(3);

	printf("Testing minionCardEffect...\n");
	printf("RANDOM TESTS\n");

	// run test n times
	for (int n = 0; n < 100; n++)
	{
		// initialize gameState with random values
		for (int i = 0; i < sizeof(struct gameState); i++)
		{
			((char*)&G)[i] = floor(Random() * 256);
		}

		// refine values that matter to baronCardEffect
		choice1 = floor(Random() * 2) + 1;  // make choice1 either 1 or 2
		G.numPlayers = floor(Random() * MAX_PLAYERS) + 1;  // make sure valid number of players
		currentPlayer = floor(Random() * G.numPlayers); // currentPlayer is a valid player number based on how many players there are
		for (int i = 0; i < G.numPlayers; i++)  // every player needs a valid hand, discard, deck count
		{
			G.handCount[i] = floor(Random() * 100);  // use to lower chance of overflow
			G.discardCount[i] = floor(Random() * 100);
			G.deckCount[i] = floor(Random() * 100);
		}
		handPos = floor(Random() * G.handCount[currentPlayer]);  // get a random position in the hand
		G.hand[currentPlayer][handPos] = minion;  // make sure Minion card is in the correct position
		G.coins = floor(Random() * 1000);  // make sure coins is positive

		checkMinionEffect(choice1, currentPlayer, &G, handPos, n);
	}

	printf("TESTING COMPLETE\n\n");
	return 0;
}


void checkMinionEffect(int choice1, int cP, struct gameState *post, int handPos, int testNum)
{
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	int result = minionCardEffect(post, choice1, cP, handPos);

	pre.numActions += 1;  // increment actions no matter what choice is
	pre.discard[cP][pre.discardCount[cP]] = minion;  // discard Minion
	pre.discardCount[cP]++;  // increment discard count
	pre.handCount[cP]--;  // decrement hand count

	if (choice1 == 1)
	{
		pre.coins += 2;  // add two coins
		MY_ASSERT(pre.discard[cP][pre.discardCount[cP] - 1] == post->discard[cP][post->discardCount[cP] - 1], "Invalid card discarded", testNum);
	}
	else if (choice1 == 2)
	{
		for (int i = 0; i < pre.numPlayers; i++)
		{
			if (i == cP || pre.handCount[i] > 4)  // only redraw if current player or have more than 4 cards
			{
				pre.discardCount[i] += pre.handCount[i];  // emulate discarding
				pre.handCount[i] = 4;  // emulate redrawing 4
			}
		}
	}

	MY_ASSERT(result == 0, "Unexpected return value", testNum);
	MY_ASSERT(pre.numActions == post->numActions, "Invalid number of actions", testNum);
	MY_ASSERT(pre.coins == post->coins, "Invalid number of coins", testNum);
	MY_ASSERT(pre.discardCount[cP] == post->discardCount[cP], "Invalid number of discarded cards", testNum);
	for (int i = 0; i < pre.numPlayers; i++)
	{

		MY_ASSERT(pre.handCount[i] == post->handCount[i], "Invalid hand count", testNum);
	}
}