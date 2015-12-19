#include "LuaClass.h"
#include "GarbageCollector.h"

using namespace GarrysMod::Lua;

namespace System
{
	LuaClassBase::LuaClassBase()
	{
		GarbageCollector::GetInstance()->AddReference( this );
	}

	LuaClassBase::~LuaClassBase()
	{
		GarbageCollector::GetInstance()->RemoveReference( this );
	}
}