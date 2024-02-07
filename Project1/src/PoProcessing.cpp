#include "include/PoProcessing.h"
// #include "library/strsep.h"

#include <algorithm>
#include <cstring>

//Helper function to read the next line and return
//the content seperated by ,
char* PoProcessing::nextLine() {
    char *p;
    fgets(buffer, 100, file);
    char* duplicate = strdup(buffer);
    
    p = strtok(duplicate, ",");
    return p;
};

bool PoProcessing::setUp() {
    
    //Read the header information of the first file.
    //If sucessful, read the ballots and distribute them out. 
    if(readHeader(0)) {
        read();
    } else {
        return false;
    }

    return true;
};

//Multiple indicates the file coming in is not the original file initalized. 
bool PoProcessing::readHeader(int multiple) {
    char *p;
    int numCandidates_;
    vector<Candidate*> candidates_;
    vector<int> mapBallot_;

    //Error check
    if(file == NULL) {
        return false;
    }

    //Account for the first line (PO) header
    if(multiple) {
        //Get the next line of the CSV file (in this case the very first line of CSV)
        if ( fgets (buffer , 100 , file) == NULL ) {
            return false;
        }

        //If the first line of CSV file is not PO.
        strtok(buffer, ",");
        strtok(buffer, "\n");
        if(strcmp(buffer, "PO") != 0) {
            return false;
        }
    };

    //Get the second line (numCandidates)
    if((p = nextLine()) == NULL) {
        return false;
    };
    numCandidates_ = stoi(p);

    //Get the third line: candidates
    fgets(buffer, 100, file);
    char* duplicate = strdup(buffer);
    p = strtok(duplicate, "]");
    
    while(p != NULL) {
        string s = p;
        s.erase(remove(s.begin(),s.end(), '['), s.cend());
        s.erase(remove(s.begin(),s.end(), ','), s.cend());
        Candidate *candidate = new Candidate(s);
        candidates_.push_back(candidate);
        p = strtok(NULL, "]");
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
            cout << "Fail A" << endl;
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
    }

    bLinesToRead = atoi(p);
    totalBallots += bLinesToRead;
   
    // Write to the audit file if the option is turned on
    // Contains information to the election

    if(auditing) {
        audit->write("PO");
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

bool PoProcessing::read() {
    char *p;
    // cout<<"this is the read function";
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
            }
            audit->write("\n");
        }
    }
    
    calculate();
    fclose(file);

    return true;
}

void PoProcessing::calculate() {
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

PoBallot* PoProcessing::output() {
    PoBallot* output = new PoBallot(candidates, mapBallot, mapPercentage);
    output->setTotalBallot(totalBallots);
    return output;
};
