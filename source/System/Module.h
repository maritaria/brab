#ifndef SYSTEM_MODULE_H
#define SYSTEM_MODULE_H

#include "IService.h"
#include "../CustomTypes.h"
#include "GarrysMod/Lua/ILuaModuleManager.h"

#define LUA_BINDING_FUNCTION( _function_ ) void _function_( GarrysMod::Lua::LuaState* L )
#define LUA_INIT_BINDINGS( _function_ ) _function_( L )

namespace System
{
	class SubModule;

	class Module : public IService
	{
	public:
		Module( GarrysMod::Lua::ILuaInterface* lua, const char* name );
		virtual ~Module();

		const char*					GetName();
		virtual void				Shutdown() = 0;

	protected:
		GarrysMod::Lua::ILuaInterface* GetLua();
		GarrysMod::Lua::ILuaObject* GetLuaTable();
		
	private:
		GarrysMod::Lua::ILuaInterface*	m_Lua;
		const char*						m_Name;
		GarrysMod::Lua::ILuaObject*		m_Table;
	};
}


#endif