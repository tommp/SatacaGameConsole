#include "tunes.h"
#include "timer.h"
#include "dac.h"

static void _play_single_note(volatile Note* note){
    int note_frequency = 0;
	switch(note->note){
		case 'a':
			note_frequency = A_FREQUENCY*(pow(2,note->octave));
			break;
		case 'b':
			note_frequency = B_FREQUENCY*(pow(2,note->octave));
			break;
		case 'c':
			note_frequency = C_FREQUENCY*(pow(2,note->octave));
			break;
		case 'd':
			note_frequency = D_FREQUENCY*(pow(2,note->octave));
			break;
		case 'e':
			note_frequency = E_FREQUENCY*(pow(2,note->octave));
			break;
		case 'f':
			note_frequency = F_FREQUENCY*(pow(2,note->octave));
			break;
		case 'g':
			note_frequency = G_FREQUENCY*(pow(2,note->octave));
			break;
		default:
			break;
	}

	if(note_frequency < 20) {
		note_frequency = 20;
	} else if(note_frequency > 20000){
		note_frequency = 20000;
	}

	int top_value = TIMER_FREQUENCY/note_frequency;
	*LETIMER0_COMP0 = top_value;

	int num_overflows_to_run = ((TIMER_FREQUENCY/1000)*(note->time_to_play_ms))/top_value;

	if(num_overflows_to_run <1){
		num_overflows_to_run = 1;
	} else if(num_overflows_to_run >255){
		num_overflows_to_run = 255;
	}

	*LETIMER0_REP0 = num_overflows_to_run;

	if(*LETIMER0_STATUS != 1){
		*LETIMER0_CMD |= LETIMER0_CMD_START;
	}
}


//This should be looped, it plays the next note in song
void tunes_play_next_note(volatile Note** note, volatile uint32_t length, volatile uint32_t* current) {
    
    if(*current == 0){
        dac_enable();
    }

	//If the current song is not finished, play currentnote and set next tne
	if(*current < length){
		_play_single_note(note[*current]);
        *current = *current+1;
	}
	else{
		//If it is finished, do nothing and restart
		*current = 0;
        dac_disable();
        timer_LE_stop();
	}
}
