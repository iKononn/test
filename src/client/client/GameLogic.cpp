#include "GameLogic.h"

/**
 * @brief Parses a game state XML file and extracts game data.
 *
 * This function loads a specified XML file and extracts the game state, including the first player,
 * game type, board state, and game status. The parsed values are stored in the provided references.
 *
 * @param filename The name of the XML file to load.
 * @param firstPlayer Reference to a character where the first player ('X' or 'O') will be stored.
 * @param gameMode Reference to a string where the game mode (e.g., "Man vs Man") will be stored.
 * @param board Reference to a 2D vector of characters representing the board state.
 * @param gameStatus Reference to a string where the game status (e.g., "Start") will be stored.
 */
void parseGameStateXML(const string& filename, char& firstPlayer, string& gameMode, vector<vector<char>>& board, string& gameStatus) {
    tinyxml2::XMLDocument doc;
    XMLError eResult = doc.LoadFile(filename.c_str());
    if (eResult != XML_SUCCESS) {
        cerr << "Error loading XML file: " << doc.ErrorStr() << endl;
        return;
    }
    XMLNode* root = doc.FirstChildElement("GameState");
    if (root == nullptr) {
        cerr << "No <GameState> element found!" << endl;
        return;
    }
    XMLElement* playerElement = root->FirstChildElement("Player");
    if (playerElement != nullptr) {
        const char* playerText = playerElement->GetText();
        if (playerText != nullptr) {
            firstPlayer = playerText[0];
        }
    }
    XMLElement* gameTypeElement = root->FirstChildElement("GameType");
    if (gameTypeElement != nullptr) {
        const char* gameTypeText = gameTypeElement->GetText();
        if (gameTypeText != nullptr) {
            gameMode = gameTypeText;
        }
    }
    XMLElement* boardElement = root->FirstChildElement("Board");
    if (boardElement != nullptr) {
        int row = 0;
        for (XMLElement* rowElement = boardElement->FirstChildElement("Row"); rowElement != nullptr; rowElement = rowElement->NextSiblingElement("Row")) {
            int col = 0;
            for (XMLElement* cellElement = rowElement->FirstChildElement("Cell"); cellElement != nullptr; cellElement = cellElement->NextSiblingElement("Cell")) {
                const char* cellText = cellElement->GetText();
                if (cellText != nullptr && col < 3) {
                    board[row][col] = cellText[0];
                }
                col++;
            }
            row++;
        }
    }
    XMLElement* statusElement = root->FirstChildElement("Status");
    if (statusElement != nullptr) {
        const char* statusText = statusElement->GetText();
        if (statusText != nullptr) {
            gameStatus = statusText;
        }
    }
}

/**
 * @brief Updates the game state in the XML file.
 *
 * This function writes the current player, game type, and board state to an XML file. It creates or
 * overwrites the "game_state.xml" file with the updated information.
 *
 * @param player The current player ('X' or 'O').
 * @param gameType The current game type (e.g., "Man vs Man").
 * @param board A 2D vector representing the current state of the game board.
 */
void updateXML(const string& player, const string& gameType, const vector<vector<char>>& board) {
    ofstream file("game_state.xml");
    file << "<GameState><Player>" << player << "</Player><GameType>" << gameType << "</GameType><Board>";
    for (int i = 0; i < 3; ++i) {
        file << "<Row>";
        for (int j = 0; j < 3; ++j) {
            file << "<Cell>" << board[i][j] << "</Cell>";
        }
        file << "</Row>";
    }
    file << "</Board><Status>Start</Status></GameState>\n";
    file.close();
}

/**
 * @brief Creates a new game state XML file with initial values.
 *
 * This function generates a new XML file with the provided first player, game type, and board state.
 * The game status is set to "Start".
 *
 * @param filename The name of the XML file to create.
 * @param firstPlayer The first player ('X' or 'O').
 * @param gameMode The game type (e.g., "Man vs Man").
 * @param board A 2D vector representing the initial state of the game board.
 */
