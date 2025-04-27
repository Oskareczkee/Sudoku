#include "Board.h"

Board::Board(const int& size) : _size(size)
{
	//initialize board
	this->_board = new Section * [size];
	for (int x = 0; x < size; x++)
		this->_board[x] = new Section[size];

	FillEmpty(); //fill with empty values
}

Board::~Board()
{
	for (int x = 0; x < _size; x++)
		delete[] _board[x];

	delete[] _board;
}

bool Board::PutNumber(const int& row, const int& col, const int& section_x, const int& section_y, const number& num)
{
	if (num <= 0 || num > Section::SECTION_SIZE * Section::SECTION_SIZE) //number was <= 0 or number is greater than possible max number in section
		return false;

	if (IsSafe(row, col, section_x, section_y, num)) {
		this->_board[row][col].numbers[section_x][section_y] = num;
		return true;
	}

	return false;
}

bool Board::RemoveNumber(const int& row, const int& col, const int& section_x, const int& section_y)
{
	if (this->_board[row][col].numbers[section_x][section_y] == EMPTY_TILE) //tile is empty
		return false;

	this->_board[row][col].numbers[section_x][section_y] = EMPTY_TILE;
	return true;
}

bool Board::IsSafe(const int& row, const int& col, const int& section_x, const int& section_y, const number& num)
{
	//EDGE CASE: there is already number
	if (_board[row][col].numbers[section_y][section_x] != EMPTY_TILE)
		return false;

	//rows
	for (int x = 0; x < _size; x++) //for each section in given row
	{
		for (int y = 0; y < Section::SECTION_SIZE; y++) //for each row in section
			if (_board[row][x].numbers[section_y][y] == num)
				return false;
	}

	//cols
	for (int x = 0; x < _size; x++) //for each section in given column
	{
		for (int y = 0; y < Section::SECTION_SIZE; y++) //for each column in section
			if (_board[x][col].numbers[y][section_x] == num)
				return false;
	}

	return true;
}

void Board::FillEmpty()
{
	for (int x = 0; x < _size; x++)
		for (int y = 0; y < _size; y++) //for each section
			for (int xs = 0; xs < Section::SECTION_SIZE; xs++)
				for (int ys = 0; ys < Section::SECTION_SIZE; ys++) //for each col,row in section
					this->_board[x][y].numbers[xs][ys] = EMPTY_TILE;
}
