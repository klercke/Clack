/******************************************************************************
* File:             clack.c
*
* Author:           Konnor Klercke [klercke <at> prototypexenon <dot> com]
* Created:          2021/04/03 
* Description:      Main file for clack, the command-line dice roller
*****************************************************************************/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdarg.h>

// Flags:
_Bool VERBOSE_MODE = 0;
_Bool VERY_VERBOSE_MODE = 0;
_Bool AVERAGE_MODE = 0;
_Bool MEDIAN_MODE = 0;
_Bool MODE_MODE = 0;


void vprint(char* input, ...) {
	va_list args;
	
	if (!VERBOSE_MODE) {
		return;
	}

	va_start(args, input);
	vprintf(input, args);
	va_end(args);
}


void vvprint(char* input, ...) {
	va_list args;
	
	if (!VERY_VERBOSE_MODE) {
		return;
	}

	va_start(args, input);
	vprintf(input, args);
	va_end(args);
}


void rollxdy(int x, int y, int* results) {
	struct timeval t;
	gettimeofday(&t, NULL);
	srand((long) &t);

	vvprint("Results:\n");
	for (int i = 0; i < x; i++) {
		// add a random number between 1 and y
		int thisdie = ((rand() % y) + 1);
		results[thisdie]++;
		vvprint("\tDice number %d result: %d\n", i , thisdie);
	}
}


int strtoxy(const char* input) {
	int x, y;
	for (int i = 0; input[i]; i++){
		if (input[i] == 'd') {
			if (i == 0){
				x = 1;
				vprint("Number of dice: 1\n");
				y = strtol(&input[1], NULL, 10);
				vprint("Size of dice: %d\n", y);
			}
			else if (input[i - 1] == ' ') {
				x = 1;
				vprint("Number of dice: 1\n");
				y = strtol(&input[1], NULL, 10);
				vprint("Size of dice: %d\n", y);
			}	
			else {
				x = strtol(&input[0], NULL, 10); 
				vprint("Number of dice: %d\n", x);
				y = strtol(&input[i + 1], NULL, 10);
				vprint("Size of dice: %d\n", y);
			}
		}
	}

	long total = 0;
	int* diceRolls;
	// diceRolls will be an array such that diceRolls[x] is equal to the
	// number of dice that rolled x
	diceRolls = malloc((y + 1) * sizeof(int));
	rollxdy(x, y, diceRolls);

	vprint("Total calculation:\n");
	for (int i = 1; i <= y; i++) {
		total += diceRolls[i] * i;
		if (diceRolls[i] > 0) {
			vvprint("\t%d dice rolled a %d. ", diceRolls[i], i);
			vvprint("New total: %ld\n", total);
		}
	}
	printf("Result from rolling %dd%d: %ld\n", x, y, total);

	if (AVERAGE_MODE) {
		double average = (double) total / (double) x;
		printf("Average roll: %f\n", average); 
	}

	if (MEDIAN_MODE) {
		// convert "array" to "list"
		vprint("Median calculation:\n");
		int diceRollList[x];
		int listIdx = 0;
		for (int i = 0; i < y; i++) {
			if (diceRolls[i] > 0) {
				vvprint("\t%d dice rolled a %d\n", diceRolls[i], i);
				for (int j = diceRolls[i]; j > 0; j--) {
					diceRollList[listIdx] = i;
					listIdx++;
				}	
			}	
		}
		
		if (VERY_VERBOSE_MODE) {
			printf("\tOrdered list of results: [");
			for (int i = 0; i < listIdx - 1; i++) {
				printf("%d, ", diceRollList[i]);
			}
			printf("%d]\n", diceRollList[listIdx - 1]);
		}

		if (x % 2 == 0){
			// even, median calculation is more difficult
			int med1 = diceRollList[(x / 2) - 1];
			int med2 = diceRollList[x / 2];

			double median = ((float) med1 + (float) med2) / 2;

			printf("Median roll: %f\n", median);
		}
		else {
			// odd number of items

			int median = diceRollList[(x / 2)];
			printf("Median roll: %d\n", median);
		}
	}

	if (MODE_MODE) {
		vprint("Mode calcultion:\n");
		int topScore = 0;
		int topScorer = 0;

		//find most common results
		for (int i = 0; i < y; i++) {
			if (diceRolls[i] > topScore) {
				topScore = diceRolls[i];
				topScorer = i;
				vvprint("\tNew most common result: %d", topScorer); 
				vvprint(" appearing %d times\n", topScore); 
			}
		}
		int topScorers[y];
		int topScoreIdx = 0;
		for (int i = 0; i < y; i++) {
			if (diceRolls[i] == topScore) {
				topScorers[topScoreIdx] = i;
				topScoreIdx++;
			}
		}

		if (topScoreIdx == 1) {
			printf("Mode result: %d\n", topScorer);
		}
		else {
			printf("Mode results: ");
			for (int i = 0; i < topScoreIdx - 1; i++){
				printf("%d, ", topScorers[i]);
			}
			printf("%d\n", topScorers[topScoreIdx - 1]);
		}
	}
   
	free(diceRolls);
	return 0;	

}


void printHelp() {
	printf("Usage: clack [OPTION] XdY...\n");
	printf("Roll X Y-sided dice and print the result\n");
	printf("Can roll multiple sets of XdY if multiple are specified\n\n");

	printf("\t-h\t\tprint this help text\n");
	printf("\t-v\t\tprint more information about processes\n");
	printf("\t-V\t\tprint the most information\n");
	printf("\t-a\t\tprint the average roll\n");
	printf("\t-m\t\tprint the median roll\n");
	printf("\t-M\t\tprint the mode roll\n");
}


int main(int argc, char** argv) {
	int opt;
	_Bool verbose, very_verbose = 0;
	while ((opt = getopt(argc, argv, ":hVvamM")) != -1) {
		switch (opt) {
			case 'h':
				printHelp();
				break;
			case 'V':
				very_verbose = 1;
				break;
			case 'v':
				verbose = 1;
				break;
			case 'a':
				AVERAGE_MODE = 1;
				break;
			case 'm':
				MEDIAN_MODE = 1;
				break;
			case 'M':
				MODE_MODE = 1;
				break;
			case ':':
				printf("option %c needs a value\n", opt);
				break;
			case '?':
				printf("unknown option: %c\n", optopt);
				break;
	 	}
	}

	
	if (very_verbose) {	
		VERBOSE_MODE = 1;
		VERY_VERBOSE_MODE = 1;
		printf("Using very verbose mode\n");
	}
	else if (verbose) {
		printf("Using verbose mode\n");
		VERBOSE_MODE = 1;
	}

	for(; optind < argc; optind++) {     
		strtoxy(argv[optind]);
	}

	return 0;
}
