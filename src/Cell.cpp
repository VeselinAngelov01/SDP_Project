#include "Cell.h"

Cell::Cell()
{
	this->data = "";
	this->row = 0;
	this->column = 0;
}

Cell::Cell(size_t row, size_t column, std::string data)
{
	this->data = data;
	this->row = row;
	this->column = column;
}

size_t Cell::getRow() const
{
	return this->row;
}

size_t Cell::getColumn() const
{
	return this->column;
}

std::string Cell::getData() const
{
	return this->data;
}

void Cell::setData(std::string& data)
{
	this->data = data;
}