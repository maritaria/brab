#ifndef CUSTOMTYPES_H
#define CUSTOMTYPES_H

#include "GarrysMod/Lua/Type.h"

namespace CustomTypes
{
	enum
	{
		Module = GarrysMod::Lua::Type::COUNT + 120,
		MyObject,
		ShellCommand,
		COUNT,
	};
}

#endif