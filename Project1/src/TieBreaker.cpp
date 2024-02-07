#include "include/TieBreaker.h"

int TieBreaker::run() {
    //Seed it and return 0 or 1
    srand(time(0));
    return rand() % 2;
}

int TieBreaker::run(int size_) {
    //Seed it and return 0-size
    srand(time(0));
    return rand() % (size_);
}

/*
void TieBreaker::run1000() {
    return;
}
*/