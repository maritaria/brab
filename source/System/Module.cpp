#include "Module.h"

#include "GarbageCollector.h"
#include "GarrysMod/Lua/ILuaBase.h"
#include "GarrysMod/Lua/LuaState.h"

using namespace GarrysMod::Lua;

namespace System
{
	Module::Module( GarrysMod::Lua::ILuaInterface* lua, const char* name )
		: m_Lua( lua )
		, m_Name( name )
	{
		const char* tableName = GetName();
		ILuaObject* moduleTable = lua->GetGlobal( tableName );
		if ( !moduleTable->IsTable() )
		{
			ILuaObject* newTable = lua->GetNewTable();
			moduleTable->Set( newTable );
			delete newTable;
		}
		this->m_Table = moduleTable;

	}

	Module::~Module()
	{
		delete m_Table;
	}

	const char* Module::GetName()
	{
		return m_Name;
	}

	ILuaInterface* Module::GetLua()
	{
		return m_Lua;
	}

	ILuaObject* Module::GetLuaTable()
	{
		return m_Table;
	}
}