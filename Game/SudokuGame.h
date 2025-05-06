#pragma once
#include <deque>
#include <fstream>
#include "Player/Player.h"
#include "Player/AIPlayer.h"

class SudokuGame {
private:
	std::deque<Player> player_queue; //we keep player in queue to keep their order
	int current_player = 0;
public:
	/// <summary>Displays main menu</summary>
	void MainMenu();

private:
	void GameLoop();
	void ShowHelp();
	void NewGame();

	/// <summary>Restarts gamestate -> removes all players, current player is set to 0 </summary>
	void RestartGameState();

	/// <summary>Loads gamestate from given file</summary>
	/// <param name="filename">File to load from</param>
	/// <returns><para>true -> if loading was successfull</para>false -> otherwise</returns>
	bool LoadGame(const std::string& filename);

	/// <summary>Saves gamestate into given file
	/// <para>File structure -></para>
	/// <para>Line : number of players (integer)</para>
	/// <para>For each player:</para>
	/// <para>    Line: player name (string)</para>
	/// <para>    Line: board data as a single-line string (e.g., serialized 2D grid)</para>
	/// </summary>
	/// <param name="filename">File to save to</param>
	///<returns><para>true -> if saving was successfull</para>false -> otherwise</returns>
	bool SaveGame(const std::string& filename);

	/// <summary>Shows prompt to get movement info and saves it into variables</summary>
	/// <returns><para>true -> if input was ok</para>false -> otherwise</returns>
	bool GetMovementInfo(Board& board, int& row, int& col, int& section_x, int& section_y, number& num);

	/// <summary>Shows prompt to get info for tile removal and saves it into variables</summary>
	/// <returns><para>true -> if input was ok</para>false -> otherwise</returns>
	bool GetRemoveInfo(Board& board, int& row, int& col, int& section_x, int& section_y);

	/// <summary>Makes random move by AI</summary>
	void AIMove();

	void ClearScreen();
	void PressEnter();
};