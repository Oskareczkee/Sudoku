#include <string>
#include "Board.h"
#include "Defaults.h"

class Player {
private:
	std::string name = "";
	Board* board = nullptr;
public:

	Player(const std::string& name, const int& board_size) : name(name), board(new Board(board_size)){}
	Player(const std::string& name): name(name), board(new Board(Defaults::DEFAULT_BOARD_SIZE)){}
	~Player() { delete board; }

	std::string GetName() { return this->name; }
	void SetName(const std::string& name) { this->name = name; }

	/// <summary>Places given number num in given section (row,col) on given coordinates </summary>
	/// <returns><para>true -> if everything went ok</para> false -> if move was not possible to make</returns>
	bool MakeMove(const int& row, const int& col, const int& section_x, const int& section_y, const number& num);

	/// <returns>Player board reference (original)</returns>
	Board& GetBoardRef() { return *board; }
};