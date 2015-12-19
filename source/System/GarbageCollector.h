#ifndef SYSTEM_GARBAGECOLLECTOR_H
#define SYSTEM_GARBAGECOLLECTOR_H

#include "IService.h"
#include "LuaClass.h"
#include <set>

namespace System
{
	class GarbageCollector : public IService
	{
	public:
		void AddReference( LuaClassBase* luaObject );
		void RemoveReference( LuaClassBase* luaObject );
		void Shutdown();

		static GarbageCollector* GetInstance();
	private:
		GarbageCollector();
		~GarbageCollector();
		static GarbageCollector* s_Instance;
		std::set<LuaClassBase*> m_Objects;
	};
}

#endif