#include "include/TableBuilder.h"

void TableBuilder::addCell(int row, std::string data) {
    //Error checks
    if(row >= (int)rows.size()) {
        return;
    }

    if(row < 0) {
        return;
    }

    //Add a new cell to the row
    rows[row]->push_back(data);
    return;
}

std::string TableBuilder::getCell(int row, int cell) {
    //Error checks
    if(row >= (int)rows.size()) {
        return " ";
    }

    if(row < 0 || cell < 0) {
        return " ";
    }

    if(cell >= (int)rows[row]->size()) {
        return " ";
    }

    //Get the cell at row cell
    std::vector<std::string> *desired_row = rows[row];
    return desired_row->at(cell);
};

void TableBuilder::setCell(int row, int cell, std::string data) {
    //Error checks
    if(row >= (int)rows.size()) {
        return;
    }

    if(row < 0 || cell < 0) {
        return;
    }

    if(cell >= (int)rows[row]->size()) {
        return;
    }
    
    //Set the cell at row cell
    rows[row]->at(cell) = data;

    return;
};

void TableBuilder::build() {
    int len = static_cast<int>(rows.size());
    int row_len;

    //Loop to print out the table
    for(int i = 0; i < len; i++) {
        row_len = static_cast<int>(rows[i]->size());
        for(int j = 0; j < row_len; j++) {
            //Left + setw + setfill is for alignment
            cout << std::left << std::setw(15) << std::setfill(' ') << rows[i]->at(j);
        }
        cout << std::endl;
    };

};
