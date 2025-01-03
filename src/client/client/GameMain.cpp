/**
 * @file main.cpp
 * @brief Main program for the "Tic-Tac-Toe" game with serial communication and game modes.
 *
 * This program initializes the game, allows the player to choose game mode,
 * serial port settings, and interact with the game. It supports multiple game modes
 * including "Man vs Man", "Man vs AI", "AI vs Man", and "AI vs AI".
 */

#include <iostream>
#include <string>
#include "Windows.h"
#include "SerialPort.h"
#include "GameLogic.h"

 /**
  * @brief Main function that runs the Tic-Tac-Toe game.
  *
  * Based on the selected game mode, the function controls the flow of the game.
  * It interacts with the player, makes moves, updates the game board, communicates
  * with the serial port, and displays the results.
  *
  * @return int Exit code.
  */

int main() {
	cout << "\n\n=============================================\n\n";
	cout << "      Welcome to \"Tic - tac - toe\" \n";
	cout << "\n=============================================\n";

	wstring portName = selectPort();
	DWORD baudRate = selectBaudRate();
	cout << "\033[2J\033[H";

	char firstPlayer = selectFirstPlayer();
	string gameMode = selectGameMode();

	cout << "\033[2J\033[H";
	cout << "\n=============================================\n";
	cout << "      " << firstPlayer << " goes first\n";
	cout << "=============================================\n";
	cout << "\n=============================================\n";
	cout << "      Selected Game Mode: " << gameMode << "\n";
	cout << "=============================================\n";

	vector<vector<char>> board = {
		{'_', '_', '_'},
		{'_', '_', '_'},
		{'_', '_', '_'}
	};
	string filename = "game_state.xml";
	createGameStateXML(filename, firstPlayer, gameMode, board);

	if (gameMode == "Man vs Man") {
		string gameStatus = "Start";
		cout << "\n=============================================\n";
		cout << "      Hints for selecting cells";
		vector<vector<char>> boardAbout = {
			{'1', '2', '3'},
			{'4', '5', '6'},
			{'7', '8', '9'}
		};
		printBoard(boardAbout);
		cout << "      Board:";
		printBoard(board);
		while (gameStatus != "Win X" && gameStatus != "Win O" && gameStatus != "Draw") {
			HANDLE comPort = openSerialPort(portName, baudRate);
			if (comPort == INVALID_HANDLE_VALUE) {
				cout << "\033[2J\033[H";
				cout << "=============================================\n";
				cerr << "\n\033[31m      Error opening com-port! \033[0m" << endl;
				cout << "\n=============================================\n";
				return 1;
			}
			int move;
			cout << "      Please, " << firstPlayer << " enter your move(1 - 9) : ";
			cin >> move;
			cout << "=============================================\n";
			if (!makeMove(board, move, firstPlayer)) {
				cout << "\033[31m      Cell already occupied! \033[0m" << endl;
				cout << "=============================================\n";
				CloseHandle(comPort);
				continue;

			}
			cout << "\033[2J\033[H";
			cout << "=============================================\n";
			cout << "      Board:";
			printBoard(board);
			updateXML(std::string(1, firstPlayer), gameMode, board);
			sendMessage(comPort, readFileToString("game_state.xml"));
			ofstream file("game_state.xml");
			file << readMessage(comPort);
			file.close();
			parseGameStateXML("game_state.xml", firstPlayer, gameMode, board, gameStatus);
			CloseHandle(comPort);
		}
		cout << "\n\033[32m============================================= \033[0m\n";
		cout << "\033[32m   \033[0m               \033[32m" << gameStatus << "\033[0m\n";
		cout << "\033[32m============================================= \033[0m\n";
	}
	else if (gameMode == "Man vs AI") {
		string gameStatus = "Start";
		cout << "\n=============================================\n";
		cout << "      Hints for selecting cells";
		vector<vector<char>> boardAbout = {
			{'1', '2', '3'},
			{'4', '5', '6'},
			{'7', '8', '9'}
		};
		printBoard(boardAbout);
		cout << "      Board:";
		printBoard(board);
		while (gameStatus != "Win X" && gameStatus != "Win O" && gameStatus != "Draw") {
			HANDLE comPort = openSerialPort(portName, baudRate);
			if (comPort == INVALID_HANDLE_VALUE) {
				cout << "\033[2J\033[H";
				cout << "=============================================\n";
				cerr << "\n\033[31m      Error opening com-port! \033[0m" << endl;
				cout << "\n=============================================\n";
				return 1;
			}
			int move;
			cout << "      Enter your move (1-9): ";
			cin >> move;
			cout << "=============================================\n";
			if (!makeMove(board, move, firstPlayer)) {
				cout << "\033[31m      Cell already occupied! \033[0m" << endl;
				cout << "=============================================\n";
				CloseHandle(comPort);
				continue;

			}
			cout << "\033[2J\033[H";
			cout << "=============================================\n";
			cout << "      Board:";
			printBoard(board);
			updateXML(std::string(1, firstPlayer), gameMode, board);
			sendMessage(comPort, readFileToString("game_state.xml"));
			ofstream file("game_state.xml");
			file << readMessage(comPort);
			file.close();
			parseGameStateXML("game_state.xml", firstPlayer, gameMode, board, gameStatus);
			cout << "\033[2J\033[H";
			cout << "\n=============================================\n";
			cout << "      AI has made a move \n";
			cout << "=============================================\n";
			printBoard(board);
			CloseHandle(comPort);
		}
		cout << "\n\033[32m============================================= \033[0m\n";
		cout << "\033[32m   \033[0m               \033[32m" << gameStatus << "\033[0m\n";
		cout << "\033[32m============================================= \033[0m\n";
	}
	else if (gameMode == "AI vs Man") {
		string gameStatus = "Start";
		cout << "\n=============================================\n";
		cout << "      Hints for selecting cells";
		vector<vector<char>> boardAbout = {
			{'1', '2', '3'},
			{'4', '5', '6'},
			{'7', '8', '9'}
		};
		printBoard(boardAbout);
		cout << "      Board:";
		printBoard(board);
		while (gameStatus != "Win X" && gameStatus != "Win O" && gameStatus != "Draw") {
			HANDLE comPort = openSerialPort(portName, baudRate);
			if (comPort == INVALID_HANDLE_VALUE) {
				cout << "\033[2J\033[H";
				cout << "=============================================\n";
				cerr << "\n\033[31m      Error opening com-port! \033[0m" << endl;
				cout << "\n=============================================\n";
				return 1;
			}
			sendMessage(comPort, readFileToString("game_state.xml"));
			ofstream file("game_state.xml");
			file << readMessage(comPort);
			file.close();
			parseGameStateXML("game_state.xml", firstPlayer, gameMode, board, gameStatus);
			cout << "\033[2J\033[H";
			cout << "\n=============================================\n";
			cout << "      AI has made a move \n";
			cout << "=============================================\n";
			printBoard(board);
			int move;
			bool validMove = false;
			if (gameStatus == "Win X" || gameStatus == "Win O" || gameStatus == "Draw") {
				break;
			}
			while (!validMove) {
				cout << "      Enter your move (1-9): ";
				cin >> move;
				cout << "=============================================\n";
				if (makeMove(board, move, firstPlayer)) {
					validMove = true;
				}
				else {
					cout << "\033[31m      Cell already occupied! \033[0m" << endl;
					cout << "=============================================\n";
				}
			}
			cout << "\033[2J\033[H";
			cout << "=============================================\n";
			cout << "      Board:";

			printBoard(board);
			firstPlayer = (firstPlayer == 'X') ? 'O' : 'X';
			updateXML(std::string(1, firstPlayer), gameMode, board);

			CloseHandle(comPort);
		}
		cout << "\n\033[32m============================================= \033[0m\n";
		cout << "\033[32m   \033[0m               \033[32m" << gameStatus << "\033[0m\n";
		cout << "\033[32m============================================= \033[0m\n";
	}
	else if (gameMode == "AI vs AI") {
		string gameStatus = "Start";
		cout << "\n=============================================\n";
		cout << "      Board:";
		printBoard(board);
		while (gameStatus != "Win X" && gameStatus != "Win O" && gameStatus != "Draw") {
			HANDLE comPort = openSerialPort(portName, baudRate);
			if (comPort == INVALID_HANDLE_VALUE) {
				cout << "\033[2J\033[H";
				cout << "=============================================\n";
				cerr << "\n\033[31m      Error opening com-port! \033[0m" << endl;
				cout << "\n=============================================\n";
				return 1;
			}
			sendMessage(comPort, readFileToString("game_state.xml"));
			ofstream file("game_state.xml");
			file << readMessage(comPort);
			file.close();
			parseGameStateXML("game_state.xml", firstPlayer, gameMode, board, gameStatus);
			cout << "\033[2J\033[H";
			cout << "\n=============================================\n";
			cout << "      AI has made a move \n";
			cout << "=============================================\n";
			printBoard(board);
			CloseHandle(comPort);
		}
		cout << "\n\033[32m============================================= \033[0m\n";
		cout << "\033[32m   \033[0m               \033[32m" << gameStatus << "\033[0m\n";
		cout << "\033[32m============================================= \033[0m\n";
	}
	else {
		cout << "\033[2J\033[H";
		cout << "=============================================\n";
		cerr << "\n\033[31m      Something went wrong... \033[0m" << endl;
		cout << "\n=============================================\n";
		return 1;
	}
}