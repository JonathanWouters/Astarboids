//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"

#include "Logger.h"

//static member variabel initialization
bool Logger::m_Isinitialized = false;

void Logger::Initialize()
{
	//If the logger is already initialized log a warning
	if (m_Isinitialized)
	{
		Log(LogLevel::Warning,L"Logger::Initialize - Logger is already initialized !");
		return;
	}

	//Initialize logger;

	m_Isinitialized = true;
}

void Logger::Release()
{
	m_Isinitialized = false;
}

void Logger::Log(LogLevel loglevel, const std::wstring & msg)
{
	std::wstringstream stream;

	//Add the correct prefix
	switch (loglevel)
	{
	case Info:
		stream << L"[INFO] ";
		break;
	case Warning:
		stream << L"[WARNING] ";
		break;
	case Error:
		stream << L"[ERROR] ";
		break;
	case Fix:
		stream << L"[FIX] ";
		break;
	default:
		stream << L"[No LOG LEVEL] ";
		break;
	}

	stream << msg << std::endl;

	//Output the message
	std::wcout << stream.str();
}

void Logger::LogInfo(const std::wstring & msg)
{
	Log(LogLevel::Info, msg);
}

void Logger::LogWarning(const std::wstring & msg)
{
	Log(LogLevel::Warning, msg);
}

void Logger::LogError(const std::wstring & msg)
{
	Log(LogLevel::Error, msg);
}

void Logger::LogFix(const std::wstring & msg)
{
	Log(LogLevel::Fix, msg);
}

void Logger::LogGLEnumInfo(GLenum info)
{
	std::wstringstream stream;
	stream << reinterpret_cast< char const * >(glGetString(info));
	Log(LogLevel::Info, stream.str());
}

void Logger::LogGLEnumError(GLenum result)
{
	std::wstringstream stream;
	stream << reinterpret_cast< char const * >(glewGetErrorString(result));
	Log(LogLevel::Error,  stream.str() );
}
