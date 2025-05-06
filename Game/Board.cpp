#include "Board.h"

Board::Board(const int& size) : _size(size)
{
	//initialize board
	this->_board = new Section * [size];
	this->_filled_board = new Section * [size];
	for (int x = 0; x < size; x++) {
		this->_board[x] = new Section[size];
		this->_filled_board[x] = new Section[size];
	}

	FillEmpty(); //fill with empty values
}

Board::~Board()
{
	for (int x = 0; x < _size; x++) {
		delete[] _board[x];
		delete[] _filled_board[x];
	}

	delete[] _board;
	delete[] _filled_board;
}

bool Board::PutNumber(const int& row, const int& col, const int& section_x, const int& section_y, const number& num)
{
	if (num <= 0 || num > Section::SECTION_SIZE * Section::SECTION_SIZE) //number was <= 0 or number is greater than possible max number in section
		return false;

	if (IsSafe(row, col, section_x, section_y, num)) {
		this->_board[row][col].numbers[section_y][section_x] = num;
		return true;
	}

	return false;
}

bool Board::CheckWin()
{
	for (int x = 0; x < _size; x++)
		for (int y = 0; y < _size; y++) //for each section
			for (int xs = 0; xs < Section::SECTION_SIZE; xs++)
				for (int ys = 0; ys < Section::SECTION_SIZE; ys++) //for each col,row in section
					if (this->_board[x][y].numbers[xs][ys] == EMPTY_TILE) //board is not filled, empty tile was found
						return false;

	return true;
}

bool Board::RemoveNumber(const int& row, const int& col, const int& section_x, const int& section_y)
{
	if (this->_board[row][col].numbers[section_y][section_x] == EMPTY_TILE) //tile is empty
		return false;

	this->_board[row][col].numbers[section_y][section_x] = EMPTY_TILE;
	return true;
}

std::string Board::ToString()
{
	/*TODO: if section would be greater than 3x3 padding is not added, only single digit numbers are supported*/

	std::string out;
	const int GLOBAL_SIZE = _size * Section::SECTION_SIZE;       

	for (int global_row = 0; global_row < GLOBAL_SIZE; global_row++)
	{
		//horizontal padding
		if (global_row > 0 && global_row % Section::SECTION_SIZE == 0)
			out += "\n";

		for (int global_col = 0; global_col < GLOBAL_SIZE; global_col++) //treat board as 2 dimensional instead of having sections
		{
			// vertical padding
			if (global_col > 0 && global_col % Section::SECTION_SIZE == 0)
				out += "  "; // 2 spaces

			//calculate section coordinates, and in_section coordinates
			int section_row = global_row / Section::SECTION_SIZE;
			int in_sec_row = global_row % Section::SECTION_SIZE;

			int section_col = global_col / Section::SECTION_SIZE;
			int in_sec_col = global_col % Section::SECTION_SIZE;

			int val = _board[section_row][section_col].numbers[in_sec_row][in_sec_col];

			if (val == EMPTY_TILE) {
				out += EMPTY_TILE_CHAR;
				out += " ";
			}
			else
				out += std::to_string(val) + " ";
		}
		out += "\n";
	}

	return out;
}

std::string Board::ToFlatString()
{
	std::string out;

	for (int x = 0; x < _size; x++)
		for (int y = 0; y < _size; y++)
			for (int sx = 0; sx<Section::SECTION_SIZE; sx++)
				for (int sy = 0; sy < Section::SECTION_SIZE; sy++) {
					if (_board[x][y].numbers[sx][sy] == EMPTY_TILE)
						out += EMPTY_TILE_CHAR;
					else
						out += _board[x][y].numbers[sx][sy] + '0';
				}

	return out;
}

bool Board::FromString(const std::string& board)
{
	const int totalTiles = _size * _size * Section::SECTION_SIZE * Section::SECTION_SIZE;

	if (board.length() != totalTiles) {
		std::cerr << "Invalid board string length: expected " << totalTiles << ", got " << board.length() << "\n";
		return false;
	}

	int index = 0;

	for (int sectionRow = 0; sectionRow < _size; sectionRow++) {
		for (int sectionCol = 0; sectionCol < _size; sectionCol++) {
			for (int tileRow = 0; tileRow < Section::SECTION_SIZE; tileRow++) {
				for (int tileCol = 0; tileCol < Section::SECTION_SIZE; tileCol++) {

					while (std::isspace(board[index])) //skip whitespaces
						index++;

					char ch = board[index++];
					if (ch == EMPTY_TILE_CHAR) { //empty tile
						_board[sectionRow][sectionCol].numbers[tileRow][tileCol] = EMPTY_TILE;
						continue;
					}

					if (ch < '0' || ch > '9') {
						std::cerr << "Invalid character in board string: '" << ch << "'\n";
						return false;
					}

					int value = ch - '0'; //'-0' converts character into digit
					_board[sectionRow][sectionCol].numbers[tileRow][tileCol] = value;
				}
			}
		}
	}

	return true;
}

