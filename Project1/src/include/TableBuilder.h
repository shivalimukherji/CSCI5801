/**
 * @file TableBuilder.h
 * @author vang2891@umn.edu
 * @brief 
 * @version 0.1
 * @date 2023-04-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef TABLEBUILDER_H
#define TABLEBUILDER_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using std::cout;

class TableBuilder {
    public:
        /**
         * @brief Construct a new Table Builder object
         * 
         */
        TableBuilder() {};

        /**
         * @brief Add a new row to the table
         * 
         */
        void buildRow() {
            rows.push_back(new std::vector<std::string>);
            numRows++;
            return;
        };
        
        /**
         * @brief Add a new cell at indicated row with data
         * 
         * @param row int
         * @param data string
         */
        void addCell(int row, std::string data);

        /**
         * @brief Get the Cell at desired row and cell.
         * 
         * @param row int
         * @param cell int
         * @return std::string 
         */
        std::string getCell(int row, int cell);

        /**
         * @brief Set the Cell at desired row and cell.
         * 
         * @param row int
         * @param cell int
         * @param data string
         */
        void setCell(int row, int cell, std::string data);

        /**
         * @brief Print out the table contents
         * 
         */
        void build();

/*
----------GETTERS + SETTERS
*/
        /**
         * @brief Get the Rows 
         * 
         * @return std::vector<std::vector<std::string>*> 
         */
        std::vector<std::vector<std::string>*> getRows() const { return rows; }

        /**
         * @brief Set the Rows
         * 
         * @param rows_ 
         */
        void setRows(const std::vector<std::vector<std::string>*> &rows_) { rows = rows_; }

        /**
         * @brief Get the number of rows
         * 
         * @return int 
         */
        int getNumRows() const { return numRows; }

        /**
         * @brief Set the number of rows
         * 
         * @param numRows_ 
         */
        void setNumRows(int numRows_) { numRows = numRows_; }

    private:
        std::vector<std::vector<std::string>*> rows = {}; ///< Vector of string vector pointers holding the rows of the table.
        int numRows = 0; ///< Holds for how many rows the table has.
};

#endif // TABLEBUILDER_H