#include "ILuaObject.h"
#include "UserData.h"
#include "Type.h"
#include <sstream>

namespace GarrysMod
{
	namespace Lua
	{
		ILuaObject::ILuaObject( ILuaBase* lua, int iRef ) : m_pLua( lua ), m_iRef( iRef )
		{
		}

		ILuaObject::ILuaObject( ILuaBase* lua, ILuaObject* obj ) : m_pLua( lua ), m_iRef( obj->m_iRef )
		{
		}

		ILuaObject::~ILuaObject()
		{
			m_pLua->ReferenceFree( m_iRef );
		}

		void ILuaObject::Set( ILuaObject* obj ) // ???
		{
			if ( m_iRef )
				m_pLua->ReferenceFree( m_iRef );

			m_pLua->ReferencePush( obj->m_iRef );
			m_iRef = m_pLua->ReferenceCreate();
		}

		void ILuaObject::SetNil() // ???
		{
			m_pLua->PushNil();
			SetFromStack( -1 );
		}

		void ILuaObject::SetFromStack( int i )
		{
			if ( m_iRef )
				m_pLua->ReferenceFree( m_iRef );

			m_pLua->Push( i );
			m_iRef = m_pLua->ReferenceCreate();
		}

		int ILuaObject::GetReference()
		{
			return m_iRef;
		}

		bool ILuaObject::IsReferenceValid()
		{
			return ( m_iRef != -1 );
		}

		int ILuaObject::GetType()
		{
			Push(); // +1
			int ret = m_pLua->GetType( -1 );
			m_pLua->Pop(); // -1
			return ret;
		}

		const char* ILuaObject::GetTypeName()
		{
			Push(); // +1
			const char* ret = m_pLua->GetTypeName( m_pLua->GetType( -1 ) );
			m_pLua->Pop(); // -1
			return ret;
		}

		const char* ILuaObject::GetString( unsigned int* iLen )
		{
			Push(); // +1
			const char* ret = m_pLua->GetString( -1, iLen );
			m_pLua->Pop(); // -1
			return ret;
		}

		int ILuaObject::GetInt( void )
		{
			Push(); // +1
			int ret = (int) m_pLua->GetNumber( -1 );
			m_pLua->Pop(); // -1
			return ret;
		}

		double ILuaObject::GetDouble( void )
		{
			Push(); // +1
			double ret = m_pLua->GetNumber( -1 );
			m_pLua->Pop(); // -1
			return ret;
		}

		float ILuaObject::GetFloat( void )
		{
			Push(); // +1
			float ret = (float) m_pLua->GetNumber( -1 );
			m_pLua->Pop(); // -1
			return ret;
		}

		bool ILuaObject::GetBool( void )
		{
			Push(); // +1
			bool ret = m_pLua->GetBool( -1 );
			m_pLua->Pop(); // -1
			return ret;
		}

		void* ILuaObject::GetUserData( void )
		{
			Push(); // +1
			UserData* data = (UserData*) m_pLua->GetUserdata( -1 );
			m_pLua->Pop(); // -1
			return data->data;
		}

		ILuaVector* ILuaObject::GetMembers()
		{
			Push();

			if ( m_pLua->GetType( -1 ) != Type::TABLE )
			{
				m_pLua->ThrowError( "ILuaObject::GetMembers, object not a table!" );
				return nullptr;
			}

			ILuaVector* tableMembers = new ILuaVector();

			m_pLua->PushNil();
			while ( m_pLua->Next( -2 ) != 0 )
			{
				LuaKeyValue keyValues;

				int iValue = m_pLua->ReferenceCreate();
				int iKey = m_pLua->ReferenceCreate();

				keyValues.value = new ILuaObject( m_pLua, iValue ); // -1
				keyValues.key = new ILuaObject( m_pLua, iKey ); // -2

				tableMembers->push_back( keyValues );

				m_pLua->ReferencePush( iKey ); // Push key back for next loop
			}

			m_pLua->Pop();

			return tableMembers;
		}

