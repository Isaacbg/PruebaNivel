#pragma once

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <stop_token>
#include <thread>


#include <ByteProcess/IModule.hpp>

class GeneratorModule : public IModule {
    public:
        GeneratorModule() = default;
        ~GeneratorModule() override;

        OperationResult start() override;
        OperationResult stop() override;

        ConfigurationResult setOutTarget(IModule* target) override; 
        ConfigurationResult setGenerationInterval(std::chrono::milliseconds value); 

    private:
        void threadCallback(std::stop_token stopToken);

        bool isRunning = false;

        std::jthread workerThread;
        std::mutex sleepMutex;
        std::condition_variable_any cvSleep;

        std::chrono::milliseconds generationInterval = std::chrono::milliseconds(100);

        IModule* outTarget = nullptr;


};