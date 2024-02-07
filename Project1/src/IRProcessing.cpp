#include "include/IRProcessing.h"
//#include "library/strsep.h"

#include <algorithm>
#include <cstring>

//Helper function to read the next line and return
//the content seperated by ,
char* IRProcessing::nextLine() {
    char *p;
    fgets(buffer, 100, file);
    char* duplicate = strdup(buffer);

    p = strtok(duplicate, ",");
    return p;
};

bool IRProcessing::setUp() {

    //Read the header information of the first file.
    //If sucessful, read the ballots and distribute them out.
    if(readHeader(0)) {
        read();
    } else {
        return false;
    }

    //If there's additional files, read them.
    //If they're invalid, add them to the invalid_files vector.
    for(int i = 1; i < (int)files.size(); i++) {
        file = fopen(files[i].c_str(), "r");

        if(readHeader(1)) {
            read();
        } else {
            invalid_files.push_back(files[i]);
            continue;
        }
    }

    for(int i = 0; i < (int)invalid_files.size(); i++) {
        cout << "Invalid files: " << invalid_files[i] << " ";
        if(i == (int)invalid_files.size()-1) {
            cout << endl;
        }
    };

    return true;
};

//Multiple indicates the file coming in is not the original file initalized.
bool IRProcessing::readHeader(int multiple) {
    char *p;
    int numCandidates_;
    vector<IRCandidate*> candidates_;
    vector<int> mapBallot_;

    //Error check
    if(file == NULL) {
        return false;
    }

    //Account for the first line (IR) header
    if(multiple) {
        //Get the next line of the CSV file (in this case the very first line of CSV)
        if ( fgets (buffer , 100 , file) == NULL ) {
            return false;
        }

        //If the first line of CSV file is not IR.
        strtok(buffer, ",");
        strtok(buffer, "\n");
        if(strcmp(buffer, "IR") != 0) {
            return false;
        }
    };

    //Get the second line (numCandidates)
    if((p = nextLine()) == NULL) {
        return false;
    };
    numCandidates_ = stoi(p);

    //Get the third line: candidates
    if((p = nextLine()) == NULL) {
        return false;
    };

    while(p != NULL) {
        string s = p;
        s.erase(remove(s.begin(),s.end(), '\n'), s.cend());

        IRCandidate *candidate = new IRCandidate(s);
        candidates_.push_back(candidate);
        p = strtok(NULL, ",");
    };

    //Set up the mapping for ballot of each candidate
    for(int i=0; i < (int)candidates_.size(); i++) {
        mapBallot_.push_back(0);
    };

    //Get the fourth line: number of ballots
    if((p = nextLine()) == NULL) {
        return false;
    };

    if(multiple) {
        //Compare the header information with the original.
        if(numCandidates != numCandidates_) {
            return false;
        }

        for(int i = 0; i < (int)candidates.size(); i++) {
            if(candidates[i]->getName() == candidates_[i]->getName()) {
                continue;
            }
            return false;
        }

    } else {
        numCandidates = numCandidates_;
        candidates = candidates_;
        mapBallot = mapBallot_;

        //Build the table
        tableSetup();
    }

    bLinesToRead = atoi(p);
    totalBallots += bLinesToRead;

    //Write to the audit file if the option is turned on
    //Contains information to the election

    if(auditing) {
        audit->write("IR");
        string numberOfCandidates = "Number of Candidates: " + to_string(numCandidates);
        audit->write(numberOfCandidates);

        audit->write("Candidates:");
        //Write each of the candidates into the audit
        for(int i=0; i < (int)candidates.size(); i++) {
            audit->write(candidates[i]->getName());
        };

        string numBallots = "Number of ballots: " + to_string(bLinesToRead);
        audit->write(numBallots);

        /*-----Ballot Process-----*/
        audit->write("-----BALLOT PROCESS----- \n");
    };

    return true;
};

