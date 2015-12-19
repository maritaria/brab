#include "MyObject.h"

#include "GarrysMod/Lua/ILuaModuleManager.h"

using namespace GarrysMod::Lua;

namespace FileSystem
{
	LUA_FUNCTION( MyObject_Create )
	{
		MyObject* object = new MyObject();
		ILuaObject* meta = Lua()->GetMetaTable( "MyObject", Type::COUNT + 1 );
		Lua()->PushUserData( meta, object, Type::COUNT + 1 );
		delete meta;
		return 1;
	}

	LUA_FUNCTION( MyObject_GetValue )
	{
		if ( !Lua()->AssertArgument( 1, Type::COUNT + 1 ) )
			return 0;

		ILuaObject* selfArgument = Lua()->GetObject( 1 );
		MyObject* object = static_cast<MyObject*>( selfArgument->GetUserData() );
		Lua()->Push( object->GetValue() );

		delete selfArgument;
		return 1;
	}

	LUA_FUNCTION( MyObject_SetValue )
	{
		if ( !Lua()->AssertArgument( 1, Type::COUNT + 1 ) || !Lua()->AssertArgument( 2, Type::NUMBER ) )
		{
			return 0;
		}
		ILuaObject* selfArgument = Lua()->GetObject( 1 );
		ILuaObject* valueArgument = Lua()->GetObject( 2 );

		MyObject* object = static_cast<MyObject*>( selfArgument->GetUserData() );
		double value = valueArgument->GetDouble();
		object->SetValue( value );

		delete selfArgument;
		delete valueArgument;
		return 0;
	}

	LUA_BINDING_FUNCTION( MyObject_Bindings )
	{
		Lua()->SetGlobal( "CreateMyObject", MyObject_Create );

		ILuaObject* meta = Lua()->GetMetaTable( "MyObject", Type::COUNT + 1 );
		meta->SetMember( "__index", meta );
		meta->SetMember( "GetValue", &MyObject_GetValue );
		meta->SetMember( "SetValue", &MyObject_SetValue );
		delete meta;
	}
}