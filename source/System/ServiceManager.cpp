#include "ServiceManager.h"

namespace System
{
	ServiceManager* ServiceManager::GetInstance()
	{
		if ( !s_Instance )
		{
			s_Instance = new ServiceManager();
		}
		return s_Instance;
	}

	void ServiceManager::RegisterService( IService* service )
	{
		m_Services.insert( service );
	}

	void ServiceManager::ShutdownServices()
	{
		for ( auto service : m_Services )
		{
			service->Shutdown();
		}
	}

	void ServiceManager::ShutdownManager()
	{
		delete this;
	}

	ServiceManager::ServiceManager()
	{
	}

	ServiceManager::~ServiceManager()
	{
		ShutdownServices();
		DeleteServices();
	}

	void ServiceManager::DeleteServices()
	{
		while ( !m_Services.empty() )
		{
			delete *m_Services.begin();
		}
	}
	ServiceManager* ServiceManager::s_Instance;
}