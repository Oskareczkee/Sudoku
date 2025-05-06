#pragma once

#include <string>
#include "PlayerBase.h"

class Player : public PlayerBase {
public:

	Player(const std::string& name, const int& board_size) : PlayerBase(name,board_size){}
	Player(const std::string& name): PlayerBase(name){}

	/// <summary>Places given number num in given section (row,col) on given coordinates </summary>
	/// <returns><para>true -> if everything went ok</para> false -> if move was not possible to make</returns>
	bool MakeMove(const int& row, const int& col, const int& section_x, const int& section_y, const number& num);
};