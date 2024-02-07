#include "include/PoBallot.h"

PoBallot::PoBallot(std::vector<Candidate*> can, std::vector<int> ballot, std::vector<double> percent){
    candidates = can;
    mapBallot = ballot;
    mapPercentage = percent;
    numCandidates = candidates.size();
}

PoBallot::PoBallot() {

};