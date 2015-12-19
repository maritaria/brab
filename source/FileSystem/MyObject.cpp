#include "MyObject.h"

using namespace GarrysMod::Lua;

namespace FileSystem
{
	MyObject::MyObject()
	{
	}

	MyObject::~MyObject()
	{
	}

	double MyObject::GetValue()
	{
		return m_Value;
	}

	void MyObject::SetValue( double value )
	{
		m_Value = value;
	}

	void MyObject::FillMetatable( ILuaObject* metatable )
	{
	}

}