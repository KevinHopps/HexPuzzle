//
//  HexPuzzle.cpp
//  HexPuzzle
//
//  Created by Kevin Hopps on 9/19/18.
//  Copyright © 2018 Kevin Hopps. All rights reserved.
//

#include "HexPuzzle.hpp"

#include <cassert>

//   a b c
//  d e f g
// h i j k l
//  m n o p
//   q r s
//
// The sum of any row must be (1 + 2 + ... + 18 + 19) / 5 = 38.
// The algorithm is to start with the rows of 3 around the outside.
// Once those all sum to 38, we move on to the six rows of 4. The
// first row of 4 will have two empty cells. Subsequent rows of
// 4 will have zero, one, or two empty cells, and after that, the
// center cell will be the only one remaining.
//

HexPuzzle::HexPuzzle()
{
    Reset();
    
    AddRow("abc");
    AddRow("cgl");
    AddRow("lps");
    AddRow("srq");
    AddRow("qmh");
    AddRow("hda");
    AddRow("bfkp");
    AddRow("gkor");
    AddRow("ponm");
    AddRow("rnid");
    AddRow("mieb");
    AddRow("defg");
    AddRow("aejos");
    AddRow("cfjnq");
    AddRow("lkjih");
}

HexPuzzle::CellValue HexPuzzle::Solve()
{
    int solved = 0;
    
    bool empty = true;
    for (std::size_t i = 0; empty && i < kNumCells; ++i)
    {
        if (fCell[i] != 0)
            empty = false;
    }
    
    if (!empty)
    {
        solved = SolveRemainingRows(0);
    }
    else
    {
        for (CellValue value = 1; !solved && value <= kNumCells; ++value)
        {
            SetCell(&fCell[0], value);
            solved = SolveRemainingRows(0);
            if (!solved)
                SetCell(&fCell[0], 0);
        }
    }
    
    return solved;
}

void HexPuzzle::Reset()
{
    for (std::size_t i = 0; i < kNumCells; ++i)
    {
        fCell[i] = 0;
        fUsed[i] = 0;
    }
    fUsed[kNumCells] = 0;
    fNumTries = 0;
}

HexPuzzle::CellName HexPuzzle::IsUsed(CellValue value) const
{
    CellName result = 0;
    if (0 < value && value <= kNumCells)
        result = fUsed[value];
    return result;
}

HexPuzzle::CellValue HexPuzzle::SetCell(CellName name, CellValue value)
{
    assert(0 <= value && value <= kNumCells);
    std::size_t i = GetIndex(name);
    CellValue result = SetCell(&fCell[i], value);
    return result;
}

HexPuzzle::CellValue HexPuzzle::GetCell(CellName name) const
{
    std::size_t i = GetIndex(name);
    return fCell[i];
}

std::size_t HexPuzzle::GetNumTries() const
{
    return fNumTries;
}

std::size_t HexPuzzle::GetIndex(CellName name)
{
    std::size_t i = name - 'a';
    assert(i < kNumCells);
    return i;
}

HexPuzzle::CellValue HexPuzzle::SumRow(const Row& row)
{
    CellValue result = 0;
    
    for (auto cell : row)
        result += *cell;
    
    return result;
}

void HexPuzzle::AddRow(const CellName* cellNames)
{
    Row row;
    
    CellName name;
    for (std::size_t i = 0; (name = cellNames[i]) != 0; ++i)
    {
        std::size_t j = GetIndex(name);
        row.push_back(&fCell[j]);
    }
    
    fRows.push_back(row);
}

HexPuzzle::CellValue HexPuzzle::SetCell(CellValue* cell, CellValue value)
{
    std::size_t i = cell - fCell;
    
    if (value != 0)
        ++fNumTries;
    
    int oldValue = *cell;
    *cell = value;
    
    fUsed[oldValue] = 0;
    fUsed[value] = 'a' + i;
    
    std::cout << *this << '\n';
    
    return oldValue;
}

HexPuzzle::CellValue HexPuzzle::SolveRemainingRows(std::size_t iRow)
{
    static CellValue kExpectedSum = 38; // (1 + 2 + ... 18 + 19) / 5
    CellValue solved = 0;
    
    Row& row = fRows[iRow];
    Row emptyCells;
    CellValue runningSum = 0;
    for (auto cell : row)
    {
        if (*cell == 0)
            emptyCells.push_back(cell);
        else
            runningSum += *cell;
    }
    
    std::size_t nextRow = iRow + 1;
    
    if (emptyCells.size() == 0)
    {
        if (runningSum == kExpectedSum)
        {
            if (nextRow == fRows.size())
                solved = kExpectedSum;
            else
                solved = SolveRemainingRows(nextRow);
        }
    }
    else if (emptyCells.size() == 1)
    {
        CellValue value = kExpectedSum - runningSum;
        if (0 <= value && value <= kNumCells && !fUsed[value])
        {
            SetCell(emptyCells[0], value);
            if (nextRow == fRows.size())
                solved = kExpectedSum;
            else
            {
                solved = SolveRemainingRows(nextRow);
                if (!solved)
                    SetCell(emptyCells[0], 0);
            }
        }
    }
    else
    {
        assert(emptyCells.size() == 2);
        CellValue valueA = kExpectedSum - runningSum;
        if (valueA > kNumCells)
            valueA = kNumCells;
        CellValue valueB = 0;
        for (; !solved && valueA > (valueB = kExpectedSum - runningSum - valueA); --valueA)
        {
            if (valueA != valueB && !fUsed[valueA] && !fUsed[valueB])
            {
                SetCell(emptyCells[0], valueA);
                SetCell(emptyCells[1], valueB);
                solved = SolveRemainingRows(nextRow);
                if (!solved)
                {
                    SetCell(emptyCells[0], 0);
                    SetCell(emptyCells[1], 0);
                    SetCell(emptyCells[0], valueB);
                    SetCell(emptyCells[1], valueA);
                    solved = SolveRemainingRows(nextRow);
                    if (!solved)
                    {
                        SetCell(emptyCells[0], 0);
                        SetCell(emptyCells[1], 0);
                    }
                }
            }
        }
    }
    
    return solved;
}

std::ostream& operator<<(std::ostream& os, const HexPuzzle& hp)
{
    char buf[256];
    char* bp = buf;
    bp += sprintf(bp, "    %2d  %2d  %2d\n", hp.GetCell('a'), hp.GetCell('b'), hp.GetCell('c'));
    bp += sprintf(bp, "  %2d  %2d  %2d  %2d\n", hp.GetCell('d'), hp.GetCell('e'), hp.GetCell('f'), hp.GetCell('g'));
    bp += sprintf(bp, "%2d  %2d  %2d  %2d  %2d\n", hp.GetCell('h'), hp.GetCell('i'), hp.GetCell('j'), hp.GetCell('k'), hp.GetCell('l'));
    bp += sprintf(bp, "  %2d  %2d  %2d  %2d\n", hp.GetCell('m'), hp.GetCell('n'), hp.GetCell('o'), hp.GetCell('p'));
    bp += sprintf(bp, "    %2d  %2d  %2d\n", hp.GetCell('q'), hp.GetCell('r'), hp.GetCell('s'));
    bp += sprintf(bp, "tries=%d\n", static_cast<int>(hp.GetNumTries()));
    os << buf;
    return os;
}
