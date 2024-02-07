#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>
#include <typeinfo>
#include <iostream>
#include "../../src/include/CPLProcessing.h"
#include "../../src/include/CPLVoteBallot.h"
#include "../../src/include/CPLVoteSystem.h"
#include "../../src/include/Party.h"
//#include "../../src/include/Ballot.h"

class CPLVotingSystemTests : public ::testing::Test {
    protected:
	string testfile = "data/CPL.csv";
	vector<string> input_files;
	const char * path = testfile.c_str();
        FILE *file = fopen (path, "r");
	CPLProcessing* test_cplprocessing;
	CPLVoteBallot* test_cplBallot;
	CPLVoteSystem* test_cplVolteSystem;
	AuditFile audit = AuditFile("audit.txt");
	
	void SetUp() override {
	    input_files.push_back("data/CPL.csv");
	    char buffer[100];
	    fgets(buffer, 100, file);
	    test_cplprocessing = new CPLProcessing(file);	    
	    audit.labelFile("CPLAudit");
            audit.open();
            test_cplprocessing->setAudit(&audit);
	    test_cplprocessing->setUp();
            test_cplBallot = test_cplprocessing->output();
	    test_cplVolteSystem = new CPLVoteSystem(test_cplBallot);
	    test_cplVolteSystem->setAuditFile(&audit);
	}

	void TearDown() override {
	    audit.close();
	}

};

TEST_F(CPLVotingSystemTests, startElectionTest) {
    bool start = test_cplVolteSystem->startElection();
    EXPECT_TRUE(start);
}

TEST_F(CPLVotingSystemTests, conductElectionTest) {
    bool conduct = test_cplVolteSystem->conductElection();
    EXPECT_TRUE(conduct);

}

TEST_F(CPLVotingSystemTests, getWinnerTest) {
    bool conduct = test_cplVolteSystem->conductElection();
    std::vector<Candidate> seatWinners = test_cplVolteSystem->getWinner();
    EXPECT_EQ("Foster",seatWinners[0].getName());
}

TEST_F(CPLVotingSystemTests, remainSeatTest) {
    bool conduct = test_cplVolteSystem->conductElection();
    int remainSeat = test_cplVolteSystem->getRemainSeat();
    EXPECT_EQ(0,remainSeat);
}

TEST_F(CPLVotingSystemTests, CPLLotteryTest) {
    int winner = test_cplVolteSystem->CPLLottery(6);
    if (winner >= 6) {
        FAIL();
    }
    SUCCEED();
}

TEST_F(CPLVotingSystemTests, getWinnerWithMultipleFiles) {
    vector<string> test_files = {"data/CPL.csv", "data/CPL2.csv"};
    test_cplprocessing = new CPLProcessing(file, test_files);	    
    test_cplBallot = test_cplprocessing->output();
    bool conduct = test_cplVolteSystem->conductElection();
    std::vector<Candidate> seatWinners = test_cplVolteSystem->getWinner();
    EXPECT_EQ("Foster",seatWinners[0].getName());
}
