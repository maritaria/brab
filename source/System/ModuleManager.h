#ifndef SYSTEM_MODULEMANAGER_H
#define SYSTEM_MODULEMANAGER_H

#include "IService.h"
#include "Module.h"
#include <set>

namespace System
{
	class ModuleManager : public IService
	{
	public:
		void RegisterModule( Module* module );
		void Shutdown();
		static ModuleManager* GetInstance();
	private:
		ModuleManager();
		~ModuleManager();

		std::set<Module*> m_Modules;
		static ModuleManager* s_Instance;

	};
}

#endif