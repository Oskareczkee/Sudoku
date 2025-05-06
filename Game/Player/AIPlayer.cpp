#pragma once
#include "AIPlayer.h"

bool AIPlayer::MakeMove()
{
	std::vector<std::tuple<int, int, int, int>> unfilled_tiles;

	for (int x = 0; x < board->_size; x++)
		for (int y = 0; y < board->_size; y++) //for each section
			for (int xs = 0; xs < Section::SECTION_SIZE; xs++)
				for (int ys = 0; ys < Section::SECTION_SIZE; ys++) //for each col,row in section
					if (this->board->_board[x][y].numbers[ys][xs] == Board::EMPTY_TILE) //board is not filled, empty tile was found
						unfilled_tiles.emplace_back(x, y, xs, ys);

	std::shuffle(unfilled_tiles.begin(), unfilled_tiles.end(), std::mt19937{ std::random_device{}() });
	const auto& [row, col, section_x, section_y] = unfilled_tiles[0];
	const number& num = board->_filled_board[row][col].numbers[section_y][section_x];
	return this->board->PutNumber(row, col, section_x, section_y, num);
}
