#include "Execute.h"

#include "GarrysMod/Lua/ILuaModuleManager.h"
#include "../System/Module.h"

#include <stdio.h>
#include <string>
#include <iostream>

#if _WIN32
#define POPEN  _popen
#define PCLOSE _pclose
#else
#define POPEN  popen
#define PCLOSE pclose
#endif

#define LINE_BUFFER_SIZE 512

using namespace GarrysMod::Lua;

namespace Execute
{

	LUA_FUNCTION( Execute )
	{
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

	LUA_BINDING_FUNCTION( ExecuteModule_Bindings )
	{
		ILuaObject* os = Lua()->GetGlobal( "os" );
		if ( !os->IsInvalid() )
		{
			os->SetMember( "execute", Execute );
		}
		else
		{
			if ( os->IsNil() )
			{
				Lua()->Error( "os is nil" );
			}
			Lua()->Error( "Missing global table 'os'" );
		}
		delete os;
	}

}