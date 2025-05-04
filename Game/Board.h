#include <iostream>
#include <string>
#define number uint8_t


struct Section {
	static constexpr int SECTION_SIZE = 3; //sections in sudoku are 3x3, but this can be changed
	number numbers[SECTION_SIZE][SECTION_SIZE];
};


class Board {
	static constexpr number EMPTY_TILE = 0;

	int _size; //Board is square of Sections
	Section** _board;
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

	/// <summary>Tries to load board from given stream (file, input, stringstream, etc...), spaces are ignored</summary>
	/// <returns> <para>true -> if everything went ok</para> false -> if string was incorrect (had foribidden symbols/wrong format)</returns>
	bool FromStream(std::istream& stream);


private:

	/// <summary>Fills board with empty tiles</summary>
	void FillEmpty();

	/// <summary> Checks whether number num can be put in section[col][row] at position [section_y][section_x] </summary>
	/// <returns> <para>true -> if it is safe to do so</para> false -> otherwise </returns>
	bool IsSafe(const int& row, const int& col, const int& section_x, const int& section_y, const number& num);

};