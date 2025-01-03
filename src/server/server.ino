/**
 * @brief Checks if a player has won.
 * 
 * Checks all possible combinations for a player to win on a 3x3 board.
 * 
 * @param player The player's symbol ('X' or 'O').
 * @param board The game board (3x3 array).
 * @return true if the player has won, false otherwise.
 */
bool checkWin(char player, char board[3][3]) {
  for (int i = 0; i < 3; i++) {
    if (board[i][0] == player && board[i][1] == player && board[i][2] == player) {
      return true;
    }
  }
  for (int i = 0; i < 3; i++) {
    if (board[0][i] == player && board[1][i] == player && board[2][i] == player) {
      return true;
    }
  }
  if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) || 
      (board[0][2] == player && board[1][1] == player && board[2][0] == player)) {
    return true;
  }
  return false;
}

/**
 * @brief Checks if the game has resulted in a draw.
 * 
 * Checks if the board is full and no player has won, indicating a draw.
 * 
 * @param board The game board (3x3 array).
 * @return true if the game is a draw, false otherwise.
 */
bool checkDraw(char board[3][3]) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == '_') {
        return false;
      }
    }
  }
  return true;
}

/**
 * @brief Makes a random move for the AI.
 * 
 * Selects an empty spot on the board and places the AI's symbol there.
 * 
 * @param aiSymbol The AI's symbol ('X' or 'O').
 * @param board The game board (3x3 array).
 */
void aiMove(char aiSymbol, char board[3][3]) {
  int row, col;
  do {
    row = random(0, 3);
    col = random(0, 3);
  } while (board[row][col] != '_');
  board[row][col] = aiSymbol;
}

/**
 * @brief Makes a move for the AI with dynamic weights.
 * 
 * The AI attempts to win or block the opponent from winning. If no such move is possible,
 * it selects the best move based on predefined weights.
 * 
 * @param aiSymbol The AI's symbol ('X' or 'O').
 * @param board The game board (3x3 array).
 */
void aiMoveWithDynamicWeights(char aiSymbol, char board[3][3]) {
  // Try to win
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == '_') {
        board[i][j] = aiSymbol;
        if (checkWin(board, aiSymbol)) {
          return;
        }
        board[i][j] = '_';
      }
    }
  }
  
  // Block opponent from winning
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == '_') {
        char playerSymbol = (aiSymbol == 'X') ? 'X' : 'O';
        board[i][j] = playerSymbol;
        if (checkWin(board, playerSymbol)) {
          board[i][j] = aiSymbol;
          return;
        }
        board[i][j] = '_';
      }
    }
  }
  
  // Select the best move based on predefined weights
  int weights[3][3] = {
    { 3, 1, 3 },
    { 1, 5, 1 },
    { 3, 1, 3 }
  };
  int bestRow = -1, bestCol = -1;
  int maxWeight = -1;
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == '_' && weights[i][j] > maxWeight) {
        maxWeight = weights[i][j];
        bestRow = i;
        bestCol = j;
      }
    }
  }
  if (bestRow != -1 && bestCol != -1) {
    board[bestRow][bestCol] = aiSymbol;
  }
}

/**
 * @brief Updates the game status based on the current board state.
 * 
 * Determines whether the game is still ongoing, or if there's a winner or a draw.
 * 
 * @param board The game board (3x3 array).
 * @param status The current game status (Win X, Win O, Draw, NextMove).
 */
void updateGameStateStatus(char board[3][3], char* status) {
  if (checkWin('X', board)) {
    strcpy(status, "Win X");
  } else if (checkWin('O', board)) {
    strcpy(status, "Win O");
  } else if (checkDraw(board)) {
    strcpy(status, "Draw");
  } else {
    strcpy(status, "NextMove");
  }
}

/**
 * @brief Extracts text between specified XML tags.
 * 
 * Extracts the value enclosed between <tag> and </tag> in the provided XML string.
 * 
 * @param xml The XML string.
 * @param tag The tag name (e.g., "Player").
 * @param output The buffer to store the extracted text.
 * @param outputSize The size of the output buffer.
 */
void extractTextBetweenTags(const char* xml, const char* tag, char* output, int outputSize) {
  char openTag[20];
  char closeTag[20];
  snprintf(openTag, sizeof(openTag), "<%s>", tag);
  snprintf(closeTag, sizeof(closeTag), "</%s>", tag);
  const char* startPos = strstr(xml, openTag);
  if (!startPos) {
    output[0] = '\0';
    return;
  }
  startPos += strlen(openTag);
  const char* endPos = strstr(startPos, closeTag);
  if (!endPos) {
    output[0] = '\0';
    return;
  }
  int length = endPos - startPos;
  if (length >= outputSize) length = outputSize - 1;
  strncpy(output, startPos, length);
  output[length] = '\0';
}

/**
 * @brief Extracts the game board from the provided XML data.
 * 
 * Extracts a 3x3 game board from the XML string.
 * 
 * @param xml The XML string.
 * @param board The game board (3x3 array) to populate.
 */
