#ifndef GARRYSMOD_LUA_ILUAOBJECT_H
#define GARRYSMOD_LUA_ILUAOBJECT_H

#include "ILuaBase.h"
#include "ILuaTable.h"

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaObject
		{
		public:
			ILuaObject( ILuaBase* lua, int iRef );
			ILuaObject( ILuaBase* lua, ILuaObject* obj );
			~ILuaObject( void );

			void			Set( ILuaObject* obj );
			void			SetNil();
			void			SetFromStack( int i );

			int				GetReference();
			bool			IsReferenceValid();

			int				GetType();
			const char*		GetTypeName();

			const char*		GetString( unsigned int* iLen = nullptr );
			int				GetInt();
			double			GetDouble();
			float			GetFloat();
			bool			GetBool();
			void*			GetUserData();

			ILuaVector*		GetMembers();

			void			SetMember( const char* name );
			void			SetMember( const char* name, ILuaObject* obj );
			void			SetMember( const char* name, double d );
			void			SetMember( const char* name, bool b );
			void			SetMember( const char* name, const char* s, unsigned int iLen = 0 );
			void			SetMember( const char* name, LuaFunction f );

			void			SetMember( double dKey );
			void			SetMember( double dKey, ILuaObject* obj );
			void			SetMember( double dKey, double d );
			void			SetMember( double dKey, bool b );
			void			SetMember( double dKey, const char* s, unsigned int iLen = 0 );
			void			SetMember( double dKey, LuaFunction f );

			void			SetMember( float fKey );
			void			SetMember( float fKey, ILuaObject* obj );
			void			SetMember( float fKey, double d );
			void			SetMember( float fKey, bool b );
			void			SetMember( float fKey, const char* s, unsigned int iLen = 0 );
			void			SetMember( float fKey, LuaFunction f );

			void			SetMember( int iKey );
			void			SetMember( int iKey, ILuaObject* obj );
			void			SetMember( int iKey, double d );
			void			SetMember( int iKey, bool b );
			void			SetMember( int iKey, const char* s, unsigned int iLen = 0 );
			void			SetMember( int iKey, LuaFunction f );

			ILuaObject*		GetMember( const char* name );
			ILuaObject*		GetMember( double dKey );
			ILuaObject*		GetMember( float fKey );
			ILuaObject*		GetMember( int iKey );
			ILuaObject*		GetMember( ILuaObject* oKey );

			bool			GetMemberBool( const char* name, bool b = true );
			int				GetMemberInt( const char* name, int i = 0 );
			float			GetMemberFloat( const char* name, float f = 0.0f );
			double			GetMemberDouble( const char* name, double d = 0 );
			const char*		GetMemberStr( const char* name, const char* s = "", unsigned int* iLen = nullptr );
			void*			GetMemberUserData( const char* name, void* = nullptr );

			void			SetMemberUserDataLite( const char* name, void* pData );
			void*			GetMemberUserDataLite( const char* name, void* u = nullptr );

			void			SetUserData( void* obj, unsigned char type );

			bool			IsType( int iType );
			bool			IsNil();
			bool			IsTable();
			bool			IsString();
			bool			IsNumber();
			bool			IsFunction();
			bool			IsUserData();
			bool			IsInvalid();
			void			Push();

		private:
			ILuaBase* m_pLua;
			int m_iRef;
		};
	}
}


#endif