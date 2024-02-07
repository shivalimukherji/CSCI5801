#include <gtest/gtest.h>
#include "../../src/include/TableBuilder.h"

class TableBuilderTests : public ::testing::Test {
    protected:
        TableBuilder T = TableBuilder();

        void SetUp() override {
        }

        //void TearDown() override {}
};

TEST_F(TableBuilderTests, buildRow) {
    T.buildRow();
};

TEST_F(TableBuilderTests, getSetRows) {
    T.setNumRows(5);

    EXPECT_EQ(T.getNumRows(), 5);
};

TEST_F(TableBuilderTests, buildRowCheck) {
    T.buildRow();
    T.buildRow();

    EXPECT_EQ(T.getNumRows(), 2);
};

TEST_F(TableBuilderTests, tableAddError) {
    T.addCell(1, "This should not be here");
    T.addCell(2, "This should also not be here");
};

TEST_F(TableBuilderTests, tableAddError1) {
    T.addCell(0, "This should not be here");
    T.addCell(-1, "This should not be here");
    T.addCell(-2, "This should also not be here");
};


TEST_F(TableBuilderTests, tableSetError1) {
    T.setCell(4, 1, "N/A");
    T.setCell(0, 1, "N/A");
    T.setCell(0, 2, "N/A");
};

TEST_F(TableBuilderTests, tableSetError2) {
    T.buildRow();
    T.setCell(0, 1, "N/A");
    
    T.addCell(0, "N/A");
    T.setCell(0, 2, "N/A");
};

TEST_F(TableBuilderTests, tableSetError3) {
    T.setCell(-1, 2, "N/A");
    T.setCell(0, -1, "N/A");
};

TEST_F(TableBuilderTests, tableGetError1) {
    T.getCell(2, 4);
    T.getCell(0, 1);
};

TEST_F(TableBuilderTests, tableGetError2) {
    T.getCell(-1, 1);
    T.getCell(-2, -1);
};

TEST_F(TableBuilderTests, table1) {
    for(int i = 0; i < 3; i++) {
        T.buildRow();
    }

    T.addCell(0, "Candidate");
    T.addCell(0, "Ballots");

    T.addCell(1, "Ross (D)");
    T.addCell(1, "10");

    T.addCell(2, "Susan (R)");
    T.addCell(2, "15");

    T.build();
};

TEST_F(TableBuilderTests, table2) {
    for(int i = 0; i < 4; i++) {
        T.buildRow();
    }

    T.addCell(0, "Candidate");
    T.addCell(0, "Ballots");

    T.addCell(1, "Ross (D)");
    T.addCell(1, "10");

    T.addCell(2, "Susan (R)");
    T.addCell(2, "15");

    T.addCell(3, "Ralph (I)");

    T.build();
};


TEST_F(TableBuilderTests, table3) {
    for(int i = 0; i < 4; i++) {
        T.buildRow();
    }

    T.addCell(0, "Candidate");
    T.addCell(0, "Ballots");

    T.addCell(1, "Ross (D)");
    T.addCell(1, "10");

    T.addCell(2, "Susan (R)");
    T.addCell(2, "15");

    T.addCell(3, "Ralph (I)");

    T.setCell(1, 1, "20");
    T.setCell(2, 0, "Alpha (R)");

    T.build();
};


TEST_F(TableBuilderTests, getCell) {
    T.buildRow();

    T.addCell(0, "Candidate");
    T.addCell(0, "Ballots");

    std::string input = T.getCell(0, 0);

    EXPECT_EQ("Candidate", input);

    input = T.getCell(0,1);
    EXPECT_EQ("Ballots", input);
    
};


TEST_F(TableBuilderTests, getCellToInt) {
    T.buildRow();

    T.addCell(0, "Candidate");
    T.addCell(0, "10");
    
    int data = stoi(T.getCell(0,1));
    EXPECT_EQ(10, data);
    
};