		void ILuaObject::SetMember( const char* name )
		{
			Push(); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushNil(); // +1
			m_pLua->SetTable( -3 ); // +2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( const char* name, ILuaObject* obj )
		{
			Push(); // +1
			m_pLua->PushString( name ); // +1
			obj->Push(); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( const char* name, double d )
		{
			Push(); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushNumber( d ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( const char* name, bool b )
		{
			Push(); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushBool( b ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( const char* name, const char* s, unsigned int iLen )
		{
			Push(); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushString( s, iLen ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( const char* name, LuaFunction f )
		{
			Push(); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushCFunction( f ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( double dKey )
		{
			Push(); // +1
			m_pLua->PushNumber( dKey ); // +1
			m_pLua->PushNil(); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( double dKey, ILuaObject* obj )
		{
			Push(); // +1
			m_pLua->PushNumber( dKey ); // +1
			obj->Push(); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( double dKey, double d )
		{
			Push(); // +1
			m_pLua->PushNumber( dKey ); // +1
			m_pLua->PushNumber( d ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( double dKey, bool b )
		{
			Push(); // +1
			m_pLua->PushNumber( dKey ); // +1
			m_pLua->PushBool( b ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( double dKey, const char* s, unsigned int iLen )
		{
			Push(); // +1
			m_pLua->PushNumber( dKey ); // +1
			m_pLua->PushString( s, iLen ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( double dKey, LuaFunction f )
		{
			Push(); // +1
			m_pLua->PushNumber( dKey ); // +1
			m_pLua->PushCFunction( f ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMember( float fKey )
		{
			SetMember( (double) fKey );
		}

		void ILuaObject::SetMember( float fKey, ILuaObject* obj )
		{
			SetMember( (double) fKey, obj );
		}

		void ILuaObject::SetMember( float fKey, double d )
		{
			SetMember( (double) fKey, d );
		}

		void ILuaObject::SetMember( float fKey, bool b )
		{
			SetMember( (double) fKey, b );
		}

		void ILuaObject::SetMember( float fKey, const char* s, unsigned int iLen )
		{
			SetMember( (double) fKey, s, iLen );
		}

		void ILuaObject::SetMember( float fKey, LuaFunction f )
		{
			SetMember( (double) fKey, f );
		}

		void ILuaObject::SetMember( int iKey )
		{
			SetMember( (double) iKey );
		}

		void ILuaObject::SetMember( int iKey, ILuaObject* obj )
		{
			SetMember( (double) iKey, obj );
		}

		void ILuaObject::SetMember( int iKey, double d )
		{
			SetMember( (double) iKey, d );
		}

		void ILuaObject::SetMember( int iKey, bool b )
		{
			SetMember( (double) iKey, b );
		}

		void ILuaObject::SetMember( int iKey, const char* s, unsigned int iLen )
		{
			SetMember( (double) iKey, s, iLen );
		}

		void ILuaObject::SetMember( int iKey, LuaFunction f )
		{
			SetMember( (double) iKey, f );
		}

		ILuaObject* ILuaObject::GetMember( const char* name )
		{
			Push(); // +1
			m_pLua->GetField( -1, name ); // +1
			ILuaObject* r = new ILuaObject( m_pLua, m_pLua->ReferenceCreate() ); // -1
			m_pLua->Pop(); // -1
			return r;
		}

		ILuaObject* ILuaObject::GetMember( double dKey )
		{
			Push(); // +1
			m_pLua->PushNumber( dKey ); // +1
			m_pLua->GetTable( -2 );
			ILuaObject* r = new ILuaObject( m_pLua, m_pLua->ReferenceCreate() ); // -1
			m_pLua->Pop(); // -1
			return r;
		}

		ILuaObject* ILuaObject::GetMember( float fKey )
		{
			return GetMember( (double) fKey );
		}

		ILuaObject* ILuaObject::GetMember( int iKey )
		{
			return GetMember( (double) iKey );
		}

		ILuaObject* ILuaObject::GetMember( ILuaObject* oKey )
		{
			Push(); // +1
			oKey->Push(); // +1
			m_pLua->GetTable( -2 ); // -2 AND +1
			ILuaObject* r = new ILuaObject( m_pLua, m_pLua->ReferenceCreate() ); // -1
			m_pLua->Pop(); // -1
			return r;
		}

		bool ILuaObject::GetMemberBool( const char* name, bool b )
		{
			Push(); // +1
			m_pLua->GetField( -1, name ); // +1
			bool r = ( m_pLua->GetType( -1 ) != Type::NIL ) ? m_pLua->GetBool( -1 ) : b;
			m_pLua->Pop( 2 ); // -2
			return r;
		}

		int ILuaObject::GetMemberInt( const char* name, int i )
		{
			return (int) GetMemberDouble( name, (int) i );
		}

		float ILuaObject::GetMemberFloat( const char* name, float f )
		{
			return (float) GetMemberDouble( name, (double) f );
		}

		double ILuaObject::GetMemberDouble( const char* name, double d )
		{
			Push(); // +1
			m_pLua->GetField( -1, name ); // +1
			double r = ( m_pLua->GetType( -1 ) != Type::NIL ) ? m_pLua->GetNumber( -1 ) : d;
			m_pLua->Pop( 2 ); // -2
			return r;
		}

		const char* ILuaObject::GetMemberStr( const char* name, const char* s, unsigned int* iLen )
		{
			Push(); // +1
			m_pLua->GetField( -1, name ); // +1
			const char* r = ( m_pLua->GetType( -1 ) != Type::NIL ) ? m_pLua->GetString( -1, iLen ) : s;
			m_pLua->Pop( 2 ); // -2
			return r;
		}

		void* ILuaObject::GetMemberUserData( const char* name, void* u )
		{
			Push(); // +1
			m_pLua->GetField( -1, name ); // +1
			void* r = ( m_pLua->GetType( -1 ) != Type::NIL ) ? ( (UserData*) m_pLua->GetUserdata( -1 ) )->data : u;
			m_pLua->Pop( 2 ); // -2
			return r;
		}

		void ILuaObject::SetUserData( void* obj, unsigned char type )
		{
			Push(); // +1
			UserData *data = (UserData*) m_pLua->GetUserdata();
			data->data = obj;
			data->type = type;
			m_pLua->Pop(); // -1
		}

		void ILuaObject::SetMemberUserDataLite( const char* name, void* pData )
		{
			Push(); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushUserdata( pData ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void* ILuaObject::GetMemberUserDataLite( const char* name, void* u )
		{
			Push(); // +1
			m_pLua->GetField( -1, name ); // +1
			void* r = ( m_pLua->GetType( -1 ) != Type::NIL ) ? m_pLua->GetUserdata( -1 ) : u;
			m_pLua->Pop( 2 ); // -2
			return r;
		}

		bool ILuaObject::IsType( int iType )
		{
			Push(); // +1
			bool ret = m_pLua->IsType( -1, iType );
			m_pLua->Pop(); // -1
			return ret;
		}

		bool ILuaObject::IsNil()
		{
			return IsType( Type::NIL );
		}

		bool ILuaObject::IsTable()
		{
			return IsType( Type::TABLE );
		}

		bool ILuaObject::IsString()
		{
			return IsType( Type::STRING );
		}

		bool ILuaObject::IsNumber()
		{
			return IsType( Type::NUMBER );
		}

		bool ILuaObject::IsFunction()
		{
			return IsType( Type::FUNCTION );
		}

		bool ILuaObject::IsUserData()
		{
			Push(); // +1
			int iType = m_pLua->GetType( -1 );
			m_pLua->Pop(); // -1

			return iType == Type::USERDATA || iType > Type::COUNT;
		}

		bool ILuaObject::IsInvalid()
		{
			return IsType( Type::INVALID );
		}

		void ILuaObject::Push()
		{
			m_pLua->ReferencePush( m_iRef );
		}
	};
};