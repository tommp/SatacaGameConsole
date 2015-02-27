#ifndef SONGS_H
#define SONGS_H


//TUNES

volatile Tune g500ms = { .note = 'g', .octave = 4, .time_to_play_ms = 500 };
volatile Tune e500ms = { .note = 'e', .octave = 4, .time_to_play_ms = 300 };
volatile Tune b200ms = { .note = 'b', .octave = 4, .time_to_play_ms = 200 };
volatile Tune g1000ms = { .note = 'b', .octave = 7, .time_to_play_ms = 1000 };
//SONGS

//Imperial march
volatile Tune* imperial_march[9] = {&g500ms, &g500ms, &g500ms, &e500ms, &b200ms, &g500ms, &e500ms, &b200ms, &g500ms};
volatile uint32_t imperial_march_length = 9;
volatile uint32_t imperial_march_current_tune = 0;

//SOUNDS

//Shoot::TODO
volatile Tune* shoot[1] = {&g1000ms};
volatile uint32_t shoot_length = 1;
volatile uint32_t shoot_current_tune = 0;
//OPTIONS

volatile uint32_t use_fadeout = 0;
volatile uint32_t fade_direction = 1;
volatile uint32_t fade_inc = 10;
volatile uint32_t soundMAX = 80;
volatile uint32_t sound0 = 80;
volatile uint32_t sound1 = 0;


#endif
