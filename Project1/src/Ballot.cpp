#include "include/Ballot.h"

using namespace std;

Ballot::Ballot(int ranking, vector<int> mapping_) {
    mapping = mapping_;
    rank = ranking;
};


Ballot::Ballot() {}

int Ballot::getIndex() {
    auto index = find(mapping.begin(), mapping.end(), rank) - mapping.begin();
    if(index >= (int)mapping.size()) {
        return -1;
    }
    return index;
}

void Ballot::increaseRank() {
    if(rank > (int)mapping.size() + 1) {
        return;
    }

    rank++;
};


