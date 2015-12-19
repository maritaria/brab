#ifndef GARRYSMOD_LUA_CLUAFUNC_H
#define GARRYSMOD_LUA_CLUAFUNC_H

#include "LuaState.h"

namespace GarrysMod
{
	namespace Lua
	{
		struct LuaState;
		typedef int( *LuaFunction ) ( LuaState* L );
	}
}

#endif