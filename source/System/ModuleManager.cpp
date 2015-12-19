#include "ModuleManager.h"
#include "Module.h"

namespace System
{
	void ModuleManager::RegisterModule( Module* module )
	{
		m_Modules.insert( module );
	}

	void ModuleManager::Shutdown()
	{
		while ( !m_Modules.empty() )
		{
			delete *m_Modules.begin();
		}
	}

	ModuleManager* ModuleManager::GetInstance()
	{
		if ( !s_Instance )
		{
			s_Instance = new ModuleManager();
		}
		return s_Instance;
	}

	ModuleManager::ModuleManager() : m_Modules()
	{
	}

	ModuleManager::~ModuleManager()
	{
		Shutdown();
	}

	ModuleManager* ModuleManager::s_Instance;

}