void createGameStateXML(const string& filename, char firstPlayer, const string& gameMode, const vector<vector<char>>& board) {
    ofstream file(filename);
    if (file.is_open()) {
        file << "<GameState><Player>" << firstPlayer << "</Player>"
            << "<GameType>" << gameMode << "</GameType>"
            << "<Board>";
        for (const auto& row : board) {
            file << "<Row>";
            for (const auto& cell : row) {
                file << "<Cell>" << cell << "</Cell>";
            }
            file << "</Row>";
        }
        file << "</Board><Status>Start</Status></GameState>\n";
        file.close();
    }
    else {
        cerr << "Failed to create XML file!" << endl;
    }
}

/**
 * @brief Reads the contents of a file and returns it as a string.
 *
 * This function opens a specified file, reads its contents, and returns them as a single string.
 *
 * @param fileName The name of the file to read.
 * @return string The contents of the file as a string.
 */
string readFileToString(const string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "File could not be opened: " << fileName << endl;
        return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

/**
 * @brief Prompts the user to select who goes first in the game.
 *
 * This function asks the user whether 'X' or 'O' should go first, or if it should be chosen randomly.
 * It returns a character ('X' or 'O') based on the user's choice or random selection.
 *
 * @return char The player ('X' or 'O') who will make the first move.
 */
char selectFirstPlayer() {
    int firstMoveChoice;
    cout << "=============================================\n";
    cout << "      	The first move:\n";
    cout << "=============================================";
    cout << "\n  [1]  X goes first\n";
    cout << "  [2]  O goes first\n";
    cout << "  [3]  Random choice\n";
    cout << "\nPlease, enter your choice (1-3): ";
    cin >> firstMoveChoice;
    if (firstMoveChoice == 3) {
        srand(time(0));
        return (rand() % 2 == 0) ? 'X' : 'O';
    }
    return (firstMoveChoice == 1) ? 'X' : 'O';
}

/**
 * @brief Prompts the user to select the game mode.
 *
 * This function asks the user to choose the type of game to play, such as 'Man vs Man', 'Man vs AI',
 * 'AI vs Man', or 'AI vs AI'. It returns a string representing the selected game mode.
 *
 * @return string The selected game mode (e.g., "Man vs Man", "Man vs AI").
 */
string selectGameMode() {
    int gameModeChoice;
    cout << "=============================================\n";
    cout << "      	Game type:\n";
    cout << "=============================================";
    cout << "\n  [1]  Man vs Man\n";
    cout << "  [2]  Man vs AI\n";
    cout << "  [3]  AI vs Man\n";
    cout << "  [4]  AI vs AI\n";
    cout << "\nPlease, enter your choice (1-4): ";
    cin >> gameModeChoice;
    switch (gameModeChoice) {
    case 1: return "Man vs Man";
    case 2: return "Man vs AI";
    case 3: return "AI vs Man";
    case 4: return "AI vs AI";
    default:
        cerr << "Invalid choice, defaulting to Man vs Man." << endl;
        return "Man vs Man";
    }
}

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
bool makeMove(vector<vector<char>>& board, int move, char player) {
    int row = (move - 1) / 3;
    int col = (move - 1) % 3;
    if (board[row][col] == '_') {
        board[row][col] = player;
        return true;
    }
    else {
        return false;
    }
}

/**
 * @brief Prints the current state of the Tic-Tac-Toe board.
 *
 * This function outputs the current state of the game board to the console. It displays the 3x3 grid
 * with each cell containing either an 'X', 'O', or an empty space ('_'). The board is visually formatted
 * with lines separating the rows and columns for easier readability. The cells are colorized with green text.
 *
 * @param board A constant reference to a 2D vector representing the Tic-Tac-Toe game board.
 */
void printBoard(const vector<vector<char>>& board) {
    cout << "\n=============================================\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << "\033[32m" << " " << board[i][j] << " " << "\033[0m";
            if (j < 2) cout << "|";
        }
        cout << "\n";
        if (i < 2) {
            cout << "-----------\n";
        }
    }
    cout << "=============================================\n";
}