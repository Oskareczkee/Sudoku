#pragma once
#include "SudokuGame.h"

void SudokuGame::MainMenu()
{
    int choice = 0;
    bool running = true;

    while (running)
    {
        std::cout << "\n====== Sudoku Game ======\n";
        std::cout << "1. Start New Game\n";
        std::cout << "2. Load Game\n";
        std::cout << "3. Show Instructions\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";

        std::cin >> choice;

        //Input was invalid
        if (!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//clear input stream
            std::cout << "Invalid input. Please enter a number between 1 and 4.\n";
            PressEnter();
            ClearScreen();
            continue;
        }

        switch (choice)
        {
        case 1:
            std::cout << "\nStarting new game...\n";
            this->NewGame();
            break;

        case 2:
            std::cout << "\nLoading saved game...\n";
            this->LoadGame("test_save.save");
            break;

        case 3:
            this->ShowHelp();
            break;

        case 4:
            std::cout << "\nExiting game. Goodbye!\n";
            running = false;
            break;

        default:
            std::cout << "Invalid choice. Please select a number between 1 and 4.\n";
            break;
        }
    }
}

void SudokuGame::ShowHelp()
{
   ClearScreen();
   std::cout << "\nInstructions:\n";
   std::cout << "- Fill each row, column, and 3x3 section with numbers 1 to 9.\n";
   std::cout << "- No duplicates allowed in any row, column, or section.\n";
   std::cout << "- Good luck!\n";

   PressEnter();
   ClearScreen();
}

void SudokuGame::NewGame()
{
    ClearScreen();

    int players = 0;
    std::cout << "\n=== Start New Game ===\n";

    while (true) //Player input loop
    {
        std::cout << "Enter number of players: ";
        std::cin >> players;

        if (!std::cin || players <= 0 || players > Defaults::MAX_PLAYERS) //input is not valid, or player count is not valid 
        {
            std::cin.clear(); // Clear input error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a positive number.\n";
            PressEnter();
            ClearScreen();
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//Clear remaining input
            break;
        }
    }

    for (int x = 0; x < players; x++) {
        std::string name = "";
        std::cout << "Input name for player " << x + 1 << ":\n";
        std::cin >> name;

        auto& p = player_queue.emplace_front(name); //emplace calls constructor directly in container
        if (!p.GetBoardRef().GenerateRandomBoard(Defaults::DEFAULT_CLUES))//generate random board for this player
        {
            std::cerr << "[Critical] -> Board could not be generated for player: " << name << "\n";
            PressEnter();
            exit(0);
        }
    }

    this->GameLoop();
}

void SudokuGame::RestartGameState()
{
    while (!this->player_queue.empty())
        player_queue.pop_back();

    this->current_player = 0;
}

bool SudokuGame::LoadGame(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "File could not be opened (Maybe it does not exist?)";
        return false;
    }

    RestartGameState();

    int player_count = 0;
    file >> player_count;

    for (int x = 0; x < player_count; x++)
    {
        std::string name;
        std::string board;

        file >> name;
        file >> board;

        Player& player = player_queue.emplace_front(name);
        if (!player.GetBoardRef().FromString(board)) //load board from string
            return false; //board was in wrong format
    }
    return true;
}

