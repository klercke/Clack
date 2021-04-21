/******************************************************************************
* File:             drawDice.c
*
* Author:           Konnor Klercke [klercke <at> prototypexenon <dot> com]
* Created:          2021/04/20 
* Description:      Dice drawing program for clack
*****************************************************************************/

#include <stdio.h>

// Draw a small d4 showing a value of face
// 	  _
//   / \.
//  / 4 \.
// /_____\.
void drawSmalld4(int face) {
	printf("   _\n");
	printf("  / \\\n");
	printf(" / %d \\\n", face);
	printf("/_____\\\n");
}


// Draw a small d6 showing a value of face
//  _____
// |     |
// |  6  |
// |_____|
void drawSmalld6(int face) {
	printf(" _____\n");
	printf("|     | \n");
	printf("|  %d  | \n", face);
	printf("|_____| \n");
}


// Draw a small d8 showing a value of face
// 	  _
//   / \.
//  / 8 \.
// /_____\.
void drawSmalld8(int face) {
	printf("   _\n");
	printf("  / \\\n");
	printf(" / %d \\\n", face);
	printf("/_____\\\n");
}


// Draw a small d10 showing a value of face
//   _
//  / \.
// / 0 \.
// "._."
void drawSmalld10(int face) {
	// A 10 on a d10 is displayed as a 0
	face = face % 10;
	printf("   _\n");
	printf("  / \\\n");
	printf(" / %d \\\n", face);
	printf(" \"._.\"\n");
}


// Draw a small d12 showing a value of face
//   __
// ."  ".
// \ 12 /
//  ----
void drawSmalld12(int face) {
	printf("   __\n");
	printf(" .\"  \".\n");
	printf(" \\ %2d /\n", face);
	printf("  ----\n");
}


// Draw a small d20 showing a value of face
//    /\.
//   /  \.
//  / 20 \.
// /______\.
void drawSmalld20(int face) {
	printf("   /\\\n");
	printf("  /  \\\n");
	printf(" / %2d \\\n", face);
	printf("/______\\\n");
}
