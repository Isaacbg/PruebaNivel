#pragma once

#include <ByteProcess/ModuleDataTypes.hpp>


class IModule {
	public:
	
	virtual ~IModule() = default;
	
	virtual OperationResult start() = 0;
	virtual OperationResult stop() = 0;
	
	virtual ConfigurationResult setOutTarget(IModule* target) {return ConfigurationResult::NotAllowed;}
	
	virtual OperationResult tryReceive(ByteArray data) { return OperationResult::NotSupported; }
    
};