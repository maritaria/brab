#ifndef GARRYSMOD_LUA_LUASTATE_H
#define GARRYSMOD_LUA_LUASTATE_H

#include "ILuaBase.h"

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaBase;

		struct LuaState
		{
			unsigned char	_ignore_this_common_lua_header_[ 69 ];
			ILuaBase*		luabase;
		};
	}
}

#endif