#ifndef TREE_H
#define TREE_H

#include "Cell.h"
#include <queue>
#include <stack>

struct Node
{
    Node* left = nullptr;
    Node* right = nullptr;
    Cell value;
    int height = 1;
};

class Tree
{
private:
    size_t max_row; // Help variable for print function
    size_t max_column; // Help variable for print function
    size_t size; // Total count of cells (nodes)
    size_t max_length; // Max length of cell's value
    Node* root; // Start cell (node)

    //Help function for destructor
    void clear(Node*& root);
    //Help functions for balancing the tree
    int max(int a, int b);
    int height(Node* node);
    Node* rightRotate(Node* node);
    Node* leftRotate(Node* node);
    int getBalance(Node* node);
    //Help function for insertion
    Node* insertNode(Node*& node, size_t row, size_t column, std::string data);
    //Help function for counting non-empty cells in given range
    void checkNonEmpty(Node* node, size_t firstRow, size_t firstColumn, size_t secondRow, size_t secondColumn,int& count);
    //Help function for calculating sum of all cells in given range
    void findSum(Node* node,size_t firstRow,size_t firstColumn,size_t secondRow, size_t secondColumn,double& sum);
    //Try to convert cell's value to number
    double tryToConvert(std::string& data) const;
    //Function to fix order of operators
    void fixPrecedence(std::queue<std::string> &args, std::stack<std::string> &operators, std::string escapingOperator) const;
    //Returns node by given row and column
    Node* find(Node* node, size_t row, size_t column) const;
    //Convert single char to string
    std::string charToString(char symbol) const;
    //Convert given formula to expression in reverse polish notation
    double toRPN(std::string input, size_t currentRow, size_t currentCol) const;
    //Help function for shunting yard algorithm
    void getTopTwoNumber(std::stack<double> result,double& first,double& second) const;
    //Calculate value from given args
    double ShuntingYardModified(std::queue<std::string>& args) const;
    //Help function for formatting print string
    void addSpaces(std::string& data, size_t count) const;
    //Help function for saving file
    void traversal(Node* node, std::string& data,size_t& row,size_t & column) const;
public:
    //Constructor
    Tree();
    //Destructor
    ~Tree();
    //Insert new cell (node) in tree
    void insert(size_t row, size_t column, std::string data);
    //Returns count of cells (nodes)
    size_t getSize() const;
    //Returns max row of table
    size_t getMaxRow() const;
    //Returns max column of table
    size_t getMaxColumn() const;
    //Returns total non-empty cells in given range
    int count(size_t firstRow, size_t firstColumn, size_t secondRow, size_t secondColumn);
    //Returns total sum of cells in given range
    double sum(size_t firstRow, size_t firstColumn, size_t secondRow, size_t secondColumn);
    //Returns cell's expression
    std::string getNodeExpr(size_t row, size_t column) const;
    //Returns cell's value
    double getNodeValue(size_t  row, size_t column) const;
    //Print function(type = true -> print expr| type = fakse -> print value)
    std::string print(bool type) const;
    //Returns all table cells saved in string ('save' command helper)
    std::string exportString() const;
    //Function that incremenets/decerements cell's value
    void change(size_t row,size_t column, bool increase);
};

#endif