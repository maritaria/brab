#include "Execute/Execute.h"
#include "Execute/ShellCommand.h"
#include "Execute/ShellModule.h"
#include "FileSystem/MyObject.h"
#include "GarrysMod/Lua/ILuaModuleManager.h"
#include "System/Module.h"
#include "System/GarbageCollector.h"
#include "System/ServiceManager.h"
#include "System/ModuleManager.h"
#include "System/IService.h"
#include <set>

using namespace GarrysMod::Lua;

void AddService( System::IService* service )
{
	System::ServiceManager::GetInstance()->RegisterService( service );
}

void AddModule( System::Module* module )
{
	System::ModuleManager::GetInstance()->RegisterModule( module );
}

LUA_FUNCTION( open )
{
	LUA_INIT_BINDINGS( FileSystem::MyObject_Bindings );
	LUA_INIT_BINDINGS( Execute::ShellCommand_Bindings );
	LUA_INIT_BINDINGS( Execute::ExecuteModule_Bindings );

	AddService( System::GarbageCollector::GetInstance() );
	AddService( System::ModuleManager::GetInstance() );
	
	AddModule( new Execute::ShellModule( Lua() ) );

	FileSystem::MyObject::CreateMeta( Lua() );

	return 0;
}

LUA_FUNCTION( close )
{
	System::ServiceManager::GetInstance()->ShutdownManager();
	return 0;
}

GMOD_MODULE( open, close )