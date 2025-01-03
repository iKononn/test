#include "SerialPort.h"

/**
 * @brief Opens a serial port with specified settings.
 *
 * This function initializes the serial port with the specified port name and baud rate.
 * It configures the communication parameters such as baud rate, byte size, stop bits, and parity.
 * The function also sets timeouts for read and write operations.
 *
 * @param portName The name of the port to open (e.g., "COM1").
 * @param baudRate The baud rate for the serial communication (e.g., 9600, 115200).
 *
 * @return HANDLE Handle to the opened serial port, or INVALID_HANDLE_VALUE if the operation fails.
 */
HANDLE openSerialPort(const wstring& portName, DWORD baudRate) {
    HANDLE hSerial = CreateFile(portName.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        return INVALID_HANDLE_VALUE;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return INVALID_HANDLE_VALUE;
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 1000;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    DWORD dwInQueue = 0;
    DWORD dwOutQueue = 0;
    SetupComm(hSerial, 8192, 8192);
    SetCommTimeouts(hSerial, &timeouts);

    return hSerial;
}

/**
 * @brief Sends a message to the serial port.
 *
 * This function writes the given message to the serial port.
 * It uses the `WriteFile` function to send the data and waits for the specified message length to be written.
 *
 * @param hSerial Handle to the serial port.
 * @param message The message to send as a C-style string.
 */
void sendMessage(HANDLE hSerial, const string& message) {
    DWORD bytesWritten;
    WriteFile(hSerial, message.c_str(), message.length(), &bytesWritten, NULL);
    Sleep(100);
}

/**
 * @brief Reads a message from the serial port.
 *
 * This function reads data from the serial port and stores it in a buffer.
 * It waits until a newline character (`\n`) is encountered, indicating the end of the message.
 * The message is then returned as a `string`.
 *
 * @param hSerial Handle to the serial port.
 *
 * @return string The message read from the serial port.
 */
string readMessage(HANDLE hSerial) {
    static char buffer[256];
    DWORD bytesRead;
    string result;
    while (true) {
        if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
            buffer[bytesRead] = '\0';
            result.append(buffer, bytesRead);
            if (result.find('\n') != string::npos) {
                break;
            }
        }
    }
    return result;
}

/**
 * @brief Prompts the user to select a COM port.
 *
 * This function displays a list of available COM ports (COM1 to COM9) and prompts
 * the user to select one by entering a number corresponding to the port. The selected
 * port is returned as a wide string (e.g., L"COM1").
 *
 * @return wstring The COM port selected by the user (e.g., L"COM1").
 */
wstring selectPort() {
    int comChoice;
    cout << "      Available COM Ports:\n";
    cout << "=============================================\n";
    for (int i = 1; i <= 9; ++i) {
        cout << "  [" << i << "]  COM" << i << "\n";
    }
    cout << "\nPlease, enter your choice (1-9): ";
    cin >> comChoice;
    switch (comChoice) {
    case 1: return L"COM1";
    case 2: return L"COM2";
    case 3: return L"COM3";
    case 4: return L"COM4";
    case 5: return L"COM5";
    case 6: return L"COM6";
    case 7: return L"COM7";
    case 8: return L"COM8";
    case 9: return L"COM9";
    default:
        cerr << "Invalid choice, defaulting to COM1." << endl;
        return L"COM1";
    }
}

/**
 * @brief Prompts the user to select a baud rate for serial communication.
 *
 * This function displays a list of available baud rates (9600, 115200, 19200) and prompts
 * the user to select one. The selected baud rate is returned as a DWORD value.
 *
 * @return DWORD The baud rate selected by the user (e.g., CBR_9600).
 */
DWORD selectBaudRate() {
    cout << "=============================================\n";
    cout << "      Available Baud Rates:\n";
    cout << "=============================================\n";
    vector<DWORD> baudRates = { CBR_9600, CBR_115200, CBR_19200 };
    vector<string> baudOptions = { "9600", "115200", "19200" };
    for (size_t i = 0; i < baudOptions.size(); ++i) {
        cout << "  [" << i + 1 << "]  " << baudOptions[i] << " bps\n";
    }
    int baudChoice;
    cout << "\nPlease, enter your choice (1-3): ";
    cin >> baudChoice;
    if (baudChoice >= 1 && baudChoice <= 3) {
        return baudRates[baudChoice - 1];
    }
    else {
        cerr << "Invalid choice, using default (9600 bps)." << endl;
        return CBR_9600;
    }
}