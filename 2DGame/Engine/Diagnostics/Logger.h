#pragma once

//Level of importance of the message
enum LogLevel
{
	Info,
	Warning,
	Error,
	Fix
};

class Logger
{
public:
	static void Initialize();
	static void Release();
	static void Log(LogLevel loglevel, const std::wstring& msg);
	static void LogInfo( const std::wstring& msg);
	static void LogWarning( const std::wstring& msg);
	static void LogError( const std::wstring& msg);
	static void LogFix(const std::wstring& msg);
	static void LogGLEnumError(GLenum result);
	static void LogGLEnumInfo(GLenum info);

private:
	//------------------------------------------//
	//			Private Functions				//
	//------------------------------------------//

	Logger(void) {};
	~Logger(void){};


private:
	//------------------------------------------//
	//			Member Variables				//
	//------------------------------------------//
	static bool m_Isinitialized;
};

