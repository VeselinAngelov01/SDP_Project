#include "Tree.h"

/*
Operand:      Level:
()        		1
not       		2
* /       		3
+ -       		4
< >       		6
== !=     		7
and       		11
or        		12
if        		13
*/

// Variable for print function
// 8 + 2 for ""
const size_t DOUBLE_SIZE = 10;

Tree::Tree()
{
	this->root = nullptr;
	this->size = 0;
	this->max_row = 0;
	this->max_column = 0;
	this->max_length = 0;
}

Tree::~Tree()
{
	clear(this->root);
}

void Tree::clear(Node *&root)
{
	if (root)
	{
		clear(root->left);
		clear(root->right);
		delete root;
		root = nullptr;
	}
}

void Tree::insert(size_t row, size_t column, std::string data)
{
	this->root = insertNode(this->root, row, column, data);
	if (row > max_row)
		max_row = row;
	if (column > max_column)
		max_column = column;
	if (data.length() > max_length)
		max_length = data.length();
}

Node *Tree::insertNode(Node *&node, size_t row, size_t column, std::string data)
{
	// Create new node
	if (!node)
	{
		Node *temp = new Node();
		temp->height = 1;
		temp->left = nullptr;
		temp->right = nullptr;
		Cell val(row, column, data);
		temp->value = val;
		this->size++;
		return temp;
	}
	// In this tree nodes (cells) are arranged first by row next by column
	// If row we want is smaller than current or
	// rows are equal but column is smaller  ===>  go left
	if (row < node->value.getRow() || (row == node->value.getRow() && column < node->value.getColumn()))
	{
		node->left = insertNode(node->left, row, column, data);
	}
	// If row we want is bigger than current or
	// rows are equal but column is bigger  ===>  go right
	else if (row > node->value.getRow() || (row == node->value.getRow() && column > node->value.getColumn()))
	{
		node->right = insertNode(node->right, row, column, data);
	}
	// Else row = current row and column = current column
	// ===> change value
	else
	{
		node->value.setData(data);
	}

	node->height = 1 + max(height(node->left), height(node->right));
	int balance = getBalance(node);

	// left left
	if (balance > 1 && (row < node->left->value.getRow() || (row == node->left->value.getRow() && column < node->left->value.getColumn())))
		return rightRotate(node);

	// right right
	if (balance < -1 && (row > node->right->value.getRow() || (row == node->right->value.getRow() && column > node->right->value.getColumn())))
		return leftRotate(node);

	// left right
	if (balance > 1 && (row > node->left->value.getRow() || (row == node->left->value.getRow() && column > node->left->value.getColumn())))
	{
		node->left = leftRotate(node->left);
		return rightRotate(node);
	}

	// right left
	if (balance < -1 && (row < node->right->value.getRow() || (row == node->right->value.getRow() && column < node->right->value.getColumn())))
	{
		node->right = rightRotate(node->right);
		return leftRotate(node);
	}

	return node;
}

int Tree::max(int a, int b)
{
	return a > b ? a : b;
}

int Tree::height(Node *node)
{
	if (!node)
		return 0;
	return node->height;
}

Node *Tree::rightRotate(Node *node)
{
	Node *temp = node->left;
	Node *TR = temp->right;

	temp->right = node;
	node->left = TR;

	node->height = max(height(node->left), height(node->right)) + 1;
	temp->height = max(height(temp->left), height(temp->right)) + 1;

	return temp;
}

Node *Tree::leftRotate(Node *node)
{
	Node *temp = node->right;
	Node *TR = temp->left;

	temp->left = node;
	node->right = TR;

	node->height = max(height(node->left), height(node->right)) + 1;
	temp->height = max(height(temp->left), height(temp->right)) + 1;

	return temp;
}

int Tree::getBalance(Node *node)
{
	if (!node)
		return 0;
	return height(node->left) - height(node->right);
}

size_t Tree::getSize() const
{
	return this->size;
}

size_t Tree::getMaxRow() const
{
	return this->max_row;
}

size_t Tree::getMaxColumn() const
{
	return this->max_column;
}

