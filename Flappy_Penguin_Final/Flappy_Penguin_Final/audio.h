#ifndef AUDIO_H
#define AUDIO_H
#include <stdint.h>

void audio_init(void);
void audio_play(uint8_t restart);   // 1 = from start, 0 = resume
void audio_stop(void);              // hard-silence + stop ticking
void audio_set_loop(uint8_t loop_on);

#endif
