/*******************************************************************************
** Name: Samantha Guilbeault
** Date: 11-14-2019
** Description: Random tester for function tributeCardEffect()
** Parameters: struct gameState *state and int currentPlayer
** Sources: Lecture notes from Lesson: Random Testing 1
*******************************************************************************/

#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>

// source: http://www.dillonbhuff.com/?p=439
#define MY_ASSERT(x, y, z) if (!(x)) { printf("   *Assertion Failed: Test %d - %s\n", z, y); }


void checkTributeEffect(int cP, int nP, struct gameState *post, int testNum);

int main()
{
	// init test variables
	int currentPlayer, nextPlayer;

	// declare the game state
	struct gameState G;

	// seed rngs
	SelectStream(2);
	PutSeed(3);

	printf("Testing tributeCardEffect...\n");
	printf("RANDOM TESTS\n");

	// run test n times
	for (int n = 0; n < 25; n++)
	{
		// initialize gameState with random values
		for (int i = 0; i < sizeof(struct gameState); i++)
		{
			((char*)&G)[i] = floor(Random() * 256);
		}

		// refine values that matter to tributeCardEffect
		G.numPlayers = floor(Random() * MAX_PLAYERS) + 2;  // make sure valid number in numPlayers
		currentPlayer = floor(Random() * G.numPlayers); // currentPlayer is a valid player number
		G.whoseTurn = currentPlayer;  // tribute expects whoseTurn to be currentPlayer
		nextPlayer = currentPlayer + 1;  // get value for the next player
		if (nextPlayer > (G.numPlayers - 1)) { nextPlayer = 0; }  // makes sure next player wraps around if needed
		for (int i = 0; i < G.numPlayers; i++)  // every player needs a valid hand, discard, deck count
		{
			G.handCount[i] = floor(Random() * 100);  // use 100 to lower chance of overflow
			G.discardCount[i] = floor(Random() * 100);
			G.deckCount[i] = floor(Random() * 100);
		}
		for (int i = 0; i < G.discardCount[nextPlayer]; i++)  // next player needs valid cards in discard pile
		{
			G.discard[nextPlayer][i] = floor(Random() * 26);
		}
		for (int i = 0; i < G.deckCount[nextPlayer]; i++)
		{
			G.deck[nextPlayer][i] = floor(Random() * 26);  // next player needs valid cards in deck pile
		}
		G.playedCardCount = floor(Random() * 100);  // make sure played card count is valid
		G.coins = floor(Random() * 1000);  // make sure coins is positive
		G.numActions = floor(Random() * 1000);  // make sure actions is positive

		checkTributeEffect(currentPlayer, nextPlayer, &G, n);
	}

	printf("TESTING COMPLETE\n");
	return 0;
}


void checkTributeEffect(int cP, int nP, struct gameState *post, int testNum)
{
	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	int result = tributeCardEffect(post, cP);

	int revealedCards[2] = { -1, -1 };
	if (pre.discardCount[nP] + pre.deckCount[nP] <= 1)  // next player has 1 or less cards to reveal
	{
		if (pre.deckCount[nP] > 0)  // the card is in the deck 
		{
			revealedCards[0] = pre.deck[nP][pre.deckCount[nP] - 1];
			pre.deckCount[nP]--;
		}
		else if (pre.discardCount[nP] > 0)  // the card is in the discard
		{
			revealedCards[0] = pre.discard[nP][pre.discardCount[nP] - 1];
			pre.discardCount[nP]--;
		}
	}
	else if (pre.deckCount[nP] > 1)  // next player has more than 1 card and they are in the deck
	{
		revealedCards[0] = pre.deck[nP][pre.deckCount[nP] - 1];
		pre.deckCount[nP]--;

		revealedCards[1] = pre.deck[nP][pre.deckCount[nP] - 1];
		pre.deckCount[nP]--;
	}

	for (int i = 0; i < 2; i++)
	{
		if (revealedCards[i] < 0)  // do not proceed if invalid cards
		{
			break;
		}

		//Treasure cards
		if (revealedCards[i] == copper || revealedCards[i] == silver || revealedCards[i] == gold)
		{
			pre.coins += 2;
		}
		//Victory Card Found
		else if (revealedCards[i] == estate || revealedCards[i] == duchy || revealedCards[i] == province
			|| revealedCards[i] == gardens || revealedCards[i] == great_hall)
		{
			pre.handCount[cP] += 2;
		}
		//Action Card
		else
		{
			pre.numActions += 2;
		}

		pre.discardCount[nP]++;

		if (revealedCards[0] == revealedCards[1])  // only loop once if revealed cards are the same
		{
			pre.discardCount[nP]++;
			break;
		}
	}

	MY_ASSERT(result == 0, "Unexpected return value from tributeCardEffect", testNum);
	MY_ASSERT(pre.deckCount[nP] == post->deckCount[nP], "Invalid next player deck count", testNum);
	MY_ASSERT(pre.discardCount[nP] == post->discardCount[nP], "Invalid next player discard count", testNum);
	MY_ASSERT(pre.coins == post->coins, "Invalid coin count", testNum);
	MY_ASSERT(pre.numActions == post->numActions, "Invalid number of actions", testNum);
	MY_ASSERT(pre.handCount[cP] == post->handCount[cP], "Invalid current player hand count", testNum);
}