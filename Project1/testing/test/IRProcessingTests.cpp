#include <gtest/gtest.h>
#include <stdio.h>
#include <string.h>
#include <typeinfo>
#include <iostream>

#include "../../src/include/IRProcessing.h"
#include "../../src/include/IRBallot.h"
#include "../../src/include/Ballot.h"

class IRProcessingTests : public ::testing::Test {
    protected:
        string testfile = "data/IR.csv";
        const char * path = testfile.c_str();
        
        std::vector<string> files = {"data/IR.csv"};

        FILE *file = fopen (path, "r");
        IRProcessing* ir_test;
        IRBallot* ir_ballot;
	
	void SetUp() override {
        char buffer[100];
	    fgets(buffer, 100, file);
        
	    ir_test = new IRProcessing(file, files);
	}

    void TearDown() override {
        //fclose(file);
    }

};

TEST_F(IRProcessingTests, runSetup) {
    
    if(!(ir_test->readHeader(0))) {
        FAIL();
    }

    SUCCEED();
};


TEST_F(IRProcessingTests, readFunction) {

//    if(!(ir_test->setUp())) {
//        FAIL();
//    }

    
    if(!(ir_test->readHeader(0))) {
       FAIL();
    }

    if(!(ir_test->read())) {
       FAIL();
    }

   SUCCEED();
};

TEST_F(IRProcessingTests, readCorrectly) {
    if(!(ir_test->readHeader(0))) {
       FAIL();
    }

    if(!(ir_test->read())) {
       FAIL();
    }

    int expectedBLines = 6;
    int expectedNumCanid = 4;

    IRCandidate E1 = IRCandidate("Rosen (D)");
    E1.setNumBallots(3);

    IRCandidate E2 = IRCandidate("Kleinberg (R)");

    IRCandidate E3 = IRCandidate("Chou (I)");
    E3.setNumBallots(2);

    IRCandidate E4 = IRCandidate("Royce (L)");
    E4.setNumBallots(1);

    vector<IRCandidate*> expectedCanid = {&E1, &E2, &E3, &E4};

    EXPECT_EQ(expectedBLines, ir_test->getBLinesToRead());
    EXPECT_EQ(expectedNumCanid, ir_test->getNumCandidates());

    for(int i = 0; i < (int)expectedCanid.size(); i++) {
        IRCandidate* expectCanid = expectedCanid[i];
        IRCandidate* systemCanid = ir_test->getCandidates()[i];

        //Check for name
        ASSERT_EQ(expectCanid->getName(), systemCanid->getName());

        //Check for # of ballots
        ASSERT_EQ(expectCanid->getNumBallots(), systemCanid->getNumBallots());

    };
};

TEST_F(IRProcessingTests, outputBallot) {
    IRBallot* ballot = ir_test->output();
    IRBallot* IRBALLOT;
    EXPECT_TRUE(typeid(*ballot) == typeid(*IRBALLOT));
};

TEST_F(IRProcessingTests, calculate) {
    if(!(ir_test->readHeader(0))) {
       FAIL();
    }

    if(!(ir_test->read())) {
       FAIL();
    }

    int expectedBLines = 6;
    int expectedNumCanid = 4;

    IRCandidate E1 = IRCandidate("Rosen (D)");
    E1.setNumBallots(3);

    IRCandidate E2 = IRCandidate("Kleinberg (R)");

    IRCandidate E3 = IRCandidate("Chou (I)");
    E3.setNumBallots(2);

    IRCandidate E4 = IRCandidate("Royce (L)");
    E4.setNumBallots(1);

    vector<IRCandidate*> expectedCanid = {&E1, &E2, &E3, &E4};

    std::vector<double> percentageMapping;

    for(int i = 0; i < (int)expectedCanid.size(); i++) {
        double percentage = (expectedCanid[i]->getNumBallots()/1.0)/expectedBLines;
        percentageMapping.push_back(percentage);
    }

    std::vector<double> systemPercentage = ir_test->getMapPercentage();
    for(int i = 0; i < (int)percentageMapping.size(); i++) {
        ASSERT_EQ(percentageMapping[i], systemPercentage[i]);
    };  
};

TEST_F(IRProcessingTests, simpleGetSetNumCandidates) {
    ir_test->setNumCandidates(5);
    EXPECT_EQ(5, ir_test->getNumCandidates());
};

TEST_F(IRProcessingTests, getSetCandidate) {
    IRCandidate* c1 = new IRCandidate("Sally");
    IRCandidate* c2 = new IRCandidate("Jane");
    std::vector<IRCandidate*> candidates{c1, c2};

    ir_test->setCandidates(candidates);
    EXPECT_EQ(ir_test->getCandidates(), candidates);
};

TEST_F(IRProcessingTests, getSetMapPercentage) {
    std::vector<double> percentages{0.25, 0.75, 0.4, 0.3};
    ir_test->setMapPercentage(percentages);
    EXPECT_EQ(ir_test->getMapPercentage(), percentages);
};

TEST_F(IRProcessingTests, setGetFiles) {
    vector<string> test_files = {"data/IR.csv", "data/IR2.csv"};
    ir_test->setFiles(test_files);

    EXPECT_EQ(ir_test->getFiles(), test_files);
};

TEST_F(IRProcessingTests, singularFile) {
    vector<string> test_files = {"data/IR.csv"};
    ir_test->setFiles(test_files);

    if(!(ir_test->setUp())) {
       FAIL();
    };
};

TEST_F(IRProcessingTests, runMultipleFiles) {
    vector<string> test_files = {"data/IR.csv", "data/IR2.csv"};
    ir_test->setFiles(test_files);

    if(!(ir_test->setUp())) {
       FAIL();
    };
};

TEST_F(IRProcessingTests, runMultipleFiles3) {
    vector<string> test_files = {"data/IR.csv", "data/IR2.csv","data/IR3.csv"};
    ir_test->setFiles(test_files);

    if(!(ir_test->setUp())) {
       FAIL();
    };
};

TEST_F(IRProcessingTests, m3CheckBallot) {
    vector<string> test_files = {"data/IR.csv", "data/IR2.csv","data/IR3.csv"};
    ir_test->setFiles(test_files);

    if(!(ir_test->setUp())) {
       FAIL();
    };

    
    int num_ballots = ir_test->getTotalBallots();
    EXPECT_EQ(num_ballots, 24);
};

TEST_F(IRProcessingTests, mFilesCheckBallot) {
    vector<string> test_files = {"data/IR.csv", "data/IR2.csv"};
    ir_test->setFiles(test_files);

    if(!(ir_test->setUp())) {
       FAIL();
    };

    int num_ballots = ir_test->getTotalBallots();
    EXPECT_EQ(num_ballots, 17);
};

TEST_F(IRProcessingTests, invalidFilesInMultiple) {
    vector<string> test_files = {"data/IR.csv", "IR2.csv"};
    ir_test->setFiles(test_files);

    if(!(ir_test->setUp())) {
       FAIL();
    };
};

TEST_F(IRProcessingTests, getInvalidFiles) {
    vector<string> test_files = {"data/IR.csv", "IR2.csv"};
    ir_test->setFiles(test_files);

    if(!(ir_test->setUp())) {
       FAIL();
    };

    vector<string> invalid_files = ir_test->getInvalidFiles();
    vector<string> expected_files = {"IR2.csv"};

    EXPECT_EQ(invalid_files, expected_files);
};
