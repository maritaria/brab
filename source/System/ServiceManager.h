#ifndef SYSTEM_SERVICEMANAGER_H
#define SYSTEM_SERVICEMANAGER_H

#include "IService.h"

#include <set>

namespace System
{
	class ServiceManager
	{
	public:
		void RegisterService( IService* service );
		void ShutdownServices();
		void ShutdownManager();

		static ServiceManager* GetInstance();
	private:
		ServiceManager();
		~ServiceManager();

		void DeleteServices();

		std::set<IService*> m_Services;
		static ServiceManager* s_Instance;
	};
}

#endif