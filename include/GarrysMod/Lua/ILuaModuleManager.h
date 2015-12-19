#ifndef GARRYSMOD_LUA_ILUAMODULEMANAGER_H
#define GARRYSMOD_LUA_ILUAMODULEMANAGER_H

#include "LuaState.h"
#include "ILuaInterface.h"
#include <map>

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaModuleManager
		{
		public:
			void			CreateInterface( LuaState* state );
			void			DestroyInterface( LuaState* state );
			ILuaInterface*	GetLuaInterface( LuaState* state );

		private:
			std::map<LuaState*, ILuaInterface*> m_States;
		};

		extern ILuaModuleManager* modulemanager;
	}
}

#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec( dllexport )
#else
#define DLL_EXPORT extern "C" __attribute__( ( visibility("default") ) )	
#endif

#define GMOD_MODULE( _startfunction_, _closefunction_ ) \
	LUA_FUNCTION(_startfunction_); \
	LUA_FUNCTION(_closefunction_); \
	DLL_EXPORT LUA_FUNCTION(gmod13_open) \
	{ \
		GarrysMod::Lua::modulemanager->CreateInterface(L);\
		return _startfunction_(L);\
	} \
	DLL_EXPORT LUA_FUNCTION(gmod13_close) \
	{ \
		int ret = _closefunction_(L);\
		GarrysMod::Lua::modulemanager->DestroyInterface(L);\
		return ret;\
	}

#define LUA_FUNCTION( _function_ ) int _function_( GarrysMod::Lua::LuaState* L )
#define Lua() GarrysMod::Lua::modulemanager->GetLuaInterface( L )

#endif