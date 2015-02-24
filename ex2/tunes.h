#ifndef TUNES_MUSIC_LIB_H
#define TUNES_MUSIC_LIB_H

#include <stdlib.h>
#include <math.h>
#include "efm32gg.h"

//Standard frequencies (approxximated)
#define A_FREQUENCY 27
#define B_FREQUENCY 30
#define C_FREQUENCY 16
#define D_FREQUENCY 18
#define E_FREQUENCY 20
#define F_FREQUENCY 22
#define G_FREQUENCY 24

//If set to 0, expects one-shot mode.
#define USE_FREE_RUN 0

//Frequency of the timer clock
#define TIMER_FREQUENCY 32678


//Linked list node for a tune
typedef struct Tune{
	//The note of the tune
	char note; 
	//The octave of the tune
	int octave;	
	//Determines time to play the tune (in miliseconds)
	int time_to_play_ms; 
} Tune;

Tune g500ms = { .note = g, .octave = 4, .time_to_play_ms = 500 };
Tune e500ms = { .note = e, .octave = 4, .time_to_play_ms = 500 };
Tune b200ms = { .note = e, .octave = 4, .time_to_play_ms = 200 };

Tune imperial_march[9] = {&g500ms,&g500ms,&g500ms,&e500ms,&b200ms,&g500ms,&e500ms,&b200ms,&sg500ms};
int length = 9;
int current = 0;


int play_tune(Tune* tune);
int playCurrentAndSetNextTune(Tune** tune, int length, int* current);

#endif
