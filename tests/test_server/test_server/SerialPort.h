#pragma once
#include "pch.h"
#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

HANDLE openSerialPort(const wstring& portName, DWORD baudRate);

void sendMessage(HANDLE hSerial, string message);

string readMessage(HANDLE hSerial);