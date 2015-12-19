#include "ILuaInterface.h"
#include "ILuaBase.h"
#include "Type.h"
#include "UserData.h"
#include "SpecialTable.h"
#include "LuaState.h"

#include <sstream>

namespace GarrysMod
{
	namespace Lua
	{
		ILuaInterface::ILuaInterface( LuaState* state ) : m_pState( state ), m_pLua( state->luabase )
		{
			m_pLua->PushSpecial( SpecialTable::Global);
			m_pG = new ILuaObject( m_pLua, m_pLua->ReferenceCreate() );

			m_pLua->PushSpecial( SpecialTable::Registry );
			m_pR = new ILuaObject( m_pLua, m_pLua->ReferenceCreate() );

			m_pLua->PushSpecial( SpecialTable::Environment );
			m_pE = new ILuaObject( m_pLua, m_pLua->ReferenceCreate() );

			m_pErrorNoHalt = GetGlobal( "ErrorNoHalt" );
		}

		ILuaInterface::~ILuaInterface()
		{
			delete m_pG;
			delete m_pR;
			delete m_pE;
			delete m_pErrorNoHalt;
		}

		LuaState* ILuaInterface::GetLuaState()
		{
			return m_pState;
		}

		ILuaObject* ILuaInterface::Global()
		{
			return m_pG;
		}

		ILuaObject* ILuaInterface::Registry()
		{
			return m_pR;
		}

		ILuaObject* ILuaInterface::Environment()
		{
			return m_pE;
		}

		ILuaObject* ILuaInterface::GetNewTable()
		{
			NewTable();
			return new ILuaObject( m_pLua, m_pLua->ReferenceCreate() );
		}

		void ILuaInterface::NewTable()
		{
			m_pLua->CreateTable();
		}

		ILuaObject* ILuaInterface::NewTemporaryObject()
		{
			m_pLua->PushNil();
			return new ILuaObject( m_pLua, m_pLua->ReferenceCreate() );
		}

		ILuaObject* ILuaInterface::NewUserData( ILuaObject* metaTable )
		{
			UserData *data = (UserData*) m_pLua->NewUserdata( sizeof( UserData ) );
			ILuaObject* obj = new ILuaObject( m_pLua, m_pLua->ReferenceCreate() );

			obj->Push(); // +1
			metaTable->Push(); // +1
			m_pLua->SetMetaTable( -2 ); // -1
			m_pLua->Pop(); // -1

			return obj;
		}

		void ILuaInterface::PushUserData( ILuaObject* metaTable, void * v, unsigned char type )
		{
			if ( !metaTable )
				Error( "CLuaInterface - No Metatable!\n" );

			UserData *data = (UserData*) m_pLua->NewUserdata( sizeof( UserData ) );
			data->data = v;
			data->type = type;

			int iRef = m_pLua->ReferenceCreate();

			m_pLua->ReferencePush( iRef );
			metaTable->Push(); // +1
			m_pLua->SetMetaTable( -2 ); // -1

			m_pLua->ReferenceFree( iRef );
		}

		void ILuaInterface::Error( const char* strError, ... )
		{
			char buff[ 1024 ];
			va_list argptr;
			va_start( argptr, strError );
			#ifdef _LINUX
			vsprintf( buff, strError, argptr );
			#else
			vsprintf_s( buff, strError, argptr );
			#endif
			va_end( argptr );

			m_pLua->ThrowError( buff );
		}

		void ILuaInterface::ErrorNoHalt( const char* strError, ... )
		{
			char buff[ 1024 ];
			va_list argptr;
			va_start( argptr, strError );
			#ifdef _LINUX
			vsprintf( buff, strError, argptr );
			#else
			vsprintf_s( buff, strError, argptr );
			#endif
			va_end( argptr );

			m_pErrorNoHalt->Push();
			m_pLua->PushString( strError );
			m_pLua->Call( 1, 0 );
		}

		void ILuaInterface::LuaError( const char* strError, int argument )
		{
			m_pLua->ArgError( argument, strError );
		}

		bool ILuaInterface::CheckArgument( int stackPosition, int expectedType )
		{
			int passedType = m_pLua->GetType( stackPosition );
			return ( passedType == expectedType );
		}