bool IRProcessing::read() {
    char *p;

    for(int i = 0; i < bLinesToRead; i++) {
        //Initalize the ranking for a Ballot
        std::vector<int> ballotRanking(numCandidates, 0);

        //Read the next line and create a duplicate of the buffer
        //This is needed for the strsep function
        fgets(buffer, 100, file);
        char* duplicate = strdup(buffer);
        p = strsep(&duplicate, ",");

        //Get the ranking of the ballot in .csv file and send it the vector
        //containing the rank of ballot (ballotRanking)
        int counter = 0;
        while(counter < numCandidates) {
            ballotRanking[counter] = atoi(p);
            counter++;
            p = strsep(&duplicate, ",");
        };

        //Find index of 1st ranking and map it to mapBallot
        auto index = find(ballotRanking.begin(), ballotRanking.end(), 1) - ballotRanking.begin();
        mapBallot[index]++;

        //Create ballot with the ranking and give it to the Candidate
        Ballot *ballot = new Ballot(1, ballotRanking);
        candidates[index]->addBallot(ballot);
        //candidates[index]->increaseNum();

        //Write to the audit file if the option is turned on
        //Write the current ballot and who got the ballot
        //Display the overall ballot for each candidate during that time.
        if(auditing) {
            //Write the currrent map ballot to the audit file
            int j;

            /*\\\\Ballot i \\\\*/
            audit->write("|| Ballot " + to_string(i+1) + "||");
            //Write the ballot ranking to the audit file.
            for(j = 0; j < (int)ballotRanking.size(); j++) {
                audit->write(to_string(ballotRanking[j]) + " ", false);
            };
            audit->write("\n", false);
            audit->write("Ballot -> " + candidates[index]->getName() + "\n");

            /*\\\\Ballot Mapping \\\\*/
            audit->write("\\\\ Ballot Mapping \\\\");
            for(j = 0; j < (int)mapBallot.size(); j++) {
                audit->write(candidates[j]->getName() + " [" + to_string(mapBallot[j]) + "] ", false);
                //audit->write(to_string(mapBallot[j]) + " ", false);
            }
            audit->write("\n");
        }
    }

    //Afterwards functions:
    //Set up the table and calculate the percentages
    tableRound0();
    calculate();
    fclose(file);

    return true;
}

void IRProcessing::calculate() {
    std::vector<double> percentageMapping;

    for(int i = 0; i < (int)candidates.size(); i++) {
        double percentage = (mapBallot[i]/1.0)/totalBallots;
        percentageMapping.push_back(percentage);
    }

    mapPercentage = percentageMapping;

    //Write to the audit file if the option is turned on
    //Write the percentage of each candidate
    if(auditing) {
        audit->write("\\\\ Percentage Mapping \\\\");
        for(int i = 0; i < (int)mapBallot.size(); i++) {
            audit->write(candidates[i]->getName() + " [" + to_string(mapPercentage[i]) + "] ", false);
        }
        audit->write("\n");
    };

}

IRBallot* IRProcessing::output() {
    IRBallot* output = new IRBallot(candidates, mapBallot, mapPercentage);
    output->setTotalBallot(totalBallots);
    return output;
};

bool IRProcessing::redistribute(IRCandidate* candidate) {
    round++;
    //Get index of inputted candidate
    auto index = find(candidates.begin(), candidates.end(), candidate) - candidates.begin();

    //Initalize variables
    std::vector<Ballot*> candidateBallot = candidate->getBallotList();
    Ballot* ballot;
    int forCandidateIndex;

    //For each ballot in candidate's ballot list
    for(int i = 0; i <= (int)candidateBallot.size(); i++) {
        if(candidateBallot.size() == 0) {
            break;
        }

        //Get the last ballot in the list
        //ballot = candidate->popBallot();

        ballot = candidateBallot.back();
        candidateBallot.pop_back();

        //Increase the rank of the ballot and check the ranking for the candidate to
        //see if they're still in the race. If not, keep increasing till ranking has been
        //found for an available candidate.
        for(int i = 0; i < (int)candidates.size(); i++) {
            ballot->increaseRank();

            forCandidateIndex = ballot->getIndex();

            if(forCandidateIndex == -1) {
                break;
            }

            if(mapBallot[forCandidateIndex] != -1) {
                break;
            }
        }

        if(forCandidateIndex != -1) {
            candidates[forCandidateIndex]->addBallot(ballot);
            mapBallot[forCandidateIndex]++;
            mapBallot[index]--;
        } else {
            exhausted++;
        }

        //Account the ballot for the new candidate and mapping
        // candidates[forCandidateIndex]->addBallot(ballot);
        // mapBallot[forCandidateIndex]++;
        // mapBallot[index]--;

        //Write to the audit file if the option is turned on
        //Write the current ballot and who got the ballot
        //Display the overall ballot for each candidate during that time.
        if(auditing) {
            //Write the currrent map ballot to the audit file
            int j;
            /*-----Ballot Redistribution-----*/
            audit->write("-----BALLOT REDISTRIBUTION----- \n");

            /*Eliminated Candidate*/
            audit->write("Eliminated Candidate: " + candidate->getName() + " [" + to_string(candidate->getNumBallots())+"]");


            /*\\\\Ballot i \\\\*/
            audit->write("|| Ballot " + to_string(i+1) + "||");
            //Write the ballot ranking to the audit file.
            for(j = 0; j < (int)candidates.size(); j++) {
                audit->write(to_string(ballot->getMapping()[j]) + " ", false);
            };
            audit->write("\n", false);

            if(forCandidateIndex != -1) {
                audit->write("Ballot -> " + candidates[forCandidateIndex]->getName() + "\n");
            } else {
                audit->write("Ballot -> No One\n");
            }

            /*\\\\Ballot Mapping \\\\*/
            audit->write("\\\\ Ballot Mapping \\\\");
            for(j = 0; j < (int)mapBallot.size(); j++) {
                audit->write(candidates[j]->getName() + " [" + to_string(mapBallot[j]) + "] ", false);
                //audit->write(to_string(mapBallot[j]) + " ", false);
            }
            audit->write("\n");
        }
    }

    //Eliminate the candidate from the race with a -1
    mapBallot[index] = -1;
    candidates[index]->setNumBallots(-1);
    numCandidates--;

    //Recalculate and adjust the table accordingly.
    redistributeTable();
    calculate();

    return true;
};

