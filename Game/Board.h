#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>

#define number uint8_t


struct Section {
	static constexpr int SECTION_SIZE = 3; //sections in sudoku are 3x3, but this can be changed
	number numbers[SECTION_SIZE][SECTION_SIZE];
};


class Board {
	friend class AIPlayer; //for the ease of use AIPlayer class has access to all Board class fields

	static constexpr number EMPTY_TILE = 0;
	static constexpr char EMPTY_TILE_CHAR = '.';

	int _size; //Board is square of Sections
	Section** _board;
	Section** _filled_board; //filled board is used by AI to give clues to solve sudoku
public:
	Board(const int& size);

	/// <summary> Destructor releases used resources (board in our case) </summary>
	~Board();

	/// <summary> Puts given number in section[col][row] at position [section_y][section_x] </summary>
	/// <returns> <para>true -> if everything went ok</para> false -> if number was wrong, or it was not safe to put it</returns>
	bool PutNumber(const int& row, const int& col, const int& section_x, const int& section_y, const number& num);

	/// <summary> Checks whether whole board is filled </summary>
	/// <returns> <para>true -> if board is filled</para> false -> board is not filled</returns>
	bool CheckWin();

	/// <returns>Board size</returns>
	int GetSize() { return this->_size; }

	/// <summary> Removes number in section[col][row] at position [section_y][section_x] </summary>
	/// <returns> <para>true -> if everything went ok</para> false -> if number tile was empty</returns>
	bool RemoveNumber(const int& row, const int& col, const int& section_x, const int& section_y);

	/// <summary>Saves board to string, which later can be displayed or saved to file</summary>
	std::string ToString();

	/// <summary>Saves board to flat string (1 dimensional) which can be displayer or saved to file</summary>
	std::string ToFlatString();

	/// <summary>Tries to load board from given string, spaces are ignored</summary>
	/// <returns> <para>true -> if everything went ok</para> false -> if string was incorrect (had foribidden symbols/wrong format)</returns>
	bool FromString(const std::string& board);

	/// <summary>Generates random sudoku board</summary>
	bool GenerateRandomBoard(const int& clues_to_leave);

private:

	/// <summary>Fills board with empty tiles</summary>
	void FillEmpty();

	/// <summary> Checks whether number num can be put in section[col][row] at position [section_y][section_x] </summary>
	/// <returns> <para>true -> if it is safe to do so</para> false -> otherwise </returns>
	bool IsSafe(const int& row, const int& col, const int& section_x, const int& section_y, const number& num);

	/// <summary>Fills Section with random numbers in range 1-Section::SECTION_SIZE. Numbers do not repeat</summary>
	void FillSectionRandom(const int& row, const int& col);
	
	/// <summary>Fills diagonal sections with random numbers in range 1-Section::SECTION_SIZE. Numbers do not repeat </summary>
	void FillDiagonalRandom();

	/// <summary>
	/// Recursively fills all remaining tiles in the grid with valid numbers, thus solving the sudoku 
	/// <see href="https://www.geeksforgeeks.org/program-sudoku-generator/">Reference</see>
	/// </summary>
	/// <returns>True -> Board was properly filled, False -> Board is unsolvable</returns>
	bool FillRandomly(const int row, const int col, const int section_x, const int section_y);

	/// <summary>Randomly removes number from board, leaving given amount of numbers. Board has to be filled with valid values</summary>
	/// <param name="clues_to_leave">Amount of numbers to leave, minimum is 17, the more, easier the board will be</param>
	void RemoveRandomly(const int& clues_to_leave);

	/// <summary>Copies _board into _filled_board, use only when _board is filled with proper numbers</summary>
	void CopyBoardToFilled();
};