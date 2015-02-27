#ifndef SONGS_H
#define SONGS_H

enum songs{
	


}


//TUNES

volatile Tune g500ms = { .note = 'g', .octave = 4, .time_to_play_ms = 500 };
volatile Tune e500ms = { .note = 'e', .octave = 4, .time_to_play_ms = 300 };
volatile Tune b200ms = { .note = 'b', .octave = 4, .time_to_play_ms = 200 };
volatile Tune g1000ms = { .note = 'b', .octave = 5, .time_to_play_ms = 1000 };

volatile Tune g400ms = { .note = 'g', .octave = 4, .time_to_play_ms = 400 };
volatile Tune e400ms = { .note = 'e', .octave = 4, .time_to_play_ms = 400 };
volatile Tune c800ms = { .note = 'a', .octave = 3, .time_to_play_ms = 800 };

//SONGS

//Imperial march
volatile Tune* imperial_march[9] = {&g500ms, &g500ms, &g500ms, &e500ms, &b200ms, &g500ms, &e500ms, &b200ms, &g500ms};
volatile uint32_t imperial_march_length = 9;
volatile uint32_t imperial_march_current_tone = 0;
volatile uint8_t use_imperial_march_fadeout = 0;
//SOUNDS

//Shoot
volatile Tune* shoot[1] = {&g1000ms};
volatile uint32_t shoot_length = 1;
volatile uint32_t shoot_current_tone = 0;
volatile uint8_t use_shoot_fadeout = 0;

//Wallhit
volatile Tune* wallhit[3] = {&g400ms, &e400ms, &c800ms};
volatile uint32_t wallhit_length = 3;
volatile uint32_t wallhit_current_tone = 0;
volatile uint8_t use_wallhit_fadeout = 1;

//OPTIONS TODO::SET TO UINT8 WHERE POSSIBLE
volatile uint8_t song_to_use = 1;
volatile uint8_t use_fadeout = 0;
volatile uint32_t fade_direction = 1;
volatile uint32_t fade_inc = 10;
volatile uint32_t soundMAX = 200;
volatile uint32_t sound0 = 80;
volatile uint32_t sound1 = 0;

void set_fadeout(uint8_t condition){
    if(condition){
        if(sound0 <= 0){
            fade_direction = 1;
        }
        else if(sound0 >= soundMAX){
            fade_direction = 0;
        }
        if(fade_direction == 1) {
            sound0+=fade_inc;
        }
        else{
            sound0-=fade_inc;
        }
    }
}

#endif
