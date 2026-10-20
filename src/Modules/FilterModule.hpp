#pragma once

#include <algorithm>
#include <mutex>
#include <stop_token>
#include <thread>

#include <commons/BufferQueue.hpp>
#include <ByteProcess/IModule.hpp>

class FilterModule : public IModule {
    public:
        FilterModule() = default;
        ~FilterModule() override;

        OperationResult start() override;
        OperationResult stop() override;
        OperationResult tryReceive(ByteArray data) override;

        ConfigurationResult setOutTarget(IModule* target) override; 
        ConfigurationResult setTargetSequence(ByteArray targetSequence); 

    private:
        void threadCallback(std::stop_token stopToken);

        bool isRunning = false;

        std::jthread workerThread;
        std::mutex stateMutex;

        IModule* outTarget = nullptr;

        ByteArray targetSequence;

        BufferQueue<ByteArray> inputBuffer{100};
};