		bool ILuaInterface::AssertArgument( int stackPosition, int expectedType )
		{
			int passedType = m_pLua->GetType( stackPosition );
			if ( passedType == expectedType )
			{
				return true;
			}

			const char* passedTypeName = m_pLua->GetTypeName( passedType );

			if ( passedType == GarrysMod::Lua::Type::NIL )
			{
				passedTypeName = "no value";
			}
			else if ( passedTypeName == 0 )
			{
				passedTypeName = "<unknown type>";
			}

			const char* expectedTypeName = m_pLua->GetTypeName( expectedType );

			if ( expectedTypeName == 0 )
			{
				expectedTypeName = "<unknown type>";
			}

			std::stringstream errorText;
			errorText << expectedTypeName << " expected, got " << passedTypeName << "\n";
			m_pLua->ThrowError( errorText.str().c_str() );
			return false;
		}

		ILuaObject* ILuaInterface::GetGlobal( const char* name )
		{
			m_pLua->PushSpecial( SpecialTable::Global ); // +1
			m_pLua->GetField( -1, name ); // +1
			ILuaObject* o = new ILuaObject( m_pLua, m_pLua->ReferenceCreate() ); // -1
			m_pLua->Pop(); // -1
			return o;
		}

		void ILuaInterface::SetGlobal( const char* name, LuaFunction f )
		{
			m_pLua->PushSpecial( SpecialTable::Global ); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushCFunction( f ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaInterface::SetGlobal( const char* name, double d )
		{
			m_pLua->PushSpecial( SpecialTable::Global ); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushNumber( d ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaInterface::SetGlobal( const char* name, const char* str )
		{
			m_pLua->PushSpecial( SpecialTable::Global ); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushString( str ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaInterface::SetGlobal( const char* name, bool b )
		{
			m_pLua->PushSpecial( SpecialTable::Global ); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushBool( b ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaInterface::SetGlobal( const char* name, void* u )
		{
			m_pLua->PushSpecial( SpecialTable::Global ); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushUserdata( u ); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaInterface::SetGlobal( const char* name, ILuaObject* o )
		{
			m_pLua->PushSpecial( SpecialTable::Global ); // +1
			m_pLua->PushString( name ); // +1
			o->Push(); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaInterface::RemoveGlobal( const char* name )
		{
			m_pLua->PushSpecial( SpecialTable::Global ); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->PushNil(); // +1
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		void ILuaInterface::NewGlobalTable( const char* name )
		{
			m_pLua->PushSpecial( SpecialTable::Global ); // +1
			m_pLua->PushString( name ); // +1
			m_pLua->CreateTable();
			m_pLua->SetTable( -3 ); // -2
			m_pLua->Pop(); // -1
		}

		ILuaObject* ILuaInterface::GetObject( int i )
		{
			if ( i != 0 )
				m_pLua->Push( i ); // +1
			return new ILuaObject( m_pLua, m_pLua->ReferenceCreate() ); // -1
		}

		const char* ILuaInterface::GetString( int i, unsigned int* iLen )
		{
			return m_pLua->GetString( i, iLen );
		}

		int ILuaInterface::GetInteger( int i )
		{
			return (int) GetNumber( i );
		}

		double ILuaInterface::GetNumber( int i )
		{
			return m_pLua->GetNumber( i );
		}

		double ILuaInterface::GetDouble( int i )
		{
			return GetNumber( i );
		}

		float ILuaInterface::GetFloat( int i )
		{
			return (float) GetNumber( i );
		}

		bool ILuaInterface::GetBool( int i )
		{
			return m_pLua->GetBool( i );
		}

		void** ILuaInterface::GetUserDataPtr( int i )
		{
			UserData* data = (UserData*) m_pLua->GetUserdata( i );
			return &data->data; // Not sure if this is correct
		}

		void* ILuaInterface::GetUserData( int i )
		{
			UserData* data = (UserData*) m_pLua->GetUserdata( i );
			return data->data;
		}

		void ILuaInterface::GetTable( int i ) // ???
		{
			Error( "ILuaInterface::GetTable( int i ) is not implemented yet, but feel free to contribute!" );
		}

		const char* ILuaInterface::GetStringOrError( int i )
		{
			m_pLua->CheckType( i, Type::STRING );
			return m_pLua->GetString( i );
		}

		ILuaVector* ILuaInterface::GetAllTableMembers( int i )
		{
			if ( i != 0 )
			{
				m_pLua->Push( i );
			}

			if ( m_pLua->GetType( -1 ) != Type::TABLE )
			{
				m_pLua->ThrowError( "ILuaInterface::GetAllTableMembers, object not a table !" );
				return nullptr;
			}

			ILuaVector* tableMembers = new ILuaVector();

			m_pLua->PushNil();
			while ( m_pLua->Next( -2 ) != 0 )
			{
				LuaKeyValue keyValues;

				keyValues.key = GetObject( -2 );
				keyValues.value = GetObject( -1 );

				tableMembers->push_back( keyValues );

				keyValues.key->Push(); // Push key back for next loop
			}

			if ( i != 0 )
			{
				m_pLua->Pop( i );
			}

			return tableMembers;
		}

		void ILuaInterface::DeleteLuaVector( ILuaVector* pVector )
		{
			for ( unsigned int i = 0; i < pVector->size(); i++ )
			{
				LuaKeyValue& keyValues = pVector->at( i );
				ILuaObject* key = keyValues.key;
				ILuaObject* value = keyValues.value;
				delete key;
				delete value;
			}

			if ( pVector )
			{
				delete pVector;
			}
		}

		int ILuaInterface::GetReference( int i )
		{
			if ( i != 0 )
			{
				m_pLua->Push( i ); // +??
			}
			return m_pLua->ReferenceCreate();
		}

		void ILuaInterface::FreeReference( int i )
		{
			m_pLua->ReferenceFree( i );
		}

		void ILuaInterface::PushReference( int i )
		{
			m_pLua->ReferencePush( i );
		}

		void ILuaInterface::Pop( int i )
		{
			m_pLua->Pop( i );
		}

		int ILuaInterface::Top()
		{
			return m_pLua->Top();
		}

		void ILuaInterface::Push( ILuaObject* o )
		{
			o->Push();
		}

		void ILuaInterface::Push( const char* str, unsigned int iLen )
		{
			m_pLua->PushString( str, iLen );
		}

		void ILuaInterface::PushVA( const char* str, ... )
		{
			char buff[ 1024 ];
			va_list argptr;
			va_start( argptr, str );
			#ifdef _LINUX
			vsprintf( buff, str, argptr );
			#else
			vsprintf_s( buff, str, argptr );
			#endif
			va_end( argptr );
			m_pLua->PushString( buff );
		}

		void ILuaInterface::Push( double d )
		{
			m_pLua->PushNumber( d );
		}

		void ILuaInterface::Push( bool b )
		{
			m_pLua->PushBool( b );
		}

		void ILuaInterface::Push( LuaFunction f )
		{
			m_pLua->PushCFunction( f );
		}

		void ILuaInterface::Push( int i )
		{
			m_pLua->PushNumber( i );
		}

		void ILuaInterface::Push( float f )
		{
			m_pLua->PushNumber( f );
		}

		void ILuaInterface::PushLong( int i )
		{
			m_pLua->PushNumber( i );
		}

		void ILuaInterface::PushNil()
		{
			m_pLua->PushNil();
		}

		void ILuaInterface::CheckType( int i, int iType )
		{
			m_pLua->CheckType( i, iType );
		}

		int ILuaInterface::GetType( int iStackPos )
		{
			return m_pLua->GetType( iStackPos );
		}

		const char* ILuaInterface::GetTypeName( int iType )
		{
			return m_pLua->GetTypeName( iType );
		}

		const char * ILuaInterface::GetTypeNameEx( int iType )
		{
			const char* typeName = m_pLua->GetTypeName( iType );

			if ( iType == GarrysMod::Lua::Type::NIL )
			{
				typeName = "no value";
			}
			else if ( typeName == 0 )
			{
				typeName = "<unknown type>";
			}

			return typeName;
		}

		ILuaObject* ILuaInterface::GetReturn( int iNum )
		{
			return GetObject( iNum );
		}

		void ILuaInterface::Call( int args, int returns )
		{
			m_pLua->Call( args, returns );
		}

		int ILuaInterface::PCall( int args, int returns, int iErrorFunc )
		{
			return m_pLua->PCall( args, returns, iErrorFunc );
		}

		ILuaObject* ILuaInterface::GetMetaTable( const char* strName, int iType )
		{
			m_pLua->CreateMetaTableType( strName, iType );
			return new ILuaObject( m_pLua, m_pLua->ReferenceCreate() );
		}

		ILuaObject* ILuaInterface::GetMetaTable( int i )
		{
			if ( m_pLua->GetMetaTable( i ) )
				return new ILuaObject( m_pLua, m_pLua->ReferenceCreate() );
			else
				return nullptr;
		}
	};
};