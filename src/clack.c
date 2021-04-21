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

#include "drawDice.h"


// Flags:
_Bool VERBOSE_MODE = 0;
_Bool VERY_VERBOSE_MODE = 0;
_Bool AVERAGE_MODE = 0;
_Bool MEDIAN_MODE = 0;
_Bool MODE_MODE = 0;
_Bool PRETTY_PRINT_MODE = 0;


// Print input if VERBOSE_MODE is set
void vprint(char* input, ...) {
	va_list args;
	
	if (!VERBOSE_MODE) {
		return;
	}

	va_start(args, input);
	vprintf(input, args);
	va_end(args);
}


// Print input if VERY_VERBOSE_MODE is set
void vvprint(char* input, ...) {
	va_list args;
	
	if (!VERY_VERBOSE_MODE) {
		return;
	}

	va_start(args, input);
	vprintf(input, args);
	va_end(args);
}


// Draw a die if PRETTY_PRINT_MODE is set, otherwise just print the number
// int size:	Type of die. Accepted types are 4, 6, 8, 10, 12, 20
// int face:	The face of the die to print
void prettyPrintDie(int size, int face) {
	if (!PRETTY_PRINT_MODE) {
		// Normal printing
		printf("%d", face);
		return;
	}

	switch (size) {
		case 4:
			drawSmalld4(face);
			break;
		case 6:
			drawSmalld6(face);
			break;
		case 8:
			drawSmalld8(face);
			break;
		case 10:
			drawSmalld10(face);
			break;
		case 12:
			drawSmalld12(face);
			break;
		case 20:
			drawSmalld20(face);
			break;
	}
}


// Rolls xdy and stores the value in results such that results[n] is equal to
// the number of dice which rolled an n
//
// int x:			the number of dice to roll
// int y:			the number of sides on each die
// int* results:	pointer to array in which results should be stored
void rollxdy(int x, int y, int* results) {
	// RNG setup (bad, improve later)
	struct timeval t;
	gettimeofday(&t, NULL);
	srand((long) &t);

	vvprint("Results:\n");
	for (int i = 0; i < x; i++) {
		// pick a random number between 1 and y
		int thisDie = ((rand() % y) + 1);
		// increment this result
		results[thisDie]++;
		if (PRETTY_PRINT_MODE && VERY_VERBOSE_MODE) {
			printf("Die number %d:\n", i + 1);
			prettyPrintDie(y, thisDie);
			printf("\n");
		}
		else {
			vvprint("\tDice number %d result: %d\n", i + 1, thisDie);
		}
	}
}


// Given x and y, process rolls and any information requested by the user
//
// int x:	the number of dice to roll
// int y:	the number of faces on each die
void processDice(int x, int y) {
	long total = 0;
	// diceRolls will be an array such that diceRolls[n] is equal to the
	// number of dice that rolled n
	int* diceRolls;
	diceRolls = malloc((y + 1) * sizeof(int));
	// Build the diceRolls array
	for (int i = 0; i < y + 1; i++) {
		diceRolls[i] = 0;
	}
	// Populate diceRolls
	rollxdy(x, y, diceRolls);

	// Add up the total of all dice rolled
	vprint("Total calculation:\n");
	for (int i = 1; i <= y; i++) {
		// Since diceRolls[i] is equal to the number of dice which rolled an
		// i, we have to multilply by i to get the total
		total += diceRolls[i] * i;
		if (diceRolls[i] > 0) {
			vvprint("\t%d dice rolled a %d. ", diceRolls[i], i);
			vvprint("New total: %ld\n", total);
		}
	}
	printf("Result from rolling %dd%d: %ld\n", x, y, total);

	if (AVERAGE_MODE) {
		// Average is easy. Since x is the number of dice rolled, the average
		// is just total / x.
		double average = (double) total / (double) x;
		printf("Average roll: %f\n", average); 
	}

	if (MEDIAN_MODE) {
		// For median, we have to convert the array of results to an ordered
		// list. For example, [1, 3, 2, 1] would become [1, 2, 2, 2, 3, 3, 4].
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

		// Now that we have this list, we can calculate the median.
		if (x % 2 == 0){
			// Even number of items means we average the two medians
			int med1 = diceRollList[(x / 2) - 1];
			int med2 = diceRollList[x / 2];

			double median = ((float) med1 + (float) med2) / 2;

			printf("Median roll: %f\n", median);
		}
		else {
			// Odd number of items means we can just find one median and use that

			int median = diceRollList[(x / 2)];
			printf("Median roll: %d\n", median);
		}
	}

	if (MODE_MODE) {
		// Mode is relatively simple. Since diceRolls[n] equals the number of
		// dice which rolled an n, we simply have to find which values for n
		// give us the highest value for diceRolls[n].
		vprint("Mode calcultion:\n");
		int topScore = 0;
		int topScorer = 0;

		// Find the number of dice which rolled the most common result
		for (int i = 0; i < y; i++) {
			if (diceRolls[i] > topScore) {
				topScore = diceRolls[i];
				topScorer = i;
				vprint("\tNew most common result: %d", topScorer); 
				vprint(" appearing %d times\n", topScore); 
			}
		}
		// Find all results which are tied for most common  
		int topScorers[y];
		int topScoreIdx = 0;
		for (int i = 0; i < y; i++) {
			if (diceRolls[i] == topScore) {
				topScorers[topScoreIdx] = i;
				topScoreIdx++;
			}
		}

		// Print the single mode or all tied modes
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
}


// Take an input in the form xdy, find x and y, and call processDice with those
// values.
//
// char* input:	The text to parse
void strtoxy(const char* input) {
	int x, y;
	int lastDie = 0;
	for (int i = 0; input[i]; i++){
		if (input[i] == 'd') {
			if (i == 0){
				vprint("\nNumber of dice: 1\n");
				y = strtol(&input[1], NULL, 10);
				vprint("Size of dice: %d\n", y);
				processDice(1, y);
			}
			else if (input[i - 1] == ' ') {
				vprint("\nNumber of dice: 1\n");
				y = strtol(&input[1], NULL, 10);
				vprint("Size of dice: %d\n", y);
				processDice(1, y);
			}	
			else {
				x = strtol(&input[lastDie], NULL, 10); 
				vprint("\nNumber of dice: %d\n", x);
				y = strtol(&input[i + 1], NULL, 10);
				vprint("Size of dice: %d\n", y);
				processDice(x, y);
			}
		}
		
		else if (input[i] == ' ') {
			lastDie = i;
		}	
	}
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
	while ((opt = getopt(argc, argv, ":hVvamMp")) != -1) {
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
			case 'p':
				PRETTY_PRINT_MODE = 1;
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