int Tree::count(size_t firstRow, size_t firstColumn, size_t secondRow, size_t secondColumn)
{
	if (secondRow > max_row || secondColumn > max_column || firstRow > secondRow || firstColumn > secondColumn)
	{
		throw std::invalid_argument("Invalid row/column index!");
	}
	int counter = 0;
	checkNonEmpty(this->root, firstRow, firstColumn, secondRow, secondColumn, counter);
	return counter;
}

void Tree::checkNonEmpty(Node *node, size_t firstRow, size_t firstColumn, size_t secondRow, size_t secondColumn, int &count)
{
	if (!node)
		return;
	checkNonEmpty(node->left, firstRow, firstColumn, secondRow, secondColumn, count);
	size_t currentRow = node->value.getRow();
	size_t currentColumn = node->value.getColumn();
	if (currentRow >= firstRow &&
		currentRow <= secondRow &&
		currentColumn >= firstColumn &&
		currentColumn <= secondColumn)
		count++;
	// If we are in cell that is out of this range we do not need to check more ==> return
	else if (currentRow > secondRow && currentColumn > secondColumn)
		return;
	checkNonEmpty(node->right, firstRow, firstColumn, secondRow, secondColumn, count);
}

double Tree::sum(size_t firstRow, size_t firstColumn, size_t secondRow, size_t secondColumn)
{
	if (secondRow > max_row || secondColumn > max_column || firstRow > secondRow || firstColumn > secondColumn)
	{
		throw std::invalid_argument("Invalid row/column index!");
	}
	double total = 0;
	findSum(this->root, firstRow, firstColumn, secondRow, secondColumn, total);
	return total;
}

void Tree::findSum(Node *node, size_t firstRow, size_t firstColumn, size_t secondRow, size_t secondColumn, double &sum)
{
	if (!node)
		return;
	findSum(node->left, firstRow, firstColumn, secondRow, secondColumn, sum);
	size_t currentRow = node->value.getRow();
	size_t currentColumn = node->value.getColumn();
	if (currentRow >= firstRow &&
		currentRow <= secondRow &&
		currentColumn >= firstColumn &&
		currentColumn <= secondColumn)
	{
		std::string cellValue = node->value.getData();
		// If is formula
		if (cellValue[0] == '=')
		{
			sum += toRPN(node->value.getData(), node->value.getRow(), node->value.getColumn());
		}
		// If it is not formula ==> text
		else
			sum += tryToConvert(cellValue);
	}
	// If current cell is out of the given range ==> we dont need to check further
	else if (currentRow > secondRow && currentColumn > secondColumn)
		return;
	findSum(node->right, firstRow, firstColumn, secondRow, secondColumn, sum);
}

double Tree::tryToConvert(std::string &data) const
{
	try
	{
		double d = std::stod(data);
		return d;
	}
	catch (const std::exception &e)
	{
		return 0;
	}
}

void Tree::fixPrecedence(std::queue<std::string> &args, std::stack<std::string> &operators, std::string escapingOperator) const
{
	if (!operators.empty())
	{
		while (!operators.empty() && operators.top() != "(" && (escapingOperator.length() > 0 ? escapingOperator.find(operators.top()) != -1 : true))
		{
			args.push(operators.top());
			operators.pop();
		}
	}
}

Node *Tree::find(Node *node, size_t row, size_t column) const
{
	if (!node)
	{
		throw std::invalid_argument("No item added!");
	}
	else if (node->value.getRow() == row && node->value.getColumn() == column)
	{
		return node;
	}
	else if (node->value.getRow() < row || (node->value.getRow() == row && node->value.getColumn() < column))
	{
		return find(node->right, row, column);
	}
	else if (node->value.getRow() > row || (node->value.getRow() == row && node->value.getColumn() > column))
	{
		return find(node->left, row, column);
	}
	else
		throw std::logic_error("Impossible state!");
}

std::string Tree::charToString(char symbol) const
{
	return std::string(1, symbol);
}

