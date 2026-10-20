#include <ByteProcess/ModuleFactory.hpp>

#include <Modules/FilterModule.hpp>
#include <Modules/GeneratorModule.hpp>
#include <Modules/LoggerModule.hpp>

std::unique_ptr<IModule> ModuleFactory::createGeneratorModule(std::chrono::milliseconds interval) {
    auto module = std::make_unique<GeneratorModule>();
    module->setGenerationInterval(interval);
    return module;
}

std::unique_ptr<IModule> ModuleFactory::createFilterModule(ByteArray targetPattern) {
    auto module = std::make_unique<FilterModule>();
    module->setTargetSequence(std::move(targetPattern));
    return module;
}

std::unique_ptr<IModule> ModuleFactory::createLoggerModule() {
    return std::make_unique<LoggerModule>();
}

