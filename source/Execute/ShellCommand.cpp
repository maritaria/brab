#include "ShellCommand.h"

#include <stdio.h>

#if _WIN32
#define POPEN  _popen
#define PCLOSE _pclose
#else
#define POPEN  popen
#define PCLOSE pclose
#endif

#define LINE_BUFFER_SIZE 512

namespace Execute
{
	using namespace GarrysMod::Lua;

	ShellCommand::ShellCommand( const char* shellCommand )
		: m_Command( shellCommand )
		, m_Output()
		, m_Pipe( nullptr )
		, m_BufferMutex()
	{
		m_BufferThread = std::thread( &ShellCommand::BufferThreadMethod, this );
	}

	ShellCommand::~ShellCommand()
	{
		m_BufferThread.join();
	}

	const char* ShellCommand::GetCommand()
	{
		return m_Command.c_str();
	}

	ShellStatus ShellCommand::GetStatus()
	{
		m_BufferMutex.lock();
		return m_Status;
		m_BufferMutex.unlock();
	}

	const char* ShellCommand::GetOutput()
	{
		if ( GetStatus() != ShellStatus::Completed )
			return nullptr;

		return m_Output.c_str();
	}

	void ShellCommand::SetStatus( ShellStatus status )
	{
		m_BufferMutex.lock();
		m_Status = status;
		m_BufferMutex.unlock();
	}

	void ShellCommand::FillMetatable( GarrysMod::Lua::ILuaObject* metatable )
	{
		BindMethod<&lua_GetCommand>( metatable, "GetCommand" );
		BindMethod<&lua_GetStatus>( metatable, "GetStatus" );
		BindMethod<&lua_GetOutput>( metatable, "GetOutput" );
	}

	LUA_FUNCTION( ShellCommand::lua_GetCommand )
	{
		const char* command = GetCommand();
		if ( !command )
			return 0;
		Lua()->Push( command );
		return 1;
	}

	LUA_FUNCTION( ShellCommand::lua_GetStatus )
	{
		ShellStatus status = GetStatus();
		Lua()->Push( status );
		return 1;

	}

	LUA_FUNCTION( ShellCommand::lua_GetOutput )
	{
		const char* output = GetOutput();
		Lua()->Push( output );
		return 1;
	}

	void ShellCommand::BufferThreadMethod()
	{
		OpenPipe();
		if ( m_Pipe )
		{
			HandleStartupFailure();
		}
		else
		{
			ReadPipeBuffer();
			ClosePipeBuffer();
		}
	}

	void ShellCommand::OpenPipe()
	{
		m_BufferMutex.lock();
		m_Pipe = POPEN( m_Command.c_str(), "r" );
		m_BufferMutex.unlock();
	}

	void ShellCommand::HandleStartupFailure()
	{
		m_Output = std::string( "__Error: Startup failure" );
		m_Status = ShellStatus::Completed;
	}

	void ShellCommand::ReadPipeBuffer()
	{
		char buffer[ LINE_BUFFER_SIZE ];
		std::string m_Output = "";

		while ( fgets( buffer, LINE_BUFFER_SIZE, m_Pipe ) )
		{
			m_Output += buffer;
		}
	}

	void ShellCommand::ClosePipeBuffer()
	{
		m_BufferMutex.lock();
		PCLOSE( m_Pipe );
		m_Status = ShellStatus::Completed;
		m_BufferMutex.unlock();
	}





}
