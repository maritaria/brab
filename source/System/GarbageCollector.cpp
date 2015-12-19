#include "GarbageCollector.h"
#include "GarrysMod/Lua/ILuaModuleManager.h"
#include "LuaClass.h"
#include "ServiceManager.h"

namespace System
{
	using namespace GarrysMod::Lua;

	GarbageCollector* GarbageCollector::GetInstance()
	{
		if ( !s_Instance )
		{
			s_Instance = new GarbageCollector();
		}
		return s_Instance;
	}

	void GarbageCollector::Shutdown()
	{
		while ( !m_Objects.empty() )
		{
			delete *m_Objects.begin();
		}
	}

	void GarbageCollector::AddReference( LuaClassBase* luaObject )
	{
		m_Objects.insert( luaObject );
	}

	void GarbageCollector::RemoveReference( LuaClassBase* luaObject )
	{
		std::set<LuaClassBase*>::iterator it = m_Objects.find( luaObject );
		if ( it != m_Objects.end() )
		{
			m_Objects.erase( it );
		}
	}

	GarbageCollector::GarbageCollector() : m_Objects()
	{
		ServiceManager::GetInstance()->RegisterService( this );
	}

	GarbageCollector::~GarbageCollector()
	{
		IService::~IService();
		Shutdown();
	}

	GarbageCollector* GarbageCollector::s_Instance;
}