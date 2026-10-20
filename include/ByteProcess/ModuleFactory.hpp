#pragma once

#include <chrono>
#include <memory>

#include <ByteProcess/IModule.hpp>

class ModuleFactory {
public:
    static std::unique_ptr<IModule> createGeneratorModule(
        std::chrono::milliseconds interval = std::chrono::milliseconds(100));

    static std::unique_ptr<IModule> createFilterModule(
        ByteArray targetPattern = {0x00, 0x01, 0x02});

    static std::unique_ptr<IModule> createLoggerModule();
};
