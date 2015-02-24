#include "tunes.h"

int play_tune(Tune* tune){
    int tune_frequency = 0;
	switch(tune->note){
		case 'a':
			tune_frequency = A_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'b':
			tune_frequency = B_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'c':
			tune_frequency = C_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'd':
			tune_frequency = D_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'e':
			tune_frequency = E_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'f':
			tune_frequency = F_FREQUENCY*(pow(2,tune->octave));
			break;
		case 'g':
			tune_frequency = G_FREQUENCY*(pow(2,tune->octave));
			break;
		default:
			return -1;
	}

	if(tune_frequency < 20) {
		tune_frequency = 20;
	} else if(tune_frequency > 20000){
		tune_frequency = 20000;
	}

	int top_value = TIMER_FREQUENCY/tune_frequency;
	*LETIMER0_COMP0 = top_value;

#if USE_FREE_RUN
	if(!(*LETIMER0_STATUS)){
		*LETIMER0_CMD |= LETIMER0_CMD_START;
	}
	return 0;
#else
	int num_overflows_to_run = ((TIMER_FREQUENCY/1000)*(tune->time_to_play_ms))/top_value;

	if(num_overflows_to_run <1){
		num_overflows_to_run = 1;
	} else if(num_overflows_to_run >255){
		num_overflows_to_run = 255;
	}

	*LETIMER0_REP0 = num_overflows_to_run;

	if(!(*LETIMER0_STATUS)){
		*LETIMER0_CMD |= LETIMER0_CMD_START;
	}
	return 0;
#endif
	
}


//TThis should be looped, it plays the next tune in song
int playCurrentAndSetNextTune(Tune* tune, int length, int* current) {

	//If the current song is not finished, play currenttune and set next tne
	if(current < length){
		play_tune(tune[current]);
	}
	else{
		//If it is finished, do nothing and restart
		current = 0;
		return 0;
	}

	//If next is not last tune, set next
	if(current < length-1){
		current++;
	}
	else{
#if USE_FREE_RUN
		current = length;
	///LOW ENERGY STOP 						:::::::::::::TODO
		if(*LETIMER0_STATUS){
			*LETIMER0_CMD |= LETIMER0_CMD_STOP
		}
#else
		//If this was the last tune, set current to NULL
		current = length;
#endif
	}
	return 0;
}