double Tree::toRPN(std::string input, size_t currentRow, size_t currentCol) const
{
	size_t length = input.length(); // size of data
	size_t index = 1;				// start index = 1 (syntax is =...)
	char current;					// current symbol
	bool isPrevNum = false;
	std::queue<std::string> args;	   // all numbers
	std::stack<std::string> operators; // all operators
	bool negative = false;			   // unary '-'
	bool ifCond = false;
	while (index < length)
	{
		current = input[index];
		// If current symbol is digit
		if (current >= '0' && current <= '9')
		{
			std::string temp;
			// Take all digits in current number
			while (input[index] >= '0' && input[index] <= '9')
			{
				temp += input[index];
				++index;
			}
			if (negative)
			{
				temp = '-' + temp;
				negative = false;
			}
			// Add it to number's queue
			args.push(temp);
			isPrevNum = true;
			continue;
		}
		else if (current == 'R')
		{
			size_t nextIndex = index + 1;
			// If it is non-fixed address
			if (input[nextIndex] == '[')
			{
				// Syntax: R[num]C[num]
				// Take number representing row
				size_t nextScope = input.find(']', nextIndex + 1);
				int row = currentRow + stoi(input.substr(nextIndex + 1, nextScope - nextIndex - 1));
				// Take number representing column
				size_t lastScope = input.find(']', nextScope + 3);
				int column = currentCol + stoi(input.substr(nextScope + 3, lastScope - nextScope - 3));
				// If address is invalid (<0) or the cell is same(stops infinity recursion)
				if (row < 0 || column < 0 || (row == currentRow && column == currentCol))
					throw std::invalid_argument("Invalid cell index!\n");
				double value = 0;
				try
				{
					row = static_cast<size_t>(row);
					column = static_cast<size_t>(column);
					std::string nodeData = find(this->root, row, column)->value.getData();
					if (nodeData[0] == '=')
						value = toRPN(nodeData, row, column);
					else
						value = tryToConvert(nodeData);

					if (negative)
					{
						value = value * (-1);
						negative = false;
					}
				}
				catch (const std::exception &)
				{
					value = 0;
				}
				args.push(std::to_string(value));
				index = lastScope;
			}
			// Else it is fixed address
			else
			{
				size_t endIndex = input.find('C', nextIndex);
				// Take number representing row
				int row = stoi(input.substr(nextIndex, endIndex - nextIndex));
				size_t secondEndIndex = endIndex;
				// In fixed address we have last delimiter ']' but here we do not have
				// so take digits while we can
				while (input[secondEndIndex + 1] >= '0' && input[secondEndIndex + 1] <= '9')
				{
					secondEndIndex++;
				}
				// Take number representing column
				int column = stoi(input.substr(endIndex + 1, secondEndIndex - endIndex));
				// If address is invalid (<0) or the cell is same(stops infinity recursion)
				if (row < 0 || column < 0 || (row == currentRow && column == currentCol))
					throw std::invalid_argument("Invalid cell index!\n");
				double value = 0;
				try
				{
					row = static_cast<size_t>(row);
					column = static_cast<size_t>(column);
					std::string nodeData = find(this->root, row, column)->value.getData();
					if (nodeData[0] == '=')
						value = toRPN(nodeData, row, column);
					else
						value = tryToConvert(nodeData);
					if (negative)
					{
						value = value * (-1);
						negative = false;
					}
				}
				catch (const std::exception &)
				{
					value = 0;
				}
				args.push(std::to_string(value));
				index = secondEndIndex;
			}
			isPrevNum = true;
		}
		else if (current == '(')
		{
			operators.push(charToString('('));
			isPrevNum = false;
		}
		else if (current == 'n')
		{
			fixPrecedence(args, operators, "not");
			operators.push(input.substr(index, 3));
			index += 2;
			isPrevNum = false;
		}
		else if (current == '+' || current == '-')
		{
			if (current == '-' && !isPrevNum)
				negative = true;
			else
			{
				fixPrecedence(args, operators, "+-*/not");
				operators.push(charToString(current));
				isPrevNum = false;
			}
		}
		else if (current == '*' || current == '/')
		{
			fixPrecedence(args, operators, "*/not");
			operators.push(charToString(current));
			isPrevNum = false;
		}
		else if (current == '<' || current == '>')
		{
			fixPrecedence(args, operators, "+-*/not<>");
			operators.push(charToString(current));
			isPrevNum = false;
		}
		else if (current == '=' || current == '!')
		{
			fixPrecedence(args, operators, "+-*/!=not==<>");
			operators.push(input.substr(index, 2));
			++index;
			isPrevNum = false;
		}
		else if (current == 'a')
		{
			fixPrecedence(args, operators, "and+-*<>/!=not==");
			operators.push(input.substr(index, 3));
			index += 2;
			isPrevNum = false;
		}
		else if (current == 'o')
		{
			fixPrecedence(args, operators, "and+-*/!=not==or<>");
			operators.push(input.substr(index, 2));
			++index;
			isPrevNum = false;
		}
		else if (current == 'i')
		{
			fixPrecedence(args, operators, "and+if-*/!=not==or<>");
			operators.push(input.substr(index, 2));
			++index;
			ifCond = true;
			isPrevNum = false;
		}
		else if (current == ',')
		{
			fixPrecedence(args, operators, "");
		}
		else if (current == ')')
		{
			std::string first;
			if (!operators.empty())
				first = operators.top();
			bool scope = false;
			if (first == "(")
				scope = true;
			while (!operators.empty() && operators.top() != "(")
			{
				args.push(first);
				operators.pop();
				if (!operators.empty())
				{
					first = operators.top();
					if (first == "(")
						scope = true;
					else
						scope = false;
				}
			}
			if (operators.empty() && !scope)
			{
				throw std::invalid_argument("Mismatched brackets!");
			}
			operators.pop();
			if (ifCond)
			{
				ifCond = false;
				first = operators.top();
				operators.pop();
				args.push(first);
			}
			isPrevNum = false;
		}
		else
		{
			throw std::invalid_argument("Invalid syntax!");
		}
		++index;
	}

	while (!operators.empty())
	{
		if (operators.top() == ")" || operators.top() == "(")
		{
			throw std::invalid_argument("Mismatched brackets!");
		}
		std::string tmp = operators.top();
		operators.pop();
		args.push(tmp);
	}

	double number = ShuntingYardModified(args);
	return number;
}

