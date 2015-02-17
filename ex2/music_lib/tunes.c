#include "tunes.h"
#include <math.h>

int play_tune(Tune* tune){

	switch(tune->note){
		case 'a':
			int tune_frequency = A_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'b':
			int tune_frequency = B_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'c':
			int tune_frequency = C_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'd':
			int tune_frequency = D_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'e':
			int tune_frequency = E_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'f':
			int tune_frequency = F_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'g':
			int tune_frequency = G_FREQUENCY*(pow(2,tune->octave));
			break;
		default:
			return -1;
	}

	if(tune_frequency < 20) {
		tune_frequency = 20;
	} else if(true_frequency > 20000){
		tune_frequency = 20000;
	}

	int top_value = TIMER_FREQUENCY/tune_frequency;
	*TOP_VALUE_REGISTER = top_value;

#if USE_FREE_RUN
#if USE_LET
	///LOW ENERGY START, REMEMBER TO CHECK IF ALREADY RUNNING
#else
	//NORMAL START
#endif
	return 0;
#else
	int num_overflows_to_run = ((TIMER_FREQUENCY/1000)*(tune->time_to_play_ms))/top_value;

	if(num_overflows_to_run <1){
		num_overflows_to_run = 1;
	} else if(num_overflows_to_run >255){
		num_overflows_to_run = 255;
	}

	*TIME_TICKS_REGISTER = num_overflows_to_run;

#if USE_LET
	///LOW ENERGY START, REMEMBER TO CHECK IF ALREADY RUNNING
#else
	//NORMAL START
#endif
	return 0;
#endif
	
}


//TThis should be looped, it plays the next tune in song
int playCurrentAndSetNextTune(Song* song) {

	//If the current song is not finished, play currenttune and set next tne
	if(song->current != NULL){
		play_tune(song->current);
	}
	else{
		//If it is finished, do nothing and restart
		song->current = song->first;
		return 0;
	}

	//If next is not last tune, set next
	if(song->current->next != NULL){
		song->current = song->current->next;
	}
	else{
#if USE_FREE_RUN
		song->current = NULL;
#if USE_LET
	///LOW ENERGY STOP
#else
	//NORMAL STOP
#endif
#else
		//If this was the last tune, set current to NULL
		song->current = NULL;
#endif
	}
	return 0;
}


//TODO::NO ERROR CHECKING HERE YET!!!
int construct_song(char* notes, int* timers, Song* song, int length, int octave) {

	song->first = (Tune*)malloc(sizeof(Tune));
	song->first->note = notes[0];
	song->first->octave = octave;
	song->first->time_to_play_ms = timers[0];
	song->first->next = NULL;

	song->current = song->first;

	for(int i = 1; i < length; i++) {
		song->current->next = (Tune*)malloc(sizeof(Tune));
		song->current->next->note = notes[i];
		song->current->next->octave = octave;
		song->current->next->time_to_play_ms = timers[i];
		song->current->next->next = NULL;

		song->current = song->current->next;
	}

	song->current = song->first;

	return 0;
}