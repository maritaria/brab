#ifndef FILESYSTEM_MYOBJECT_H
#define FILESYSTEM_MYOBJECT_H

#include "GarrysMod/Lua/Type.h"
#include "GarrysMod/Lua/LuaState.h"
#include "../CustomTypes.h"
#include "../System/LuaClass.h"
#include "../System/Module.h"

namespace FileSystem
{
	using namespace GarrysMod::Lua;

	class MyObject : public System::LuaClass<MyObject, CustomTypes::MyObject>
	{
	public:
		MyObject();
		~MyObject();

		double	GetValue();
		void	SetValue(double value);

		static void FillMetatable( ILuaObject* metatable );

	private:
		double m_Value;
	};

	LUA_BINDING_FUNCTION( MyObject_Bindings );
}

#endif