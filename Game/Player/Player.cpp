#pragma once

#include "Player.h"

bool Player::MakeMove(const int& row, const int& col, const int& section_x, const int& section_y, const number& num)
{
	return board->PutNumber(row, col, section_x, section_y, num);
}
