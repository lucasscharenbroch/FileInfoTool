#include "header.h"

#define BYTE_COMPACITY 128
#define MAX_WORD_LENGTH 100
#define MAX_WORDS 100000
#define HISTOGRAM_LINE_MAX 150

char *tostring(char c){/* '\n' -> "\n" */
	static char str[2];

	str[0] = c;
	str[1] = '\0';

	return str;
}

char *toreadstring(char c){/* '\n' -> "\\n" */
	static char str[5];
	switch(c){
		case '\n':
			strcpy(str, "\\n");
			break;
		case '\t':
			strcpy(str, "\\t");
			break;
		case ' ':
			strcpy(str, "\" \"");
			break;
		case '\0':
			strcpy(str, "EOF");
			break;
		default:
			str[0] = c;
			str[1] = '\0';
			break;
	}
	return str;
}

char *xstring(int len){/* returns string of x's of length len */
	static char str[101];
	int i;

	for(i = 0; i < len; i++){
		str[i] = 'X';
	}
	str[i] = '\0';
	return str;
}


void drawHist(char **words, int numwords, Map *mp){
	/* set up histogram */
	int max = 0; /* largest total count */
	int current = 0; /* current total count */

	for(int i = 0; i < numwords; i++){/* find max to set scale */
		current = mapget(mp, words[i]);
		max = (current > max) ? current : max;
	}
	
	int scale = 1 + (max / HISTOGRAM_LINE_MAX);

	/* for each word, look it up in the map and print it out */
	printf("\n Word: Frequency (X = %d)\n", scale);/*label */
	for(int i = 0; i < numwords; i++){
		current = mapget(mp, words[i]);
		
		if(strlen(words[i]) == 1)/* single character word */
			printf("%15s: %s (%d)\n", toreadstring(words[i][0]), xstring(current/scale + 1), current);
		else
			printf("%15s: %s (%d)\n", words[i], xstring(current/scale + 1), current);
	}
}

void wordSwap(char** words, int a, int b){
	char *temp = words[a];
	words[a] = words[b];
	words[b] = temp;
}

void mergeSort(char **arr, int length, Map *mp, int (*comparison)(char *, char *, Map *)){
	if(length <= 1) 
		return;

	/* split array in half */
	int mid = length/2;


	/* sort both halves */
	mergeSort(arr, mid, mp, comparison);
	mergeSort(arr+mid, length-mid, mp, comparison);

	/* combine both halves */
	int i = 0, j = mid;
	int tempj;

	while(i < mid && j < length){
		if(!comparison(arr[i], arr[j], mp)) /* i < j */
			i++;
		else{
			/* j < i */
			/* swap i and j (placing j into sorted array */
			wordSwap(arr, i, j);
			i++;

			/* put value previously in arr[i] into the correct place in j's array */
			tempj = j;
			while(tempj+1 < length && comparison(arr[tempj], arr[tempj+1], mp)){/* arr[tempj] > arr[tempj+1] */
				wordSwap(arr, tempj, tempj+1);
				tempj++;
			}
		}
	}
}

int ascending(char *a, char *b, Map *mp){
	return mapget(mp, a) > mapget(mp, b);
}
int descending(char *a, char *b, Map *mp){
	return mapget(mp, a) < mapget(mp, b);
}
int alphabetical(char *a, char *b, Map *mp){
	if(*a == *b){
		if(*a == '\0')/* end of word reached */
			return TRUE;
		else
			return alphabetical(a+1, b+1, mp);
	}
	return tolower(*a) > tolower(*b);
}
int reversealphabetical(char *a, char *b, Map *mp){
	if(*a == *b){
		if(*a == '\0')/* end of word reached */
			return TRUE;
		else
			return alphabetical(a+1, b+1, mp);
	}
	return tolower(*a) < tolower(*b);
}


int nofilter(int c){
	return c;
}


/* generic histogram function */
void histPrompt(char **words, int numwords, Map *mp){
	/* prompt for histogram layout */
	printf("\nSort by?\n");

	printf("1.) Frequency, ascending\n");
	printf("2.) Frequency, descending\n");
	printf("3.) Order of appearance\n");
	printf("4.) Alphabetical\n");
	printf("5.) Reverse-Alphabetical\n");
	printf("6.) Cancel\n");

	switch(prompt("\n")){
		case '1':
			mergeSort(words, numwords, mp, ascending);
			break;
		case '2':
			mergeSort(words, numwords, mp, descending);
			break;
		case '3':
			/* do nothing (already in order of appearance) */
			break;
		case '4':
			mergeSort(words, numwords, mp, alphabetical);
			break;
		case '5':
			mergeSort(words, numwords, mp, reversealphabetical);
			break;
		case '6': default:
			return;

	}

	/* print histogram */
	drawHist(words, numwords, mp);
}

void charHistPrompt(FILE *fp){
	Map m = newmap();
	char *words[BYTE_COMPACITY];/* chars listed as "words" */
	int c, numwords = 0;

	int (*filter)(int);
	c = prompt("Ignore letter-cases? (y/n)?\n");

	switch(tolower(c)){
		case 'y':
			filter = tolower;
			break;
		case 'n':
			filter = nofilter;
			break;
		default:
			return;
	}

	
	char str[2];
	char *word;
	/* read chars from file*/
	while((c = getc(fp)) != EOF){
		c = filter(c);
		strcpy(str, tostring(c));

		if(!mapget(&m, str)){/* if c hasn't been seen before */
			word = malloc(2); 
			strcpy(word, str);/* copy c to words */
			words[numwords++] = word;
			mapset(&m, word, 1); /* init to 1 */
		}else{
			mapset(&m, str, mapget(&m, str)+1);/* increment m[c] */ 
		}
	}

	histPrompt(words, numwords, &m);

	/* free word pointers */
	for(int i = 0; i < numwords; i++){
		free(words[i]);
	}

	rewind(fp);
}

void wordHistPrompt(FILE *fp){
	Map m = newmap();
	char *words[MAX_WORDS];/* chars listed as "words" */
	int c, numwords = 0;

	int (*filter)(int);
	c = prompt("Ignore letter-cases? (y/n)?\n");

	switch(tolower(c)){
		case 'y':
			filter = tolower;
			break;
		case 'n':
			filter = nofilter;
			break;
		default:
			return;
	}

	
	char str[MAX_WORD_LENGTH];
	char *word;
	int wordlength = 0;

	/* read chars from file*/
	while((c = getc(fp)) != EOF){
		if(c == ' ' || c == '\n' || c == '\t' || wordlength == MAX_WORD_LENGTH-1){
			if(wordlength != 0){
				str[wordlength++] = '\0';/* truncate word */

				if(!mapget(&m, str)){/* word hasn't been seen before */ 
					word = malloc(wordlength + 6);/*without the +6, free crashes.*/
					strcpy(word, str);	
					words[numwords++] = word;
					mapset(&m, str, 1);/* init to 1 */
				}else{
					mapset(&m, str, mapget(&m, str)+1);/* increment */
				}
				wordlength = 0;
			}
		}else{
			str[wordlength++] = filter(c);
		}
	}

	histPrompt(words, numwords, &m);

	/* free word pointers */
	for(int i = 0; i < numwords; i++){
		free((char*)words[i]);
	}

	rewind(fp);
}
