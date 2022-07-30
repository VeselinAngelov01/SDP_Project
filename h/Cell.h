#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <string>
#include <cmath>
class Cell
{
private:
    std::string data;
    size_t row;
    size_t column;
public:
    Cell();
    Cell(size_t row, size_t column, std::string data);

    size_t getRow() const;
    size_t getColumn() const;
    std::string getData() const;

    void setData(std::string& data);
};

#endif