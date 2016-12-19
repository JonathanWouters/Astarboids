//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "GameCore.h"

int main(int argc, char** argv)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	//Enable memory leak detection in debug mode
#ifdef _DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	flag |= _CRTDBG_LEAK_CHECK_DF; // Turn on leak-checking bit
	_CrtSetDbgFlag(flag);
#endif

	GameCore* pGame = new GameCore();

	pGame->RunGame();

	delete(pGame);

	exit(EXIT_SUCCESS);
}