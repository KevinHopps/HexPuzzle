//
//  HexPuzzle.hpp
//  HexPuzzle
//
//  Created by Kevin Hopps on 9/19/18.
//  Copyright © 2018 Kevin Hopps. All rights reserved.
//

#ifndef HexPuzzle_hpp
#define HexPuzzle_hpp

#include <iostream>
#include <vector>

/*
   a b c
  d e f g
 h i j k l
  m n o p
   q r s
 
 Place a number 1-19 in each cell such that each row
 across, each row along a 60 degree diagonal, and each
 row along a -60 degree diagonal has a common sum.
 */

class HexPuzzle
{
public:
    
    using CellName = char;
    using CellValue = int;
    
    HexPuzzle();
    
    // The function solve will start from the current state
    // and return either the common sum of each row, or zero
    // if the puzzle cannot be solved.
    //
    int Solve();
    
    void Reset();
    
    CellName IsUsed(CellValue value) const; // returns cell holding the value or 0
    CellValue SetCell(CellName cellName, CellValue value); // returns old value
    CellValue GetCell(CellName cellName) const;
    std::size_t GetNumTries() const;
    
private:
    
    using Row = std::vector<CellValue*>;
    using RowVec = std::vector<Row>;
    
    static const std::size_t kNumCells = 's' - 'a' + 1;
    
    static std::size_t GetIndex(char name);
    static CellValue SumRow(const Row& row);
    
    std::size_t fNumTries;
    CellValue fCell[kNumCells];
    CellName fUsed[kNumCells + 1]; // indexed by values 1..kNumCells
    RowVec fRows;
    
    void AddRow(const CellName* cellNames);
    CellValue SetCell(CellValue* cell, CellValue value);
    CellValue SolveRemainingRows(std::size_t iRow);
    
};

std::ostream& operator<<(std::ostream& os, const HexPuzzle& hp);

#endif /* HexPuzzle_hpp */
