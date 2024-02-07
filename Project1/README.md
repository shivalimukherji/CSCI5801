# repo-Team24
Matin Horri, Wenjiang Jiang, Shivali Mukherji, and Michael Vang

# Get started
Run the following commands:

cd Project1  
make mainAll  
./main or ./main <.csv files>

The program will prompt user to enter a command. Enter "read".

Then, it will prompt user to enter csv files. The user can either enter just one, or multiple at once. Enter "testing/data/IR.csv" for IR election or "testing/data/CPL.csv" for CPL election.

If the user enters just one file following the read command, the program will prompt the user again and ask if there are any other files they want to enter. The user can enter another csv file or enter no into the command line. 

If the user enters no, the program will process the files and run the election. 

The election process and the winners will be displayed.

# Unit Testing
Option 1:  
mkdir build <br />
cd build <br />
cmake ../ <br />
make <br />
cd testing/test <br />
./AllTests <br />

Option 2:  
cmake -S . -B build <br />
cmake --build build <br />
cd build && ctest <br />

If ctest fails, do the following in the build folder: <br />
ctest -C debug --verbose <br />

# Current Issues
Buglists contain the current issue.
Currently on Linux side, there is an error on parsing the Candidate's name. This can be seen in testing/data/IR3.csv and testing/data/IR4.csv. The program works
intended on Windows & Mac side. The audit file is still produced correctly with the candidate's name.
