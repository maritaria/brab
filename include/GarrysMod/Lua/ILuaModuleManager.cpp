#include "ILuaModuleManager.h"

namespace GarrysMod
{
	namespace Lua
	{
		void ILuaModuleManager::CreateInterface( LuaState* state )
		{
			m_States[ state ] = new ILuaInterface( state );
		}

		void ILuaModuleManager::DestroyInterface( LuaState* state )
		{
			ILuaInterface* gLua = GetLuaInterface( state );
			m_States.erase( state );
			delete gLua;
		}

		ILuaInterface* ILuaModuleManager::GetLuaInterface( LuaState* state )
		{
			return m_States[ state ];
		}

		ILuaModuleManager* modulemanager = new ILuaModuleManager();
	}
}