void Tree::getTopTwoNumber(std::stack<double> result, double &first, double &second) const
{
	first = result.top();
	result.pop();
	second = result.top();
	result.pop();
}

double Tree::ShuntingYardModified(std::queue<std::string> &args) const
{
	std::stack<double> result;
	double first = 0, second = 0;
	while (!args.empty())
	{
		std::string current = args.front();
		if (current == "+")
		{
			getTopTwoNumber(result, first, second);
			result.push(first + second);
		}
		else if (current == "-")
		{
			getTopTwoNumber(result, first, second);
			result.push(second - first);
		}
		else if (current == "*")
		{
			getTopTwoNumber(result, first, second);
			result.push(first * second);
		}
		else if (current == "/")
		{
			getTopTwoNumber(result, first, second);
			if (first == 0)
				throw std::invalid_argument("Divison by zero!\n");
			result.push(second / first);
		}
		else if (current == "<")
		{
			getTopTwoNumber(result, first, second);
			result.push(second < first);
		}
		else if (current == ">")
		{
			getTopTwoNumber(result, first, second);
			result.push(second > first);
		}
		else if (current == "==")
		{
			getTopTwoNumber(result, first, second);
			result.push(first == second);
		}
		else if (current == "!=")
		{
			getTopTwoNumber(result, first, second);
			result.push(first != second);
		}
		else if (current == "and")
		{
			getTopTwoNumber(result, first, second);
			if (result.empty())
				throw std::invalid_argument("Use () for \"if\"\n");
			result.push((first && second));
		}
		else if (current == "or")
		{
			getTopTwoNumber(result, first, second);
			result.push((first || second));
		}
		else if (current == "not")
		{
			first = result.top();
			result.pop();
			result.push(!first);
		}
		else if (current == "if")
		{
			getTopTwoNumber(result, first, second);
			double cond = result.top();
			result.pop();
			result.push((cond ? second : first));
		}
		else
		{
			double num = std::stod(current);
			result.push(num);
		}
		args.pop();
	}
	return result.top();
}

