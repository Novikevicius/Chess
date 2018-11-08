#pragma once
#include <windows.h>
#include <strsafe.h>
#include <minwinbase.h>
#include <string>

class ChessEngine
{
	HANDLE engineInput_read = NULL;
	HANDLE engineInput_write = NULL;
	HANDLE engineOutput_read = NULL;
	HANDLE engineOutput_write = NULL;
	SECURITY_ATTRIBUTES saAttr;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	bool hasRequestedMove;

	static ChessEngine* instance;
private:
	ChessEngine();
public:
	//Lazy singleton
	static ChessEngine* GetInstance()
	{
		if (!instance)
			instance = new ChessEngine();
		return instance;
	}
	ChessEngine(const ChessEngine& e) = delete;
	ChessEngine operator= (const ChessEngine& rhs) = delete;
public:
	~ChessEngine();
	std::string GetMsg();
	void SendMsg(const char* _msg);
	std::string GetBestmove(const std::string& fen, const int& depth = 0);
	bool HasRequestedMove() const { return hasRequestedMove; }
	void FreeMemory();
private:
	int GetStringSize(const char* str) const;
};

