#include "header.h"

int countIf(FILE *fp, int (*condition)(char)){
	int c, total = 0;
	while((c = getc(fp)) != EOF){
		if(condition(c))
			total++;
	}    
	rewind(fp);
	return total;
}

int countWords(FILE *fp){
	int total = 0;
	int inword = 0;
	int c;
	while((c = getc(fp)) != EOF){
		if(c == ' ' || c == '\t' || c == '\n'){
			if(inword){
				inword = 0;
				total++;
			}
		}else{
			inword = 1;
		}
	}
	rewind(fp);
	return total + inword;
}

int isNonwhite(char c){
	return !(c == ' ' || c == '\t' || c == '\n');
}

int isAnything(char c){
	return 1;
}

int isNewline(char c){
	return (c == '\n');    
}

char matchChar;
int isMatch(char c){
	return (c == matchChar);
}


void basicCounts(FILE *fp){
	printf("\n");
	printf("Words: %d\n", countWords(fp)); 
	printf("Characters: %d\n", countIf(fp, isAnything)); 
	printf("Nonwhite Characters: %d\n", countIf(fp, isNonwhite)); 
	printf("Lines: %d\n", 1 + countIf(fp, isNewline)); 
}

void countCharFrequency(FILE *fp){
	matchChar = prompt("\nCharacter to match:");
	printf("\nMatches to '%c': %d\n", matchChar, countIf(fp, isMatch)); 
}
