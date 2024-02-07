/*
 * @file main.cpp
 * @authors Matin horri (horri031@umn.edu), Shivali Mukherji (mukhe105), Michael Vang (vang2891@umn.edu)
 * @brief
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "include/IRVoteSystem.h"
#include "include/IRProcessing.h"
#include "include/CPLVoteSystem.h"
#include "include/CPLProcessing.h"
#include "include/PoVoteSystem.h"
#include "include/PoProcessing.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <sstream>
#include <vector>

using namespace std;


//VARIABLES for MAIN
string input;
FILE *file;
char buffer[100];
vector<string> input_files;

enum ElectionType {
  IR,
  CPL,
  PO
};

IBallotProcessing* ballotSystem;
IVoteBallot* processedBallot;
string winner;
ElectionType election;
AuditFile audit = AuditFile("audit.txt");

/*
 * @brief Displays the welcome screen for the voting system software.
 */

void welcomeScreen() {
  cout << "Voting System Software v1.0.0" << endl;
  cout << endl;
  cout << "To view all the commands, type in 'help'" << endl;
  cout << "The following command will return details of how to use the command 'cmd': type in 'man <cmd>'" << endl;
  cout <<  "To load in a CSV file for process, type in 'read <filename>.csv'" <<endl;
  cout << "'e' or 'exit' to exit" << endl;
};

/*
 * @brief Prompts the user to enter a command.
 */

void prompt() {
  cout << "Please enter a command: ";
};

/*
 * @brief Displays the manual page for a specific command.
 *
 * @param str The name of the command to display the manual page for.
 */

void man(string str) {
  if (str == "read") {
    cout << "This command is used to read a CSV file entered with this command into the terminal: read_file <filename> ";
  }
  else if (str == "write_file") {
    cout << "This command writes a text file in audit format";
  }
  else if (str == "help") {
    cout << "This command shows a list of available commands in the terminal";
  }
  else if (str == "report_election") {
    cout << " This command returns the information of the election including the type of election, number of seats, the candidates, the total votes and the winner. Use option “-attribute” to report the specified information.";
  }
  else if (str == "no") {
    cout << "This command stops the reading of files";
  }
  else {
    cout << "“man cmd” command returns the details of how to use the command “cmd”.";
  }
  cout << endl;
};

/*
 * @brief Displays the list of available commands in the terminal.
 */

void help() {
  const char *list =
  "read\n"
  "write_file\n"
  "man\n"
  "help\n"
  "report_election\n"
  "no\n";
  cout << list;
};

/*
 * @brief Reads a CSV file containing ballot information for a specific election type.
 *
 * @param input The name of the CSV file to read.
 *
 * @return true if the file was successfully opened and read.
 * @return false if the file could not be opened or read.
 */


bool readFile(string input) {
  //Open the file and read it
  const char * path = input.c_str();
  file = fopen (path, "r");

  //Error checks
  if(file == NULL) {
    return false;
  }

  //Get the first line
  fgets(buffer, 100, file);
  strtok(buffer, ",");
  strtok(buffer, "\n");

  // if first line of file is IR, ballot is processed and IR election is run
  if(strcmp(buffer, "IR") == 0) {
    ballotSystem = new IRProcessing(file, input_files);
    election = IR;
  }
  // if first line of file is CPL, ballot is processed and CPL election is run
  else if(strcmp(buffer, "CPL") == 0) {
    ballotSystem = new CPLProcessing(file, input_files);
    election = CPL;
  }
  else if(strcmp(buffer, "PO") == 0) {
    ballotSystem = new PoProcessing(file, input_files);
    election = PO;
  }
  else {
    //If the first line does not match any of the software's accounted election.
    return false;
  }

  return true;

};

