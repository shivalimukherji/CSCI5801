#include "include/PoVoteSystem.h"
#include "include/TieBreaker.h"
#include "include/PopularityCase.h"

#include <vector>
#include <iostream>
#include <cstring>
#include <string>
#include <numeric>
#include <algorithm>

//All access of information comes from ballot
//PoProcessing* system is only there for redistribution
PoVoteSystem::PoVoteSystem(PoBallot* ballot) {
    processedBallot = ballot;
};


bool PoVoteSystem::startElection() {
        specialCase.push_back(new TieBreaker());
    	conductElection();
        return 1;
}

bool PoVoteSystem::conductElection() {
    candidates = processedBallot->getCandidates();
    std::vector<double> mapPercentage = processedBallot->getMapPercentage();
    std::vector<Candidate*> result = {}; 
	double high_1 = 0.0;
    int winIndex = -1;
    for(int i = 0; i < (int)mapPercentage.size(); i++) {
            if (mapPercentage[i] > high_1){
                high_1 = mapPercentage[i];
                winIndex = i;
            }

    }
    winner = *candidates[winIndex];

    for(int i = 0; i < (int)mapPercentage.size(); i++) {
            if (mapPercentage[i] == high_1 && winIndex != i){
                result.push_back(candidates[winIndex]);
                result.push_back(candidates[i]);
            }
    }
    if(result.size() > 1) {
        cout << "There is a tie among candidates with the lowest ballot." << endl;
        if(auditing) {
            audit->write("----RUNNING TIEBREAKER-----");
        }
        TieBreaker* t = static_cast<TieBreaker*>(specialCase[0]);
        winIndex = t->run(result.size());
        winner = *result[winIndex];
    }

    return true;
};