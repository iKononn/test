#include "pch.h"

#include <gtest/gtest.h>
#include "SerialPort.h"
#include <string>

using namespace std;
wstring comport;
DWORD baudRate;

string sendReceiveData(const string& inputData) {
    HANDLE hSerial = openSerialPort(comport, baudRate);
    if (hSerial == INVALID_HANDLE_VALUE) {
        throw runtime_error("Error opening serial port");
    }
    sendMessage(hSerial, inputData);
    string response = readMessage(hSerial);
    CloseHandle(hSerial);
    return response;
}

TEST(ServerTest, TestInitialGameState) {
    string inputXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><GameState><Player>O</Player><GameType>Man vs AI</GameType><Board><Row><Cell>_</Cell><Cell>_</Cell><Cell>_</Cell></Row><Row><Cell>_</Cell><Cell>_</Cell><Cell>_</Cell></Row><Row><Cell>_</Cell><Cell>_</Cell><Cell>_</Cell></Row></Board><Status>Start</Status></GameState>";
    string response = sendReceiveData(inputXml);
    EXPECT_TRUE(response.find("<Status>NextMove</Status>") != string::npos);
    EXPECT_TRUE(response.find("<Player>O</Player>") != string::npos);
    EXPECT_TRUE(response.find("<GameType>Man vs AI</GameType>") != string::npos);
}

TEST(ServerTest, TestGameStateUpdate) {
    string inputXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><GameState><Player>O</Player><GameType>AI vs AI</GameType><Board><Row><Cell>O</Cell><Cell>_</Cell><Cell>X</Cell></Row><Row><Cell>O</Cell><Cell>X</Cell><Cell>X</Cell></Row><Row><Cell>X</Cell><Cell>_</Cell><Cell>O</Cell></Row></Board><Status>NextMove</Status></GameState>";
    string response = sendReceiveData(inputXml);
    EXPECT_TRUE(response.find("<Status>Win X</Status>") != string::npos);
}

TEST(ServerTest, TestFirstMoveGameState) {
    string inputXml = "<?xml version=\"1.0\" encoding=\"utf - 8\"?><GameState><Player>O</Player><GameType>Man vs Man</GameType><Board><Row><Cell>_</Cell><Cell>_</Cell><Cell>_</Cell></Row><Row><Cell>_</Cell><Cell>X</Cell><Cell>_</Cell></Row><Row><Cell>_</Cell><Cell>_</Cell><Cell>_</Cell></Row></Board><Status>NextMove</Status></GameState>";
    string response = sendReceiveData(inputXml);
    EXPECT_TRUE(response.find("<Status>NextMove</Status>") != string::npos);
}

TEST(ServerTest, TestDrawGameState) {
    string inputXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><GameState><Player>O</Player><GameType>AI vs AI</GameType><Board><Row><Cell>X</Cell><Cell>O</Cell><Cell>X</Cell></Row><Row><Cell>O</Cell><Cell>X</Cell><Cell>O</Cell></Row><Row><Cell>X</Cell><Cell>O</Cell><Cell>X</Cell></Row></Board><Status>Draw</Status></GameState>";
    string response = sendReceiveData(inputXml);
    EXPECT_TRUE(response.find("<Status>Draw</Status>") != string::npos);
    EXPECT_TRUE(response.find("<Cell>X</Cell>") != string::npos);
    EXPECT_TRUE(response.find("<Cell>O</Cell>") != string::npos);
    EXPECT_TRUE(response.find("<Cell>_</Cell>") == string::npos);
}

TEST(ServerTest, TestPlayerXWin) {
    string inputXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><GameState><Player>O</Player><GameType>AI vs AI</GameType><Board><Row><Cell>X</Cell><Cell>X</Cell><Cell>X</Cell></Row><Row><Cell>O</Cell><Cell>O</Cell><Cell>_</Cell></Row><Row><Cell>_</Cell><Cell>_</Cell><Cell>_</Cell></Row></Board><Status>Win X</Status></GameState>";
    string response = sendReceiveData(inputXml);
    EXPECT_TRUE(response.find("<Status>Win X</Status>") != string::npos);
    EXPECT_TRUE(response.find("<Cell>X</Cell>") != string::npos);
    EXPECT_TRUE(response.find("<Cell>O</Cell>") != string::npos);
    EXPECT_TRUE(response.find("<Cell>X</Cell><Cell>X</Cell><Cell>X</Cell>") != string::npos);
}

TEST(ServerTest, TestTurnSwitching) {
    string inputXml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><GameState><Player>X</Player><GameType>AI vs Man</GameType><Board><Row><Cell>X</Cell><Cell>_</Cell><Cell>_</Cell></Row><Row><Cell>_</Cell><Cell>_</Cell><Cell>_</Cell></Row><Row><Cell>_</Cell><Cell>_</Cell><Cell>_</Cell></Row></Board><Status>NextMove</Status></GameState>";
    string response = sendReceiveData(inputXml);
    EXPECT_TRUE(response.find("<Player>X</Player>") != string::npos);
    EXPECT_TRUE(response.find("<Status>NextMove</Status>") != string::npos);
    EXPECT_TRUE(response.find("<Cell>X</Cell>") != string::npos);
}

int main(int argc, char** argv) {
    int result;
    ::testing::InitGoogleTest(&argc, argv);
    if (argc > 2) {
        comport = wstring(argv[1], argv[1] + strlen(argv[1]));
        baudRate = stoi(argv[2]);
        cout << endl << "--------------------------------------------------" << endl;
        cout << "Using COM-port: " << argv[1] << " with baud rate: " << baudRate << "...";
        cout << endl << "--------------------------------------------------" << endl;
        result = RUN_ALL_TESTS();
    }
    else {
        cout << endl << "--------------------------------------------------" << endl;
        wcout << L"No COM-port or BAUD RATE specified...";
        cout << endl << "--------------------------------------------------" << endl;
        result = -1;
    }
    return result;
}