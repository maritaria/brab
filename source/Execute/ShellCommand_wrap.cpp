#include "ShellCommand.h"

#include "GarrysMod/Lua/ILuaModuleManager.h"

namespace Execute
{
	using namespace GarrysMod::Lua;

	LUA_FUNCTION( ShellCommand_Create )
	{
		if ( !Lua()->AssertArgument( 1, Type::STRING ) )
			return 0;

		const char* string = Lua()->GetString( 1 );
		ShellCommand* command = new ShellCommand( string );
		ILuaObject* metaTable = Lua()->GetMetaTable( "ShellCommand", ShellCommand::TypeID );
		Lua()->PushUserData( metaTable, command, ShellCommand::TypeID );
		return 1;
	}

	LUA_FUNCTION( ShellCommand_GetStatus )
	{
		if ( !Lua()->AssertArgument( 1, ShellCommand::TypeID ) )
			return 0;

		ShellCommand* shellCommand = static_cast<ShellCommand*>( Lua()->GetUserData( 1 ) );
		ShellStatus status = shellCommand->GetStatus();
		Lua()->Push( status );
		return 1;
	}

	LUA_BINDING_FUNCTION( ShellCommand_Bindings )
	{
		Lua()->SetGlobal( "CreateMyObject", ShellCommand_Create );

		ILuaObject* meta = Lua()->GetMetaTable( "MyObject", Type::COUNT + 1 );
		meta->SetMember( "__index", meta );
		meta->SetMember( "GetStatus", &ShellCommand_GetStatus );
		delete meta;

		Lua()->SetGlobal( "SHELLSTATUS_COMPLETED", (double) ShellStatus::Completed );
		Lua()->SetGlobal( "SHELLSTATUS_RUNNING", (double) ShellStatus::Running );
		Lua()->SetGlobal( "SHELLSTATUS_STARTUPFAILURE", (double) ShellStatus::StartupFailure );
	}
}
