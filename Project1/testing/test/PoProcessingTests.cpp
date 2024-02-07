#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>
#include <typeinfo>
#include <iostream>

#include "../../src/include/PoProcessing.h"
#include "../../src/include/PoBallot.h"
#include "../../src/include/Ballot.h"
#include "../../src/include/Candidate.h"

class PoProcessingTests : public ::testing::Test {
    protected:
        string testfile = "data/PO.csv";
        const char * path = testfile.c_str();
        FILE *file = fopen (path, "r");
        PoProcessing* po_test;
        PoBallot* po_ballot;
	
	void SetUp() override {
        char buffer[100];
	    fgets(buffer, 100, file);
        
	    po_test = new PoProcessing(file);
	}

    void TearDown() override {
        //fclose(file);
    }

};

TEST_F(PoProcessingTests, runSetup) {
    
    if(!(po_test->readHeader(0))) {
        FAIL();
    }

    SUCCEED();
};


//TEST_F(PoProcessingTests, readFunction) {
//
//    if(!(po_test->setUp())) {
//        FAIL();
//    }
//
//    
//    if(!(po_test->readHeader(0))) {
//       FAIL();
//    }
//
//    if(!(po_test->read())) {
//       FAIL();
//    }
//
//   SUCCEED();
//};

TEST_F(PoProcessingTests, outputBallot) {
    PoBallot* ballot = po_test->output();
    PoBallot* POBALLOT;
    EXPECT_TRUE(typeid(*ballot) == typeid(*POBALLOT));
};

TEST_F(PoProcessingTests, getSetCandidate) {
    Candidate* c1 = new Candidate("[Sally, D]");
    Candidate* c2 = new Candidate("[Jane, R]");
    std::vector<Candidate*> candidates{c1, c2};

    po_test->setCandidates(candidates);
    EXPECT_EQ(po_test->getCandidates(), candidates);
};

TEST_F(PoProcessingTests, getSetMapPercentage) {
    std::vector<double> percentages{0.25, 0.75, 0.4, 0.3};
    po_test->setMapPercentage(percentages);
    EXPECT_EQ(po_test->getMapPercentage(), percentages);
};


TEST_F(PoProcessingTests, readCorrectly) {
    if(!(po_test->readHeader(0))) {
       FAIL();
    }

    if(!(po_test->read())) {
       FAIL();
    }

    int expectedBLines = 9;
    int expectedNumCanid = 6;

    Candidate E1 = Candidate("Pike D", "D");
    Candidate E2 = Candidate("Foster D", "D");
    Candidate E3 = Candidate("Deutsch R", "R");
    Candidate E4 = Candidate("Borg R", "R");
    Candidate E5 = Candidate("Jone R", "R");
    Candidate E6 = Candidate("Smith I", "I");

    vector<Candidate*> expectedCanid = {&E1, &E2, &E3, &E4, &E5, &E6};

    EXPECT_EQ(expectedBLines, po_test->getBLinesToRead());
    EXPECT_EQ(expectedNumCanid, po_test->getNumCandidates());

    for(int i = 0; i < (int)expectedCanid.size(); i++) {
        Candidate* expectCanid = expectedCanid[i];
        Candidate* systemCanid = po_test->getCandidates()[i];

	ASSERT_EQ(expectCanid->getName(), systemCanid->getName());
 
    };
};


TEST_F(PoProcessingTests, calculate) {
    if(!(po_test->readHeader(0))) {
       FAIL();
    }

    if(!(po_test->read())) {
       FAIL();
    }
    int expectedBLines = 9;
    int expectedNumCanid = 6;

    std::vector<double> percentageMapping;
    std::vector<int> ballotMapping{3,2,0,2,1,1};

    for(int i = 0; i < expectedNumCanid; i++) {
        double percentage = (ballotMapping[i]/1.0)/expectedBLines;
        percentageMapping.push_back(percentage);
    }

    std::vector<double> systemPercentage = po_test->getMapPercentage();
    for(int i = 0; i < (int)percentageMapping.size(); i++) {
        ASSERT_EQ(percentageMapping[i], systemPercentage[i]);
    };  
};





