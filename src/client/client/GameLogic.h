/**
 * @file GameLogic.h
 * @brief Contains functions for game logic.
 */

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

/**
 * @brief Parses the game state from an XML file.
 *
 * This function loads an XML file containing the game state and parses the relevant details,
 * including the first player, the game mode, the current board state, and the game status.
 *
 * @param filename The path to the XML file to load.
 * @param firstPlayer The player who will make the first move ('X' or 'O').
 * @param gameMode The type of game being played (e.g., "Man vs Man", "AI vs Man").
 * @param board The 3x3 game board represented as a 2D vector of characters ('X', 'O', '_').
 * @param gameStatus The current status of the game (e.g., "Start", "Game Over").
 */
void parseGameStateXML(const string& filename, char& firstPlayer, string& gameMode, vector<vector<char>>& board, string& gameStatus);

/**
 * @brief Updates the XML file with the current game state.
 *
 * This function writes the updated game state (current player, game type, board) to an XML file.
 *
 * @param player The current player ('X' or 'O').
 * @param gameType The type of game being played (e.g., "Man vs Man", "AI vs Man").
 * @param board The 3x3 game board represented as a 2D vector of characters ('X', 'O', '_').
 */
void updateXML(const string& player, const string& gameType, const vector<vector<char>>& board);

/**
 * @brief Creates a new XML file with the initial game state.
 *
 * This function creates an XML file containing the initial game state with player settings,
 * game type, and an empty 3x3 game board.
 *
 * @param filename The path to the XML file to create.
 * @param firstPlayer The player who will make the first move ('X' or 'O').
 * @param gameMode The type of game being played (e.g., "Man vs Man", "AI vs Man").
 * @param board The 3x3 game board represented as a 2D vector of characters ('X', 'O', '_').
 */
void createGameStateXML(const string& filename, char firstPlayer, const string& gameMode, const vector<vector<char>>& board);

/**
 * @brief Reads the contents of a file into a string.
 *
 * This function reads the entire content of the given file and returns it as a string.
 *
 * @param fileName The path to the file to read.
 * @return The contents of the file as a string.
 */
string readFileToString(const string& fileName);

/**
 * @brief Prompts the user to select which player goes first.
 *
 * This function prompts the user to choose whether 'X' or 'O' goes first, or it randomly selects a player.
 *
 * @return The character representing the first player ('X' or 'O').
 */
char selectFirstPlayer();

/**
 * @brief Prompts the user to select the game mode.
 *
 * This function allows the user to choose the game mode (e.g., "Man vs Man", "AI vs Man").
 *
 * @return The selected game mode as a string.
 */
string selectGameMode();

/**
 * @brief Makes a move on the Tic-Tac-Toe board.
 *
 * This function updates the game board with the specified player's move. It places the player's symbol
 * ('X' or 'O') on the board at the position determined by the move number (1-9). If the cell is already
 * occupied, the function returns `false`. Otherwise, it places the symbol and returns `true`.
 *
 * @param board A reference to a 2D vector representing the game board.
 * @param move The move number (1-9) representing the position on the board where the player wants to place their symbol.
 * @param player The symbol of the player ('X' or 'O') making the move.
 * @return true if the move was successful (the cell was empty and the symbol was placed), false otherwise (if the cell was already occupied).
 */
bool makeMove(vector<vector<char>>& board, int move, char player);

/**
 * @brief Prints the current state of the Tic-Tac-Toe board.
 *
 * This function outputs the current state of the game board to the console. It displays the 3x3 grid
 * with each cell containing either an 'X', 'O', or an empty space ('_'). The board is visually formatted
 * with lines separating the rows and columns for easier readability. The cells are colorized with green text.
 *
 * @param board A constant reference to a 2D vector representing the Tic-Tac-Toe game board.
 */
void printBoard(const vector<vector<char>>& board);