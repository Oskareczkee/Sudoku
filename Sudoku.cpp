// Sudoku.cpp: definiuje punkt wejścia dla aplikacji.
//

#include "Sudoku.h"

using namespace std;

int main()
{
	Board board = Board(3);

	std::cout << board.ToString();
}
