#ifndef TUNES_MUSIC_LIB_H
#define TUNES_MUSIC_LIB_H

#include <stdlib.h>
#include <math.h>

//Standard frequencies (approxximated)
#define A_FREQUENCY 27
#define B_FREQUENCY 30
#define C_FREQUENCY	16
#define D_FREQUENCY	18
#define E_FREQUENCY	20
#define F_FREQUENCY 22
#define G_FREQUENCY 24

//If set to 0, expects one-shot mode.
#define USE_FREE_RUN 0

//Set to one if using low energy timer
#define USE_LET 0

//Frequency of the timer clock
#define TIMER_FREQUENCY 32678

#define TOP_VALUE_REGISTER 0xlolfochizzle
#define TIME_TICKS_REGISTER 0xchizzlemahnizzle

//imperial march:
char* letters = "gggebgebg";
int * times = {500,500,500,500,200,500,500,200,500};
int length = 9;
int octave = 3;

//Linked list node for a tune
typedef struct Tune{
	//Pointer to the following tune node
	struct Tune* next;
	//The note of the tune
	char note; 
	//The octave of the tune
	int octave;	
	//Determines time to play the tune (in miliseconds)
	int time_to_play_ms; 
} Tune;

//Linked list of tunes representing a song
typedef struct Song{
	//Pointer to the first note in the sequence
	struct Tune* first;
	//Pointer to the current note (the one to be played)
	struct Tune* current;
} Song;

int play_tune(Tune* tune);
int playCurrentAndSetNextTune(Song* song);
int construct_song(char* notes, int* timers, Song* song, int length, int octave);
int destroy_song(Song* song);

#endif