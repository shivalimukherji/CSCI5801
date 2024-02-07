/** \file CPLVoteSystem.cpp
 *  \brief Conduct election according to CPL rules and show the winners.
 *
 *  \author Wenjing Jiang
 *  \bug No known bugs.
 */




#include <iostream>
#include <string>
#include <numeric>

#include <algorithm>


#include "include/CPLVoteSystem.h"

CPLVoteSystem::CPLVoteSystem(CPLVoteBallot* incomingBallot_) {
    parties = incomingBallot_->getParties();
    mapAllocatedSeat = incomingBallot_->getMapAllocatedSeat();
    mapRemainBallot = incomingBallot_->getMapBallot();
    seats = incomingBallot_->getSeats();
    numParties = incomingBallot_->getNumParties();
    quota = incomingBallot_->getQuota();
    status = 0;
    for (int i=0; i < numParties; i++) {
    	mapRemainSeat.push_back(parties[i]->getMaxSeat());
    } 
    // std::cout << "Passing to CPLVoteSystem:"<< endl;
    // std::cout << quota << endl;
    // for (int i=0; i < 6; i++) {
    // 	std::cout << mapRemainBallot[i]<< endl;
    // }
};

bool CPLVoteSystem::startElection() {
	audit->write("-----Ballot Distribution-----");
        for (int i=0; i < numParties; i++) {
	    audit->write(parties[i]->getName()+": "+to_string(mapRemainBallot[i]));
	}
	conductElection();
    return 1;
}

bool CPLVoteSystem::conductElection() {
    for (int i=0; i < numParties; i++) {
	mapAllocatedSeat[i] += mapRemainBallot[i]/quota;
	mapRemainBallot[i] = mapRemainBallot[i]%quota;
	mapRemainSeat[i] -= mapAllocatedSeat[i];
	std::cout << parties[i]->getName() << ": " << mapAllocatedSeat[i] << endl;
    }
    remainSeat = seats - std::accumulate(mapAllocatedSeat.begin(), mapAllocatedSeat.end(), 0);
    std::cout << "Election start:"<< endl;
    std::cout << "Allcated seats:"<< endl;
    audit->write("\n-----Seat Allocation-----");
    for (int i=0; i < numParties; i++) {
    	std::cout << parties[i]->getName() << ": " << mapAllocatedSeat[i]<< endl;
        audit->write(parties[i]->getName()+": "+to_string(mapAllocatedSeat[i]));	
    }
    std::cout << "Remaining Ballots:"<< endl;
    audit->write("\n-----Remaining Ballots and Available Seats-----");
    for (int i=0; i < numParties; i++) {
    	if (mapRemainSeat[i] > 0) {
    		std::cout << parties[i]->getName() << ": " << mapRemainBallot[i]<< endl;
		audit->write(parties[i]->getName()+": "+to_string(mapRemainBallot[i])+"\n");
    	}
    }
    std::cout << "Remaining Seats:" << remainSeat << endl;
    audit->write("Remaining seats: "+to_string(remainSeat));
    std::vector< std::pair<int, int> > sortedRemainBallot;
    for (int i=0; i < numParties; i++) {
	if (mapRemainSeat[i] > 0) {
    	    sortedRemainBallot.push_back(std::make_pair(mapRemainBallot[i], i));
	}
    }
    sort(sortedRemainBallot.rbegin(),sortedRemainBallot.rend());

    for (int i=0; i < remainSeat; i++) {
	    audit->write("-----Draw Lottery-----");
	    std::cout << "Sorted remain ballots:" << endl;
	    for (int i=0; i < numParties; i++) {
	    	std::cout << sortedRemainBallot[i].first << " " << sortedRemainBallot[i].second << " " << parties[sortedRemainBallot[i].second]->getName() << endl;
	    }
	    
    	
        int winner;
	int randNum = CPLLottery(sortedRemainBallot.size());
	winner = sortedRemainBallot[randNum].second;
	mapAllocatedSeat[winner] += 1;
	mapRemainSeat[winner] -= 1;
	sortedRemainBallot[randNum].first = 0;
	sort(sortedRemainBallot.rbegin(),sortedRemainBallot.rend());
	sortedRemainBallot.pop_back();
	
	//int candidate1 = sortedRemainBallot[0].second;
	//int winner;
    	//if (sortedRemainBallot[0].first == sortedRemainBallot[1].first) {
    	//        if (rand() > 0.5) {	
    	//	    winner = candidate1;
	//	}
	//	else {
	//	    winner = sortedRemainBallot[1].second;
	//	}
    	//	mapAllocatedSeat[winner] += 1;
    	//	mapRemainSeat[winner] -= 1;
     	//	sortedRemainBallot[0].first = 0;
    	//	sort(sortedRemainBallot.rbegin(),sortedRemainBallot.rend());   		
    	//}
    	//else {
    	//	winner = candidate1;
    	//	mapAllocatedSeat[winner] += 1;
    	//	mapRemainSeat[winner] -= 1;
    	//	sortedRemainBallot[0].first = 0;
    	//	sort(sortedRemainBallot.rbegin(),sortedRemainBallot.rend());
    	//}
	audit->write(parties[winner]->getName()+" gets a seat");
    }

    std::cout << "Election Winners :" << endl;
    audit->write("\n-----Election Winners-----");
    for (int i=0; i < numParties; i++) {
    	int seat = mapAllocatedSeat[i];
	string party_winners = "-- ";
    	std::cout << parties[i]->getName() << " gets seat: " << seat << endl;
	party_winners += parties[i]->getName() + ": ";
	    for (int j=0; j < seat; j++) {
	    	std::vector<Candidate> partyCandidates = parties[i]->getCandidate();
	    	string winner = partyCandidates[j].getName();
		seatWinners.push_back(partyCandidates[j]);
                std::cout << "Winner: " << winner << endl;
		party_winners += winner + " ";
	    }
	audit->write(party_winners);
    }
    remainSeat = seats - std::accumulate(mapAllocatedSeat.begin(), mapAllocatedSeat.end(), 0);
    audit->write("\n-----End-----");    
    return 1;

};

int CPLVoteSystem::CPLLottery(int parties_size) {
    std::srand(std::time(nullptr));
    int randNum = std::rand() % parties_size;
    return randNum;
};
