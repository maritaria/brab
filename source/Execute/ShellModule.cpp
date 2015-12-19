#include "ShellModule.h"

#include "ShellCommand.h"
#include "ShellStatus.h"

using namespace GarrysMod::Lua;

namespace Execute
{
	ShellModule::ShellModule( ILuaInterface* lua )
		: System::Module( lua, "Shell" )
	{
		ShellCommand::CreateMeta( lua );
		ILuaObject* moduleTable = GetLuaTable();
		SetupShellStatusTable( moduleTable );

		moduleTable->SetMember( "DirectShellCommand", DirectShellCommand );
		lua->SetGlobal( GetName(), moduleTable );
	}

	ShellModule::~ShellModule()
	{
	}

	void ShellModule::Shutdown()
	{
	}

	LUA_FUNCTION( ShellModule::DirectShellCommand )
	{
		#if _WIN32
		#define POPEN  _popen
		#define PCLOSE _pclose
		#else
		#define POPEN  popen
		#define PCLOSE pclose
		#endif

		#define LINE_BUFFER_SIZE 512

		std::string shell = Lua()->GetStringOrError( 1 );

		FILE* pipe = POPEN( shell.c_str(), "r" );

		if ( !pipe )
		{
			Lua()->Error( "Unable to open pipe" );
			return 0;
		}

		char buffer[ LINE_BUFFER_SIZE ];
		std::string result = "";

		while ( fgets( buffer, LINE_BUFFER_SIZE, pipe ) )
		{
			result += buffer;
		}

		PCLOSE( pipe );
		Lua()->Push( result.c_str() );

		return 1;
	}

	void ShellModule::SetupShellStatusTable( ILuaObject* moduleTable )
	{
		ILuaObject* table = GetLua()->GetNewTable();
		table->SetMember( "Completed", (double) ShellStatus::Completed );
		table->SetMember( "Running", (double) ShellStatus::Running );
		table->SetMember( "StartupFailure", (double) ShellStatus::StartupFailure );
		moduleTable->SetMember( "ShellStatus", table );
		delete table;
	}
}