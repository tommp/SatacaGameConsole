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

//Frequency of the timer clock
#define TIMER_FREQUENCY 32678

typedef struct Tune{
	//The note of the tune
	char note; 
	//The octave of the tune
	int octave;	
	//Determines time to play the tune (in miliseconds)
	int time_to_play_ms; 
} Tune;

void play_tune(volatile Tune* tune);
void playCurrentAndSetNextTune(volatile Tune** tune,volatile uint32_t length,volatile uint32_t* current);

#endif
