#ifndef SYSTEM_DELEGATE_H
#define SYSTEM_DELEGATE_H

namespace System
{
	/**
	 * specialization for member functions
	 *
	 * \tparam T            class-type of the object who's member function to call
	 * \tparam R            return type of the function that gets captured
	 * \tparam params       variadic template list for possible arguments
	 *                      of the captured function
	 */
	template<typename Class, typename Return, typename... Params>
	class Delegate
	{
	public:
		typedef Return( Class::*DelegateType )( Params... );

		Delegate( Class& instance, DelegateType method )
			: m_Instance( instance )
			, m_Function( method )
		{}

		Return operator()( Params... args ) const
		{
			return ( m_Instance.*m_Function )( args... );
		}

	private:
		Class& m_Instance;
		DelegateType m_Function;
	};
}

/* Based on:
http://blog.coldflake.com/posts/C++-delegates-on-steroids/
https://github.com/marcmo/delegates
*/

#endif