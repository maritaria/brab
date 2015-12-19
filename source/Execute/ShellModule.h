#ifndef EXECUTE_SHELLMODULE_H
#define EXECUTE_SHELLMODULE_H

#include "../System/Module.h"
#include "GarrysMod/Lua/ILuaModuleManager.h"

namespace Execute
{
	class ShellModule : public System::Module
	{
	public:
		ShellModule( GarrysMod::Lua::ILuaInterface* lua );
		~ShellModule();

		void Shutdown();

		static LUA_FUNCTION(DirectShellCommand);

	private:
		void SetupShellStatusTable( GarrysMod::Lua::ILuaObject* moduleTable );
	};
}

#endif