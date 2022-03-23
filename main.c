#include "header.h"

#define INPUT_ERROR 1
#define FILE_ERROR 2

void throwerror(int type){
	exit(type);
}


int prompt(char *string){
	printf("%s", string);
	int c = getchar();
	ungetc(c, stdin);

	while(getchar() != '\n')
		;

	return c;
}

void analyze(FILE *fp){
	int c;

	while(1){

		printf("\n");
		printf("Choose an operation:\n");

		printf("1.) Basic counts\n");
		printf("2.) Find frequency of a character\n");
		printf("3.) Word histogram\n");
		printf("4.) Character histogram\n");
		printf("5.) Exit\n");


		switch(prompt("\n")){
			case '1':
				basicCounts(fp);
				break;
			case '2':
				countCharFrequency(fp);
				break;
			case '3':
				wordHistPrompt(fp);
				break;
			case '4':
				charHistPrompt(fp);
				break;
			case '5':
				return;
			case '\n':
				break;
			default:
				printf("\ninvalid entry\n");
				break;
		}
	}
}


int main(int argc, char **argv){
	/* open file from argv[1] to filep */
	if(argc != 2){
		printf("Invalid number of arguments\n");
		throwerror(INPUT_ERROR);
	}

	char *filename = argv[1];
	FILE *filep = fopen(filename, "r");

	if(filep == NULL){
		printf("Problem opening file: %s\n", filename);
		throwerror(FILE_ERROR);
	}

	analyze(filep);

	return 0;
}