std::string Tree::getNodeExpr(size_t row, size_t column) const
{
	return find(this->root, row, column)->value.getData();
}

double Tree::getNodeValue(size_t row, size_t column) const
{
	std::string nodeData = find(this->root, row, column)->value.getData();
	return nodeData[0] == '=' ? toRPN(nodeData, row, column) : tryToConvert(nodeData);
}

void Tree::addSpaces(std::string &data, size_t count) const
{
	for (size_t i = 0; i < count; ++i)
	{
		data += " ";
	}
}

std::string Tree::print(bool type) const
{
	std::string data;
	size_t MAX_SIZE = max_length > DOUBLE_SIZE ? max_length + 2 : DOUBLE_SIZE;
	std::string temp;
	// Start from 0 to max_row
	for (size_t i = 0; i <= max_row; ++i)
	{
		// And from 0 to max_column
		for (size_t j = 0; j <= max_column; ++j)
		{
			try
			{
				// Try to find cell at this address and take value
				temp = find(this->root, i, j)->value.getData();
				// Type represent VAL or EXPR in print command
				// type = true -> EXPR
				// type = false -> VAL
				if (type)
				{
					data += "\"" + temp + "\"";
					addSpaces(data, MAX_SIZE > temp.length() + 2 ? MAX_SIZE - temp.length() - 2 : 0);
				}
				else
				{
					// If is formula
					if (temp[0] == '=')
					{
						std::string calculated = std::to_string(toRPN(temp, i, j));
						data += ('"' + calculated + '"');
						addSpaces(data, MAX_SIZE > calculated.length() + 2 ? MAX_SIZE - calculated.length() - 2 : 0);
					}
					// Else if it is string
					else
					{
						data += "\"" + temp + "\"";
						addSpaces(data, MAX_SIZE > temp.length() + 2 ? MAX_SIZE - temp.length() - 2 : 0);
					}
				}
			}
			catch (const std::exception &)
			{
				// If cell is not found in tree(table)
				addSpaces(data, MAX_SIZE);
			}
			// Delimiter between columns
			if (j != max_column)
				data += "|";
		}
		data += '\n';
	}
	return data;
}

void Tree::traversal(Node *node, std::string &data, size_t &row, size_t &column) const
{
	if (!node)
		return;
	// Go to the 'smallest' address
	traversal(node->left, data, row, column);
	while (true)
	{
		// If at current row and column exists cell
		if (row == node->value.getRow() && column == node->value.getColumn())
		{
			std::string res;
			if (node->value.getData()[0] == '=')
				res = node->value.getData();
			else
				res = '"' + node->value.getData() + '"';
			data += res;
			// If cell is not in last column add deldimiter
			if (node->value.getColumn() != max_column)
				data += ";";
			// Increase current column
			column++;
			break;
		}
		else if (row <= node->value.getRow() && column <= max_column)
		{
			// Add delimiter if column is not last
			if (column < max_column)
				data += ";";
			// Increase current column
			column++;
		}
		else if (column > max_column)
		{
			// Increase current row
			row++;
			// Start from first column (index 0)
			column = 0;
			// Add new line
			data += '\n';
		}
	}
	// Go to right child
	traversal(node->right, data, row, column);
}

std::string Tree::exportString() const
{
	std::string data;
	size_t row = 0, column = 0;
	traversal(root, data, row, column);
	return data;
}

void Tree::change(size_t row, size_t column, bool increase)
{
	std::string data = find(this->root, row, column)->value.getData();
	if (data[0] == '=') // Formula
	{
		if (increase)
			data += "+1";
		else
			data += "-1";
		find(this->root, row, column)->value.setData(data);
	}
	else // String
	{
		try
		{
			// Try to convert to number
			double value = std::stod(data);
			if (increase)
				++value;
			else
				--value;
			data = std::to_string(value);
			find(this->root, row, column)->value.setData(data);
		}
		catch (const std::exception &e)
		{
			throw std::invalid_argument("Value is not a number");
		}
	}
}