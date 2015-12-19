#ifndef SYSTEM_ISERVICE_H
#define SYSTEM_ISERVICE_H

namespace System
{
	class IService
	{
	public:
		virtual ~IService() {}
		virtual void Shutdown() = 0;
	};
}

#endif