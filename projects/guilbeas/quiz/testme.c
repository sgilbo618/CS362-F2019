/*
Name: Samantha Guilbeault
Date: 11-4-19
Description: Random Tester Quiz
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    // generate random values from ASCII 32(space) through 125(})
	char random_char = rand() % (126 - 32) + 32; 

    return random_char;
}

char *inputString()
{
	static char random_string[6] = { '\0' };

	// generate random values from ASCII 101(e) through 116(t)
	for (int i = 0; i < 5; i++)
	{
		random_string[i] = rand() % (117 - 101) + 101;
	}
	random_string[5] = '\0';  // null terminate string

    return random_string;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
	if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
