#include "include/Party.h"

Party::Party(vector<Candidate> cans, int seat, string n) {
    candidates = cans;
    maxSeat = seat;
    name = n;
}

Party::Party() {
    
}
