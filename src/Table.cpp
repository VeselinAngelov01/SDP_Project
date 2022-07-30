#include "Table.h"

// Function to validate formula (regex)
void checkFormulaSyntax(const std::string &data)
{
	std::string regexCheck = "=(not\\(*(-?[1-9][0-9]*|R[0-9]+C[0-9]+|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*|\\(*(-?[0-9]*|R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*(\\(*[+/*-](-?[0-9]*|-?R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*)+|if\\(+((\\(*(-?[0-9]*|R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*(\\(*[+/*-](-?[0-9]*|-?R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*)*)(<|>|!=|==)(\\(*(-?[0-9]*|R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*(\\(*[+/*-](-?[0-9]*|-?R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*)*)+((and|or)(\\(*(-?[0-9]*|R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*(\\(*[+/*-](-?[0-9]*|-?R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*)*)(<|>|!=|==)(\\(*(-?[0-9]*|R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*(\\(*[+/*-](-?[0-9]*|-?R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*)*)+)*|not\\(*(-?[1-9][0-9]*|R[0-9]+C[0-9]+|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])\\)*),\\(*(-?[0-9]*|R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])+\\)*,\\(*(-?[0-9]*|R[0-9]*C[0-9]*|R\\[-?[0-9]*\\]C\\[-?[0-9]*\\])+\\)*\\)+)+";
	std::regex check(regexCheck);
	if (std::regex_match(data, check))
	{
		int index = 0;
		size_t size = data.length();
		int open = 0;
		while (index < size)
		{
			if (open < 0)
				throw std::invalid_argument("Invalid brackets!");
			else if (data[index] == '(')
				open++;
			else if (data[index] == ')')
				open--;
			index++;
		}
		if (open != 0)
			throw std::invalid_argument("Invalid brackets!");
	}
	else
		throw std::invalid_argument("Invalid value!");
}

// Correct data inserted - string/formula/error
void Validate(const std::string &data, unsigned int row, unsigned int column)
{
	if (data.length() == 0 || (data[0] == '"' && data[data.length() - 1] == '"'))
	{
		// text
		return;
	}
	else if (data[0] == '=' && data.length() > 1)
	{
		// formula
		checkFormulaSyntax(data);
	}
	else
		throw std::invalid_argument("Invalid value in set function!");
}

Table::Table()
{
	this->table = new Tree();
}

Table::~Table()
{
	delete this->table;
}

void Table::helper()
{
	std::cout << "Commands:\n1.SET address value\n2.PRINT VAL address\n3.PRINT EXPR address\n4.PRINT VAL ALL\n5.PRINT EXPR ALL\n6.SAVE file\n7.LOAD file\n8.++ address\n9.-- address\n10.COUNT start_address end_address\n11.SUM start_address end_address\n12.EXIT\n";
}

void Table::trim(std::string &data)
{
	size_t size = data.length() - 1;
	for (size_t i = 0; i < size;)
	{
		if (data[i] == ' ' && data[i + 1] == ' ')
		{
			data.erase(i);
			--size;
		}
		else
			++i;
	}
}

void Table::splitString(std::string &data, std::vector<std::string> &arr)
{
	if (data.length() == 0)
		throw std::invalid_argument("Empty command!");
	size_t startIndex = 0;
	size_t endIndex = 0;
	std::string temp;
	while (true)
	{
		endIndex = data.find(' ', startIndex);
		if (endIndex == -1)
		{
			temp = data.substr(startIndex);
			if (temp.length() > 0)
				arr.push_back(temp);
			break;
		}
		else
		{
			temp = data.substr(startIndex, endIndex - startIndex);
			if (temp.length() > 0)
				arr.push_back(temp);
			startIndex = endIndex + 1;
		}
	}
}