bool Board::GenerateRandomBoard(const int& clues_to_leave)
{
	if (clues_to_leave < 17 || //there are no sudoku boards that have less than 17 clues and are solvable
		clues_to_leave >= _size * _size * Section::SECTION_SIZE * Section::SECTION_SIZE) //number of clues is greater than plane size
		return false;

	FillDiagonalRandom();
	FillRandomly(0, 0, 0, 0);
	CopyBoardToFilled();//copy filled _board into _filled_board for further use
	RemoveRandomly(clues_to_leave);

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

	//check if number is already in given section
	for (int x = 0; x < _size; x++)
		for (int y = 0; y < _size; y++)
			if (_board[row][col].numbers[x][y] == num)
				return false;

	return true;
}

void Board::FillSectionRandom(const int& row, const int& col)
{
	std::vector<number> available_numbers(Section::SECTION_SIZE * Section::SECTION_SIZE);
	std::iota(available_numbers.begin(), available_numbers.end(), 1);
	std::shuffle(available_numbers.begin(), available_numbers.end(), std::mt19937{std::random_device{}()}); //shuffle the numbers randomly

	auto iter = available_numbers.begin();

	//fill section with random numbers in range
	for (int x = 0; x < Section::SECTION_SIZE; x++)
		for (int y = 0; y < Section::SECTION_SIZE; y++)
			_board[row][col].numbers[x][y] = *iter++;
}

void Board::FillDiagonalRandom()
{
	for (int x = 0; x < _size; x++)
		FillSectionRandom(x, x);
}

bool Board::FillRandomly(const int row, const int col, const int section_x, const int section_y)
{
	//check if we are at the end of the board
	if (row == _size - 1 && col == _size - 1 && section_x == Section::SECTION_SIZE - 1 && section_y == Section::SECTION_SIZE - 1)
		return true; //end of recursion

	//order is important, do not change it!
	if (col == _size)
		return FillRandomly(row + 1, 0, 0, 0);

	if (section_x == Section::SECTION_SIZE - 1 && section_y == Section::SECTION_SIZE)
		return FillRandomly(row, col + 1, 0, 0);

	if (section_x == Section::SECTION_SIZE)
		return FillRandomly(row, col, 0, section_y + 1);

	if (section_y == Section::SECTION_SIZE)
		return FillRandomly(row, col + 1, 0, 0);

	if (_board[row][col].numbers[section_y][section_x] != EMPTY_TILE)
		return FillRandomly(row, col, section_x + 1, section_y);

	for (int num = 1; num <= Section::SECTION_SIZE * Section::SECTION_SIZE; num++) {

		if (PutNumber(row, col, section_x, section_y,num)) {
			if (FillRandomly(row, col, section_x + 1, section_y))
				return true;
			else
				_board[row][col].numbers[section_y][section_x] = EMPTY_TILE;
		}
	}

	return false;
}

void Board::RemoveRandomly(const int& clues_to_leave)
{
	std::vector<std::tuple<int, int, int, int>> all_positions;

	for (int x = 0; x < _size; x++)
		for (int y = 0; y < _size; y++)
			for (int xs = 0; xs < Section::SECTION_SIZE; xs++)
				for (int ys = 0; ys < Section::SECTION_SIZE; ys++)
					all_positions.emplace_back(x, y, xs, ys);

	std::shuffle(all_positions.begin(), all_positions.end(),std::mt19937{ std::random_device{}() });
	
	for (int x = 0; x < all_positions.size() - clues_to_leave; x++) {
		const auto&[row,col,section_x, section_y] = all_positions[x];
		this->RemoveNumber(row, col, section_x, section_y);
	}
}

void Board::CopyBoardToFilled()
{
	for (int row = 0; row < _size; row++)
		for (int col = 0; col < _size; col++)
			_filled_board[row][col] = _board[row][col];
}

void Board::FillEmpty()
{
	for (int x = 0; x < _size; x++)
		for (int y = 0; y < _size; y++) //for each section
			for (int xs = 0; xs < Section::SECTION_SIZE; xs++)
				for (int ys = 0; ys < Section::SECTION_SIZE; ys++) { //for each col,row in section
					this->_board[x][y].numbers[xs][ys] = EMPTY_TILE;
					this->_filled_board[x][y].numbers[xs][ys] = EMPTY_TILE;
				}
}
