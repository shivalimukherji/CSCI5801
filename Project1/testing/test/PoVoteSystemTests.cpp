#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>
#include <typeinfo>
#include <iostream>

#include "../../src/include/PoProcessing.h"
#include "../../src/include/PoVoteSystem.h"
#include "../../src/include/PoBallot.h"
#include "../../src/include/Ballot.h"
#include "../../src/include/Candidate.h"

class PoVoteSystemTests : public ::testing::Test {
    protected:
	string testfile = "data/PO.csv";
	const char * path = testfile.c_str();
    FILE *file = fopen (path, "r");

	PoProcessing* po_test;
	PoBallot* po_ballot;

	PoVoteSystem* po_vote;
	
	void SetUp() override {
	    char buffer[100];
	    fgets(buffer, 100, file);
	    po_test = new PoProcessing(file);	

	    po_test->setUp();
        //ir_test->read();
        po_ballot = po_test->output();
	    po_vote = new PoVoteSystem(po_ballot);
	}

	void TearDown() override {
	}

};

TEST_F(PoVoteSystemTests, startElectionTest) {
    bool start = po_vote->startElection();
    EXPECT_TRUE(start);
};


TEST_F(PoVoteSystemTests, getWinnerTest) {
    bool start = po_vote->startElection();

    Candidate Winner = po_vote->getWinner();
    EXPECT_EQ("Smith I", Winner.getName());
};


TEST_F(PoVoteSystemTests, simpleGetSetWinner) {
	Candidate winner = Candidate("Johnson I");
	po_vote->setWinner(winner);

	EXPECT_EQ(po_vote->getWinner().getName(), "Johnson I");
};


TEST_F(PoVoteSystemTests, getSetProcessedBallot) {
	Candidate* can1 = new Candidate("John");
	Candidate* can2 = new Candidate("Doe");
	Candidate* can3 = new Candidate("Sally");

	std::vector<Candidate*> candidates = {can1, can2, can3};
	std::vector<int> sample_map = {1,2,3};
	std::vector<double> sample_map_percntage = {0.17, 0.33, 0.5};

	PoBallot* test = new PoBallot(candidates, sample_map, sample_map_percntage);

	po_vote->setProcessedBallot(test);

	EXPECT_EQ(po_vote->getProcessedBallot(), test);

};