void Table::getAddress(std::string &input, size_t &row, size_t &column)
{
	if (input[0] != 'R')
		throw std::invalid_argument("No row value found!");
	size_t columnIndex = input.find('C', 1);
	if (columnIndex == -1)
		throw std::invalid_argument("No column value found!");
	std::string rowNumber, colNumber;
	// Get only number without symbols 'R' and 'C'
	rowNumber = input.substr(1, columnIndex - 1);
	colNumber = input.substr(columnIndex + 1);
	// Check if there is something after this symbols
	if (rowNumber.length() == 0 || colNumber.length() == 0)
		throw std::invalid_argument("No number for row/column!");
	// Check if numbers are negative
	else if (rowNumber[0] == '-' || colNumber[0] == '-')
		throw std::invalid_argument("Negative number in row/column index!");
	try
	{
		row = std::stoul(rowNumber);
		column = std::stoul(colNumber);
	}
	catch (const std::exception)
	{
		throw std::invalid_argument("Invalid argument for row/column");
	}
}

void Table::set(size_t row, size_t column, std::string &data)
{
	try
	{
		// Validate(data, row, column); // TO DO
		table->insert(row, column, data);
	}
	catch (const std::exception &e)
	{
		std::cout << e.what() << "at: R:" << row << " C:" << column << std::endl;
	}
}

void Table::save(std::string &path)
{
	std::ofstream file;
	// Check for correct file format
	if (path.length() < 5 || path.substr(path.length() - 4) != ".csv")
		throw std::invalid_argument("Invalid path!");
	// Clear file to save table
	file.open(path, std::ios::trunc);
	if (file.is_open())
	{
		std::string exportStr = table->exportString();
		file << exportStr;
		file.close();
		std::cout << "File saved!\n";
	}
	else
		throw std::invalid_argument("Can not open file for saving!");
}

void Table::setFromFile(std::string line, size_t currentRow, size_t &currentColumn)
{
	size_t startIndex = 0;
	size_t endIndex = 0;
	while (true)
	{
		// Find delimiter
		endIndex = line.find(';', startIndex);
		if (endIndex == -1)
		{
			// If it is last stop inserting
			std::string value = line.substr(startIndex);
			// Cell can be empty
			if (value.length() > 0)
			{
				if (value[0] != '=')
				{
					value.erase(value.length() - 1);
					value.erase(0, 1);
				}
				set(currentRow, currentColumn, value);
			}
			break;
		}
		else
		{
			// Else insert value increase current column and continue
			std::string value = line.substr(startIndex, endIndex - startIndex);

			// Cell can be empty
			if (value.length() > 0)
			{
				// If text remove '\"'
				if (value[0] != '=')
				{
					value.erase(value.length() - 1);
					value.erase(0, 1);
				}
				set(currentRow, currentColumn, value);
			}
			startIndex = endIndex + 1;
			currentColumn++;
		}
	}
}

void Table::saveOption()
{
	std::cout << "Changes not saved. Do you want to save it? (Y,N)\n";
	std::string answer;
	while (true)
	{
		std::getline(std::cin, answer);
		if (answer == "Y")
		{
			std::string pathToSave;
			std::cout << "Enter path to save file:\n";
			std::getline(std::cin, pathToSave);
			save(pathToSave);
			break;
		}
		else if (answer == "N")
		{
			// Just delete opened table from memory
			delete this->table;
			this->table = new Tree();
			break;
		}
		else
			std::cout << "Please enter 'Y' or 'N'\n";
	}
}

