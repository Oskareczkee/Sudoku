#pragma once

#include <string>
#include "../Board.h"
#include "../Defaults.h"

class PlayerBase {

protected:
	std::string name = "";
	std::shared_ptr<Board> board = nullptr; //shared ptr is deleted only after pointer is not used by anything
public:
	PlayerBase(const std::string& name, const int& board_size) : name(name), board(new Board(board_size)) {}
	PlayerBase(const std::string& name) : name(name), board(new Board(Defaults::DEFAULT_BOARD_SIZE)) {}

	std::string GetName() { return this->name; }
	virtual void SetName(const std::string& name) { this->name = name; }

	/// <returns>Player board reference (original)</returns>
	virtual Board& GetBoardRef() { return *board; }
	virtual std::shared_ptr<Board> GetBoardShared() { return board; }
};