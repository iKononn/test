/**
 * @file SerialPort.h
 * @brief Contains functions for serial port communication in Windows.
 */

#pragma once

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

 /**
  * @brief Baud rates constants for serial communication.
  */
#define CBR_9600            9600
#define CBR_19200           19200
#define CBR_115200          115200

  /**
   * @brief Opens a serial port with specified settings.
   *
   * @param portName The name of the port to open (e.g., "COM1").
   * @param baudRate The baud rate for the serial communication (e.g., 9600, 115200).
   *
   * @return HANDLE Handle to the opened serial port, or INVALID_HANDLE_VALUE if the operation fails.
   */
HANDLE openSerialPort(const wstring& portName, DWORD baudRate);

/**
 * @brief Sends a message to the serial port.
 *
 * @param hSerial Handle to the serial port.
 * @param message The message to send as a C-style string.
 */
void sendMessage(HANDLE hSerial, const string& message);

/**
 * @brief Reads a message from the serial port.
 *
 * @param hSerial Handle to the serial port.
 *
 * @return string The message read from the serial port.
 */
string readMessage(HANDLE hSerial);

/**
 * @brief Prompts the user to select a COM port.
 *
 * This function displays a list of available COM ports (COM1 to COM9) and prompts
 * the user to select one by entering a number corresponding to the port. The selected
 * port is returned as a wide string (e.g., L"COM1").
 *
 * @return wstring The COM port selected by the user (e.g., L"COM1").
 */
wstring selectPort();

/**
 * @brief Prompts the user to select a baud rate for serial communication.
 *
 * This function displays a list of available baud rates (9600, 115200, 19200) and prompts
 * the user to select one. The selected baud rate is returned as a DWORD value.
 *
 * @return DWORD The baud rate selected by the user (e.g., CBR_9600).
 */
DWORD selectBaudRate();