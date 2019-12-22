#include <time.h>

clock_t start;
float target_delay = 1. / 30.;

void fps_set(float fps) {
    target_delay = 1. / fps;
}

void fps_start_game_loop() {
    start = clock();
}

void fps_end_game_loop() {
    while (((float)(clock()-start)/CLOCKS_PER_SEC) < target_delay);
}