bool conductVotingSystem() {
  string date;
  string total;
  switch(election) {
    case IR:
    {
      // Create IR Processing
      IRProcessing* IRProcess = static_cast<IRProcessing*>(ballotSystem);

      // Auditing purposes. Prompt for date of election conducted for naming convention.
      cout<<"Please Enter The Date(MM.DD.YYYY): ";
      cin>>date;
      total = "IR_"+date + "_Audit";
      audit.labelFile(total);
      audit.open();

      // Run the processing system
      IRProcess->setAudit(&audit);
      IRProcess->setUp();

      // Get the processed ballot. The information needed for the voting system to run
      IRBallot* IRProcessBallot = static_cast<IRBallot*>(processedBallot);
      IRProcessBallot = IRProcess->output();

      // Voting system and conducting to find a winner
      IRVoteSystem* IRVote = new IRVoteSystem(IRProcess, IRProcessBallot);
      IRVote->setAuditing();
      IRVote->setAuditFile(&audit);
      IRVote->startElection();

      // Winner
      cout << "The winner is " << IRVote->getWinner().getName() << endl << endl;

      // Produce the IR table
      IRProcess->produceTable();

      audit.write("-----WINNER-----");
      audit.write("The winner is " + IRVote->getWinner().getName());
      audit.close();

      return true;
      break;
    }

    case CPL:
    {
      CPLProcessing* CPLProcess = static_cast<CPLProcessing*>(ballotSystem);

      cout<<"Please Enter The Date(MM.DD.YYYY): ";
      cin>>date;

      total = "CPL_"+date + "_Audit";
      audit.labelFile(total);

      audit.open();

      CPLProcess->setAudit(&audit);
      CPLProcess->setUp();

      CPLVoteBallot* CPLProcessBallot = static_cast<CPLVoteBallot*>(processedBallot);
      CPLProcessBallot = CPLProcess->output();

      CPLVoteSystem* CPLVote = new CPLVoteSystem(CPLProcessBallot);

      CPLVote->setAuditFile(&audit);

      CPLVote->startElection();

      std::vector<Candidate> seatWinners = CPLVote->getWinner();
      cout << "CPL winners:" << endl;
      for (int i=0; i < (int)seatWinners.size();i++) {
        cout << "Name: " << seatWinners[i].getName() << ", Party: " << seatWinners[i].getParty() << endl;
      }

      audit.close();

      return true;
      break;
    }

    case PO:
    {
      // create PO Process object
      PoProcessing* PoProcess = static_cast<PoProcessing*>(ballotSystem);
      cout<<"Please Enter The Date(MM.DD.YYYY): ";
      cin>>date;

      total = "PO_"+date + "_Audit";
      audit.labelFile(total);
      audit.open();


      PoProcess->setAudit(&audit);
      PoProcess->setUp();

      // create processed ballot object
      PoBallot* PoProcessBallot = static_cast<PoBallot*>(processedBallot);
      PoProcessBallot = PoProcess->output();

      PoVoteSystem* PoVote = new PoVoteSystem(PoProcessBallot);
      PoVote->setAuditing();

      PoVote->setAuditFile(&audit);
      PoVote->startElection();

      cout << "The winner is " << PoVote->getWinner().getName() << endl;

      audit.write("-----WINNER-----");
      audit.write("The winner is " + PoVote->getWinner().getName());

      audit.close();
      return true;
      break;
    }

  }
  return false;
}


/*
 * @brief The main function for the voting system software.
 *
 * @return 0 if the program runs successfully.
 */

int main(int argc, char *argv[]) {
  string input_file;
  // check if the number of arguments entered is greater than 1
  if(argc > 1) {
    // put store input file as second argument entered
    input_file = argv[1];

    // populate input_files vector
    for (int i = 1; i < argc; ++i) {
        // add each file entered by user to vector
        input_files.push_back(argv[i]);
    }
  }

  if (input_file.length() != 0) {
      if(!readFile(input_file)) {
        cout << "Invalid file" << endl;
        return -1;
      }
      cout << endl << "--ELECTION--" << endl;;
      conductVotingSystem();
      return 0;
  }

  //UI Prompts the user for information
  welcomeScreen();
  while (true) {
    cout << endl;
    prompt();
    getline(cin, input);

    stringstream s(input);
    string parse;
    s >> parse;

    //Accounts for READ
    if (parse == "read") {
      // prompt user to enter file if file is not entered follwing read command
      if(!(s >> parse)) {
        cout << "Please enter Ballot file in CSV format (path/IRBallot.csv): ";
        cin >> parse;
      }

      // Add files to vector
      input_files.push_back(parse);
      while((s >> parse)) {
        input_files.push_back(parse);
      }

      // If there's more incoming files:
      while(true) {
      cout << "Enter any additional files in CSV format. type (no) to proceed to run election: ";
      cin >> parse;

      if(parse == "no") {
        //Start the election with the incoming files
        if(readFile(input_files[0])) {
          cout << endl;
          cout << "--ELECTION--" << endl;;
          conductVotingSystem();
          input_files.clear();
          break;
        } else {
          cout << "Invalid files" << endl;
          input_files.clear();
          break;
        }

      } else {
        input_files.push_back(parse);
        int len = static_cast<int>(input_files.size());

        cout << endl << "Files inputted: " << input_files.size() << endl;
        for(int i = 0; i < len; ++i) {
          cout << input_files[i] << endl;
          }
        }

      }

      continue;
    }


    //Accounts for MAN
    else if (parse == "man")
    {
      if(!(s >> parse)) {
        cout << "What command are you trying to get help with? ";
        cin >> parse;
      }

      man(parse);
      continue;
    }

    //Accounts for HELP
    else if (input == "help")
    {
      help();
      continue;
    }

    //Accounts to reprint the election results.
    //UNIMPLEMENTED
    else if(input == "report_election") {
      continue;
    }

    //Accounts to reoutput the audit file
    //UNIMPLEMENTED
    else if(input == "write-file") {
      continue;
    }

    //Terminate
    else if ((input == "e") || (input == "exit")) {
      break;
    }

    // else {
    //   cout << input << " is not a command. Type 'help' for a list of commands." << endl;
    // }

  }

  return 0;

};