/*
------------ FUNCTIONS RELATING TO TABLE ---------------------------
*/

//Setup initial table
void IRProcessing::tableSetup() {
    //Add a row for each candidate, 1 for header, 1 for exhausted pile, 1 for total ballot
    int rowToConstruct = candidates.size() + 3;

    for(int i = 0; i < rowToConstruct; i++) {
        table.buildRow();
    };

    //Setup table headers
    table.addCell(0, "Candidates");
    table.addCell(0, "Votes");
    table.addCell(0, "+-");

    //Setup candidate rows
    for(int i = 0; i < (int)candidates.size(); i++) {
        table.addCell(i+1, candidates[i]->getName());
        table.addCell(i+1, "0");
        table.addCell(i+1, "0");
    }

    //Setup exhausted pile and total ballots.
    table.addCell(rowToConstruct-2, "Exhausted");
    table.addCell(rowToConstruct-2, to_string(exhausted));
    table.addCell(rowToConstruct-1, "Totals");
    table.addCell(rowToConstruct-1, "0");
};

void IRProcessing::tableRound0() {
    //Populate each candidate's votes column in table
    //i+1 for candidate because of the following:
    //[0] [HEADER]
    //[1] [Candidate 1]
    //[2] [Candidate 2]
    //...
    for(int i = 0; i < (int)candidates.size(); i++) {
        int candidateBallot = mapBallot[i];

        table.setCell(i+1, 1, to_string(candidateBallot));
        table.setCell(i+1, 2, to_string(candidateBallot));
    }

    table.setCell(table.getNumRows()-1, 1, to_string(totalBallots));
};

//Generate two columns for votes and +- of next round
void IRProcessing::generateTableRound() {
    table.addCell(0, "Votes");
    table.addCell(0, "+-");

    for(int i = 0; i < (int)candidates.size(); i++) {
        table.addCell(i+1, "0");
        table.addCell(i+1, "0");
    }
};

void IRProcessing::redistributeTable() {
    generateTableRound();

    //Calculate for each candidate in their row respective of the round column
    for(int i = 0; i < (int)candidates.size(); i++) {
        //Get the current ballots and subtract the previous round ballots to get the difference for +-
        int candidateBallot = mapBallot[i];
        //Eliminated candidate is -1 in mapBallot
        if(candidateBallot == -1) {
            candidateBallot = 0;
        }

        //Calculate the +- by subtracting the current round ballot w/ previous round ballot
        int previousRound = stoi(table.getCell(i+1 , round*2-3));
        int difference = candidateBallot - previousRound;


        /*
        Round*2 formula comes from the following based on the table structure:
        Round 1 [1,2]
        Round 2 [3,4]
        Round 3 [5,6]

        If round 3, 3*2 = 6.
        6 is the column of +- of specific round.
        To get the Votes column of specific round, 6-1 = 5.

        */
        //Set Votes column
        table.setCell(i+1, round*2-1, to_string(candidateBallot));

        //Set +- column
        table.setCell(i+1, round*2, to_string(difference));

        //Set exhaustion piles if any
        table.setCell(table.getNumRows()-2, 1, to_string(exhausted));

    }
};
