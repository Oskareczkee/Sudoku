#pragma once
#include "PlayerBase.h"

class AIPlayer : public PlayerBase {
public:
	AIPlayer(const std::string& name, std::shared_ptr<Board> board_ref) : PlayerBase(name) {
		this->board = board_ref;
	}

	/// <summary>Places given random number in random proper section on the board. Ensures that move is possible</summary>
	/// <returns><para>true -> if everything went ok</para> false -> if move was not possible to make</returns>
	bool MakeMove();
};