void extractBoard(const char* xml, char board[3][3]) {
  const char* boardStart = strstr(xml, "<Board>");
  if (!boardStart) return;
  int row = 0;
  const char* rowPos = boardStart;
  while (row < 3 && (rowPos = strstr(rowPos, "<Row>"))) {
    int col = 0;
    const char* cellPos = rowPos;
    while (col < 3 && (cellPos = strstr(cellPos, "<Cell>"))) {
      cellPos += 6;
      const char* cellEnd = strstr(cellPos, "</Cell>");
      if (!cellEnd) return;
      board[row][col] = *cellPos;
      col++;
      cellPos = cellEnd;
    }
    row++;
    rowPos = strstr(rowPos, "</Row>");
  }
}

/**
 * @brief Reads the XML game data and updates the game logic.
 * 
 * This function parses the XML data, updates the game state, and makes AI moves when needed.
 * 
 * @param xmlData The game data in XML format.
 */
void readAndUpdateGameLogic(const String& xmlData) {
  char player[2];
  char gameType[20];
  char status[10];
  char board[3][3];
  const char* xmlCString = xmlData.c_str();
  extractTextBetweenTags(xmlCString, "Player", player, sizeof(player));
  extractTextBetweenTags(xmlCString, "GameType", gameType, sizeof(gameType));
  extractTextBetweenTags(xmlCString, "Status", status, sizeof(status));
  extractBoard(xmlCString, board);
  if (strcmp(gameType, "Man vs Man") == 0 && (strcmp(status, "Start") == 0 || strcmp(status, "NextMove") == 0)) {
    player[0] = (player[0] == 'X') ? 'O' : 'X';
    updateGameStateStatus(board, status);
  }
  if (strcmp(gameType, "AI vs Man") == 0 && (strcmp(status, "Start") == 0 || strcmp(status, "NextMove") == 0)) {
    if (strcmp(status, "Start") != 0)
      player[0] = (player[0] == 'X') ? 'O' : 'X';
    updateGameStateStatus(board, status);
    if (strcmp(status, "Win X") != 0 && strcmp(status, "Win O") != 0 && strcmp(status, "Draw") != 0) {
      aiMoveWithDynamicWeights(player[0], board);
      player[0] = (player[0] == 'X') ? 'O' : 'X';
      updateGameStateStatus(board, status);
    }
  }
  if (strcmp(gameType, "Man vs AI") == 0 && (strcmp(status, "Start") == 0 || strcmp(status, "NextMove") == 0)) {
    updateGameStateStatus(board, status);
    if (strcmp(status, "Win X") != 0 && strcmp(status, "Win O") != 0 && strcmp(status, "Draw") != 0) {
      player[0] = (player[0] == 'X') ? 'O' : 'X';
      aiMoveWithDynamicWeights(player[0], board);
      player[0] = (player[0] == 'X') ? 'O' : 'X';
      updateGameStateStatus(board, status);
    }
  }
  if (strcmp(gameType, "AI vs AI") == 0 && (strcmp(status, "Start") == 0 || strcmp(status, "NextMove") == 0)) {
    updateGameStateStatus(board, status);
    if (strcmp(status, "Win X") != 0 && strcmp(status, "Win O") != 0 && strcmp(status, "Draw") != 0) {
      for (int i = 0; i < 9; i++) {
        if (strcmp(status, "Win X") != 0 && strcmp(status, "Win O") != 0 && strcmp(status, "Draw") != 0) {
          aiMove(player[0], board);
          player[0] = (player[0] == 'X') ? 'O' : 'X';
          updateGameStateStatus(board, status);
        }
      }
    }
  }
  exportGameStateXML(player, gameType, status, board);
}

/**
 * @brief Exports the game state to XML format.
 * 
 * Converts the current game state (player, game type, status, and board) to XML format
 * and sends it to the serial output.
 * 
 * @param player The current player ('X' or 'O').
 * @param gameType The type of game (e.g., "Man vs Man", "AI vs Man").
 * @param status The current game status (e.g., "NextMove", "Win X", "Draw").
 * @param board The current game board (3x3 array).
 */
void exportGameStateXML(const char* player, const char* gameType, const char* status, char board[3][3]) {
  Serial.print("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
  Serial.print("<GameState>");
  Serial.print("<Player>");
  Serial.print(player);
  Serial.print("</Player>");
  Serial.print("<GameType>");
  Serial.print(gameType);
  Serial.print("</GameType>");
  Serial.print("<Board>");
  for (int i = 0; i < 3; i++) {
    Serial.print("<Row>");
    for (int j = 0; j < 3; j++) {
      Serial.print("<Cell>");
      Serial.print(board[i][j]);
      Serial.print("</Cell>");
    }
    Serial.print("</Row>");
  }
  Serial.print("</Board>");
  Serial.print("<Status>");
  Serial.print(status);
  Serial.print("</Status>");
  Serial.print("</GameState>");
}

/**
 * @brief Initializes the game and serial communication.
 * 
 * Initializes the serial communication and sets the random seed based on analog input.
 */
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

/**
 * @brief Main game loop.
 * 
 * Continuously checks for incoming data, reads and processes the XML data, updates
 * the game state, and exports the updated game state back to the serial output.
 */
void loop() {
  if (Serial.available() > 0) {
    String xmlData = Serial.readStringUntil('\r\n');
    if (xmlData.indexOf("</GameState>") != -1) {
      readAndUpdateGameLogic(xmlData);
      Serial.println();
    } else {
      Serial.println("Error: </GameState> not found.");
    }
  }
}
