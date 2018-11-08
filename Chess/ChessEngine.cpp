#include "ChessEngine.h"
#include <iostream>
#include <cassert>

ChessEngine* ChessEngine::instance = nullptr;

ChessEngine::ChessEngine()
{
	std::cout << "Initializing chess engine\n";

	ZeroMemory(&saAttr, sizeof(saAttr));
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	CreatePipe(&engineInput_read, &engineInput_write, &saAttr, 0);
	CreatePipe(&engineOutput_read, &engineOutput_write, &saAttr, 0);

	si.cb = sizeof(si);
	si.hStdInput = engineInput_read;
	si.hStdOutput = engineOutput_write;
	si.hStdError = engineOutput_write;
	si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	si.wShowWindow = FALSE;

	if (!CreateProcess("stockfish_9_x64.exe",
		NULL,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		TRUE,          // Set handle inheritance to TRUE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		)
	{
		std::cout << "CreateProcess failed " << GetLastError() << std::endl;;
		throw std::runtime_error("Chess engine process creation error!");
	}
	std::cout << "Chess engine info:\n";
	std::cout << GetMsg();
	SendMsg("uci\n");
	std::string str = GetMsg();
	if(str.find("uciok") == std::string::npos)
	{
		std::cout << "Msg is: " << str << '\n';
		throw std::invalid_argument("Engine is not supporting uci protocol");
	}
	SendMsg("setoption name Skill Level value 20\n");
	SendMsg("isready\n");
	int timer = 0;
	while(GetMsg().find("readyok") == std::string::npos)
	{
		if(timer > 120)
			throw std::invalid_argument("Engine is not ready");
		timer++;
	}
	std::cout << "Chess engine initialized\n";
}
ChessEngine::~ChessEngine()
{
	SendMsg("quit\n");
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	std::cout << "Chess engine memory freed\n";
}
void ChessEngine::SendMsg(const char* _msg)
{
	if (!_msg)
		return;
	int size = GetStringSize(_msg) + 1;
	char* msg = new char[size];
	ZeroMemory(msg, size - 1);
	for (int i = 0; i < size; i++)
	{
		msg[i] = _msg[i];
	}
	DWORD writtenCountBytes = 0;
	//for (int i = 0; i < size - 1; i++)
	{
		std::cout << msg;
		WriteFile(engineInput_write, msg, size-1, &writtenCountBytes, NULL);
	}
	delete[] msg;
}

std::string ChessEngine::GetBestmove(const std::string& fen, const int& depth)
{
	if(!hasRequestedMove)
	{
		// send to the engine current position
		std::string msg = "position fen " + fen + '\n';
		SendMsg(msg.c_str());
		std::string goCmd = "go ";
		if(depth != 0)
			goCmd += "depth " + std::to_string(depth) + '\n';
		else
			goCmd += '\n';
		// start searching for best move
		SendMsg(goCmd.c_str());
		hasRequestedMove = true;
	}
	auto answer = GetMsg();
	auto pos = answer.find("bestmove");
	while (pos == std::string::npos || answer.size() < pos + 9 + 6)
	{
		answer += GetMsg();
		pos = answer.find("bestmove");
	}
	/*{
		std::cout << answer;
		std::cout << "Error: too short string";
	}*/
	// if still searching
	if (pos == std::string::npos)
	{
		return "w";
	}
	hasRequestedMove = false;
	auto temp = answer.substr(pos + 9, 6);
	if(temp == "")
	{
		throw std::runtime_error("bestmove is empty string\n");
	}
	if(temp.find("(none)") != std::string::npos)
	{
		return "none";
	}
	hasRequestedMove = false;
	if(temp.size() < 4)
	{
		std::cout << "Error: too short string";
	}
	return temp.substr(0, 4);
}

std::string ChessEngine::GetMsg()
{
	static const UINT32 bufSize = 2000;
	char* buf = new char[bufSize];
	ZeroMemory(buf, bufSize);
	DWORD readCountBytes = 1;
	DWORD availableBytes = 1;
	/*
	DWORD t = 0;
	char symbol = '\0';
	if(PeekNamedPipe(engineOutput_read, nullptr, bufSize, 0, &availableBytes, &t))
	{
		int i = 0;
		while(symbol != '\n')
		{
			ReadFile(engineOutput_read, &symbol, 1, &readCountBytes, NULL);
			if(readCountBytes != 1)
			{
				throw std::runtime_error("pipe reading error");
			}
			buf[i++] = symbol;
		}
	}
	*/
	ReadFile(engineOutput_read, buf, bufSize, &readCountBytes, NULL);
	if (readCountBytes > bufSize)
	{
		throw std::overflow_error("Could not read full engine message: too small buffer size");
	}
	std::string msg(buf);
	//std::cout << msg << std::endl;
	delete[] buf;
	return msg;
}
/*
std::string ChessEngine::GetMsg()
{
	static const UINT32 bufSize = 90000;
	DWORD readCountBytes = 1;
	DWORD availableBytes = 1;
	DWORD bytesleft = 1;
	std::string msg;
	while (1)
	{
		if(PeekNamedPipe(engineOutput_read, nullptr, bufSize, nullptr, &availableBytes, 0))
		{		
			if (availableBytes)
			{
				char* buf = new char[availableBytes + 2];
				//while(symbol != '\n')
				{
					ReadFile(engineOutput_read, buf, availableBytes, &readCountBytes, NULL);
					//if(readCountBytes != 1)
					{
						//throw std::runtime_error("pipe reading error");
					}
				}
				buf[availableBytes + 1] = '\0';
				msg += buf;
				delete[] buf;
			}
			else
			{
				std::cout << "Nothing to read\n";
				break;
			}
		}
		else
		{
			std::cout << "Error Code: " << GetLastError() << '\n';
			throw std::runtime_error("PeekNamedPipe returned false");
		}
	}
	std::cout << msg << std::endl;
	return msg;
}
*/
void ChessEngine::FreeMemory()
{
	if(!instance)
		return;
	delete instance;
	instance = nullptr;
}

int ChessEngine::GetStringSize(const char* str) const
{
	if (!str)
		return 0;
	int size = 0;
	while (str[size] != '\0')
		size++;
	return size;
}
