#ifndef EXECUTE_SHELLCOMMAND_H
#define EXECUTE_SHELLCOMMAND_H

#include "ShellStatus.h"
#include "../CustomTypes.h"
#include "../System/LuaClass.h"
#include "../System/Module.h"
#include "GarrysMod/Lua/ILuaModuleManager.h"
#include <string>
#include <iostream>
#include <thread>
#include <mutex>

namespace Execute
{
	class ShellCommand : public System::LuaClass<ShellCommand, CustomTypes::ShellCommand>
	{
	public:
		ShellCommand( const char* shellCommand );
		~ShellCommand();

		const char*	GetCommand();
		ShellStatus	GetStatus();
		const char*	GetOutput();

		static void FillMetatable( GarrysMod::Lua::ILuaObject* metatable );
		LUA_FUNCTION( lua_GetCommand );
		LUA_FUNCTION( lua_GetStatus );
		LUA_FUNCTION( lua_GetOutput );
	private:
		void		BufferThreadMethod();
		void		OpenPipe();
		void		HandleStartupFailure();
		void		ReadPipeBuffer();
		void		ClosePipeBuffer();
		void		SetStatus( ShellStatus status );
		std::string	m_Command;
		ShellStatus	m_Status;
		std::string	m_Output;
		FILE*		m_Pipe;
		std::thread	m_BufferThread;
		std::mutex	m_BufferMutex;
	};

	LUA_BINDING_FUNCTION( ShellCommand_Bindings );
}

#endif