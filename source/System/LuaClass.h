#ifndef SYSTEM_LUACLASS_H
#define SYSTEM_LUACLASS_H

#include "GarrysMod/Lua/ILuaModuleManager.h"

namespace System
{
	class LuaClassBase
	{
	public:
		LuaClassBase();
		virtual ~LuaClassBase();
	};

	template<typename Class, unsigned UID>
	class LuaClass : public LuaClassBase
	{
	public:
		enum { TypeID = UID };

		static void CreateMeta(GarrysMod::Lua::ILuaInterface* lua)
		{
			ILuaObject* meta = lua->GetMetaTable( typeid( Class ).name(), UID );
			Class::FillMetatable(meta);
			delete meta;
		}

	protected:
		template<int ( Class::*Callback )( GarrysMod::Lua::LuaState* L )>
		static void BindMethod( GarrysMod::Lua::ILuaObject* metatable, const char* name )
		{
			metatable->SetMember( name, WrappedLuaMethod<Callback>::invoke );
		}

	private:
		template<int ( Class::*Callback )( GarrysMod::Lua::LuaState* L )>
		class WrappedLuaMethod
		{
		public:
			static LUA_FUNCTION( invoke )
			{
				if ( !Lua()->AssertArgument( 1, TypeID ) )
					return 0;

				Class* instance = static_cast<Class*>( Lua()->GetUserData( 1 ) );
				return (instance->*Callback)( L );
			}
		};

	};
}

#endif