void SudokuGame::GameLoop()
{
    const int totalPlayers = player_queue.size();
    bool switch_player = false;

    while(true){

        //get references, so copies won't be made
        Player& player = player_queue.at(this->current_player);
        Board& board = player.GetBoardRef();

        ClearScreen();
        std::cout << "\n=== Player " << player.GetName() << "'s Turn ===\n";
        std::cout<<board.ToString()<<"\n"; // assuming `board` is the current game board

        std::cout << "Choose action:\n";
        std::cout << "1. Make a move\n";
        std::cout << "2. Remove a number\n";
        std::cout << "3. AI random move\n";
        std::cout << "4. Save game\n";
        std::cout << "5. Load game\n";
        std::cout << "6. Quit\n";
        std::cout << "Enter option: ";

        int option;
        std::cin >> option;

        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input.\n";
            continue;
        }

        int row, col, section_x, section_y;
        number num;

        switch (option)
        {
        case 1: { //Make a move
            while (!GetMovementInfo(board,row, col, section_x, section_y, num)) { //if prompt was invalid, press enter and show it again
                PressEnter();
                ClearScreen();
            }

            if (board.PutNumber(row - 1, col - 1, section_x - 1, section_y - 1, num)) //sections and tiles are 1-indexed
                switch_player = true;
            else {
                std::cout << "Invalid move. Try again\n";
                PressEnter();
            }
            break;
        }

        case 2: { // Remove a number
            while (!GetRemoveInfo(board, row, col, section_x, section_y)) { //if prompt was invalid, press enter and show it again
                PressEnter();
                ClearScreen();
            }

            if (board.RemoveNumber(row - 1, col - 1, section_x - 1, section_y - 1)) //sections and tiles are 1-indexed
                switch_player = true;
            else
                std::cout << "Number could not be removed. Try again\n";
            break;
        }

        case 3: { //AI random move
            AIMove();
            std::cout << "Move was successfully done!\n";
            PressEnter();
            switch_player = true;
            break;
        }

        case 4: {
            // Save game
            if (SaveGame("test_save.save")) {
                std::cout << "Game saved successfully.\n";
                PressEnter();
            }
            else {
                std::cout << "Failed to save game.\n";
                PressEnter();
            }

            break;
        }

        case 5: // Load game
            if (LoadGame("test_save.save")) {
                std::cout << "Game loaded successfully.\n";
                PressEnter();
            }
            else {
                std::cout << "Failed to load game.\n";
                PressEnter();
            }
            break;

        case 6: {
            std::cout << "Exiting game...\n";
            PressEnter();
            exit(0);
        }

        default:
            std::cout << "Invalid option. Try again.\n";
            break;
        }

        if (player.GetBoardRef().CheckWin()) {
            std::cout << "Congratulations " << player.GetName() << "\nYou have won!\n";
            PressEnter();
            break; //out of the loop
        }


        if(switch_player)
            this->current_player = (this->current_player + 1) % totalPlayers;
    }
}

bool SudokuGame::SaveGame(const std::string& filename)
{
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file for saving: " << filename << "\n";
        return false;
    }

    file << this->player_queue.size() << "\n";

    for (auto& player : player_queue) {

        file << player.GetName() << "\n";
        file << player.GetBoardRef().ToFlatString() << "\n";
    }

    return true;
}

bool SudokuGame::GetMovementInfo(Board& board, int& row, int& col, int& section_x, int& section_y, number& num)
{
    std::cout << "Enter section row (1-" << board.GetSize() << "): ";
    std::cin >> row;

    std::cout << "Enter section col (1-" << board.GetSize() << "): ";
    std::cin >> col;

    std::cout << "Enter tile row in section (1-3): ";
    std::cin >> section_y;

    std::cout << "Enter tile col in section (1-3): ";
    std::cin >> section_x;

    std::cout << "Enter number to place (1-9): ";
    std::cin >> num;

    num = num - '0';//uint8_t is loaded as character, we have to convert it to the digit

    if (num < 1 || num > 9)
        return false;
    else if (row < 1 || row > board.GetSize() ||
        col < 1 || col > board.GetSize() ||
        section_y < 1 || section_y > Section::SECTION_SIZE ||
        section_x < 1 || section_x > Section::SECTION_SIZE)
        return false;

    return true;
}

bool SudokuGame::GetRemoveInfo(Board& board, int& row, int& col, int& section_x, int& section_y)
{
    std::cout << "Enter section row (1-" << board.GetSize() << "): ";
    std::cin >> row;
    std::cout << "Enter section col (1-" << board.GetSize() << "): ";
    std::cin >> col;
    std::cout << "Enter tile row in section (1-3): ";
    std::cin >> section_y;
    std::cout << "Enter tile col in section (1-3): ";
    std::cin >> section_x;
    
    if (row < 1 || row > board.GetSize() ||
        col < 1 || col > board.GetSize() ||
        section_y < 1 || section_y > Section::SECTION_SIZE ||
        section_x < 1 || section_x > Section::SECTION_SIZE)
        return false;

    return true;
}

void SudokuGame::AIMove()
{
    Player& player = player_queue[current_player];
    AIPlayer ai(player.GetName() + "_AI", player.GetBoardShared());

    ai.MakeMove();
}

void SudokuGame::ClearScreen()
{
    //platform independent
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif // _WIN32
}

void SudokuGame::PressEnter()
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //ignore previous input
    std::cin.get();
}
