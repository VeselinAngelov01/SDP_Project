#ifndef TABLE_H
#define TABLE_H

#include "Tree.h"
#include <fstream>
#include <regex>

class Table
{
private:
	//Table
	Tree* table;
	//Removes white-spaces
	void trim(std::string& data);
	//Split user input in vector (delimiter ' ')
	void splitString(std::string& data, std::vector<std::string>& arr);
	//Convert input string to numbers representing row and column
	void getAddress(std::string& input, size_t& row, size_t& column);
	//Function to insert in table
	void set(size_t row, size_t column, std::string& data);
	//Save table to file (.csv format)
	void save(std::string& path);
	//Help function to insert in file
	void setFromFile(std::string line,size_t currentRow, size_t& currentColumn);
	//Function to check if user wants to save file
	void saveOption();
public:
	//Constructor
	Table();
	//Destructor
	~Table();
	//Shows commands
	void helper();
	//Read user commands
	void readCommands();
};
#endif