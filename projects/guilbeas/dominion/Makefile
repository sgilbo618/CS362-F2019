CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/

unit: unittest5.c  dominion_helpers.h dominion.o rngs.o
	gcc -o t unittest5.c -g dominion.o rngs.o $(CFLAGS)

testBaron: unittest1.c dominion_helpers.h dominion.o rngs.o
	gcc -o testBaron unittest1.c -g dominion.o rngs.o $(CFLAGS)

testMinion: unittest2.c dominion_helpers.h dominion.o rngs.o
	gcc -o testMinion -g unittest2.c dominion.o rngs.o $(CFLAGS)

testAmbassador: unittest3.c dominion_helpers.h dominion.o rngs.o
	gcc -o testAmbassador -g unittest3.c dominion.o rngs.o $(CFLAGS)

testTribute: unittest4.c dominion_helpers.h dominion.o rngs.o
	gcc -o testTribute -g unittest4.c dominion.o rngs.o $(CFLAGS)

testMine: unittest5.c dominion_helpers.h dominion.o rngs.o
	gcc -o testMine -g unittest5.c dominion.o rngs.o $(CFLAGS)

unittestresults: testBaron testMinion testAmbassador testTribute testMine
	./testBaron &> unittestresults.out
	./testMinion >> unittestresults.out
	./testAmbassador >> unittestresults.out
	./testTribute >> unittestresults.out
	./testMine >> unittestresults.out
	gcov -f -b dominion.c >> unittestresults.out

testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
