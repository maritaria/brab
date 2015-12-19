#ifndef GARRYSMOD_LUA_ILUATABLE_H
#define GARRYSMOD_LUA_ILUATABLE_H

#include <vector>

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaObject;

		struct LuaKeyValue
		{
			ILuaObject* key;
			ILuaObject* value;
		};

		typedef std::vector<LuaKeyValue> ILuaVector;
	}
}

#endif