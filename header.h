#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define HASH_MAX 50000

#define TRUE 1
#define FALSE 0

int prompt(char *string);

typedef struct ln{
	char *key;
	int value;
	struct ln *next;
} ListNode;

typedef struct{
	ListNode *nodes[HASH_MAX];
} Map;


Map newmap();
int mapget(Map *mp, char *key);
void mapset(Map *mp, char *key, int value);

void basicCounts(FILE *fp);
void countCharFrequency(FILE *fp);
void wordHistPrompt(FILE *fp);
void charHistPrompt(FILE *fp);
