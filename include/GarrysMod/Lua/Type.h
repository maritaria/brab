#ifndef GARRYSMOD_LUA_TYPES_H
#define GARRYSMOD_LUA_TYPES_H

namespace GarrysMod 
{
	namespace Lua
	{
		namespace Type
		{
			enum
			{
				INVALID = -1,
				NIL, 
				BOOL,
				LIGHTUSERDATA,
				NUMBER, 
				STRING, 
				TABLE,
				FUNCTION,
				USERDATA,
				THREAD,

				// UserData
				ENTITY, 
				VECTOR, 
				ANGLE,
				PHYSOBJ,
				SAVE,
				RESTORE,
				DAMAGEINFO,
				EFFECTDATA,
				MOVEDATA,
				RECIPIENTFILTER,
				USERCMD,
				SCRIPTEDVEHICLE,

				// Client Only
				MATERIAL,
				PANEL,
				PARTICLE,
				PARTICLEEMITTER,
				TEXTURE,
				USERMSG,

				CONVAR,
				IMESH,
				MATRIX,
				SOUND,
				PIXELVISHANDLE,
				DLIGHT,
				VIDEO,
				FILE,

				COUNT
			};
		}
	}
}

#endif 