void Table::readCommands()
{
	helper();
	std::string userCommand;
	std::vector<std::string> arrayCommands;
	std::string firstArgument;
	size_t row = 0;
	size_t col = 0;
	while (true)
	{
		try
		{
			std::getline(std::cin, userCommand);
			trim(userCommand);
			splitString(userCommand, arrayCommands);
			if (arrayCommands.size() == 0)
				throw std::invalid_argument("Empty command input!");
			firstArgument = arrayCommands[0];
			if (firstArgument == "SET")
			{
				if (arrayCommands.size() != 3)
					throw std::invalid_argument("Invalid count of arguments!");
				getAddress(arrayCommands[1], row, col);
				set(row, col, arrayCommands[2]);
			}
			else if (firstArgument == "PRINT")
			{
				if (arrayCommands.size() != 3)
					throw std::invalid_argument("Invalid count of arguments!");
				std::string secondArgument = arrayCommands[1];
				std::string thirdArgument = arrayCommands[2];
				if (secondArgument == "VAL")
				{
					if (thirdArgument == "ALL")
					{
						std::cout << table->print(false);
					}
					else
					{
						getAddress(thirdArgument, row, col);
						std::cout << table->getNodeValue(row, col);
					}
				}
				else if (secondArgument == "EXPR")
				{
					if (thirdArgument == "ALL")
					{
						std::cout << table->print(true);
					}
					else
					{
						getAddress(thirdArgument, row, col);
						std::cout << table->getNodeExpr(row, col);
					}
				}
				else
					throw std::invalid_argument("Invalid second argument!");
			}
			else if (firstArgument == "SAVE")
			{
				if (arrayCommands.size() != 2)
					throw std::invalid_argument("Invalid count of arguments!");
				save(arrayCommands[1]);
				// After saving file delete old table from memory
				// And add new (like "new file" in other programs)
				delete this->table;
				this->table = new Tree();
			}
			else if (firstArgument == "LOAD")
			{
				if (arrayCommands.size() != 2)
					throw std::invalid_argument("Invalid count of arguments!");
				std::string line;
				std::ifstream file;
				size_t rowLoad = 0;	   // Current row in file
				size_t colLoad = 0;	   // Cureent column in file
				size_t lastColumn = 0; // Last column read column
				bool firstLine = true; // Indicates if this is first read line
				// If we have already opened table
				if (table->getSize() > 0)
					saveOption();
				file.open(arrayCommands[1]);
				if (file.is_open())
				{
					if (arrayCommands[1].length() < 5 || arrayCommands[1].substr(arrayCommands[1].length() - 4) != ".csv")
					{
						file.close();
						throw std::invalid_argument("Invalild file extension!");
					}
					// While we can read from file
					while (getline(file, line))
					{
						setFromFile(line, rowLoad, colLoad);
						if (firstLine)
						{
							lastColumn = colLoad;
							firstLine = false;
						}
						// If we have two rows with different count of columns => invalid file
						if (lastColumn != colLoad)
						{
							delete table;
							this->table = new Tree();
							file.close();
							throw std::invalid_argument("Error reading file! Check rows size!");
						}
						rowLoad++;
						lastColumn = colLoad;
						colLoad = 0;
					}
					file.close();
				}
				else
					throw std::invalid_argument("Can not open file!");
			}
			else if (firstArgument == "--")
			{
				getAddress(arrayCommands[1], row, col);
				table->change(row, col, false);
			}
			else if (firstArgument == "++")
			{
				try
				{
					getAddress(arrayCommands[1], row, col);
					table->change(row, col, true);
				}
				catch (const std::exception &e)
				{
					std::cout << e.what() << std::endl;
				}
			}
			else if (firstArgument == "COUNT")
			{
				if (arrayCommands.size() != 3)
					throw std::invalid_argument("Invalid count of arguments!");
				getAddress(arrayCommands[1], row, col);
				size_t secondRow = 0, secondCol = 0;
				getAddress(arrayCommands[2], secondRow, secondCol);
				std::cout << "Count of non-empty cells in given interval: " << table->count(row, col, secondRow, secondCol) << std::endl;
			}
			else if (firstArgument == "SUM")
			{
				if (arrayCommands.size() != 3)
					throw std::invalid_argument("Invalid count of arguments!");
				getAddress(arrayCommands[1], row, col);
				size_t secondRow = 0, secondCol = 0;
				getAddress(arrayCommands[2], secondRow, secondCol);
				std::cout << "Sum of non-empty cells in given interval: " << table->sum(row, col, secondRow, secondCol) << std::endl;
			}
			else if (firstArgument == "EXIT")
			{
				if (table->getSize() > 0)
					saveOption();
				std::cout << "Exiting program...\n";
				break;
			}
			else
				throw std::invalid_argument("Invald command!");
		}
		catch (const std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
		arrayCommands.clear();
		userCommand.clear();
		row = 0;
		col = 0;
	}
}
