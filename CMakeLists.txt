﻿# CMakeList.txt: projekt CMake dla elementu Sudoku, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#
cmake_minimum_required (VERSION 3.8)

# Włącz przeładowywanie kodu XAML na gorąco dla kompilatorów MSVC, jeśli jest to obsługiwane.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Sudoku")

# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (Sudoku "Sudoku.cpp" "Sudoku.h" "Game/Board.h" "Game/Board.cpp" "Game/SudokuGame.h" "Game/Player/Player.h" "Game/Defaults.h" "Game/Player/Player.cpp" "Game/SudokuGame.cpp"  "Game/Player/PlayerBase.h" "Game/Player/AIPlayer.h" "Game/Player/AIPlayer.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sudoku PROPERTY CXX_STANDARD 20)
endif()

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
