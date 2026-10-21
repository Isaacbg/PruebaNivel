#pragma once

#include <chrono>
#include <stop_token>
#include <thread>
#include <vector>

#include <commons/BufferQueue.hpp>
#include <ByteProcess/IModule.hpp>

struct TimeStampedArray {
    std::chrono::system_clock::time_point timestamp;
    ByteArray data;
};

class LoggerModule : public IModule {
    public:
        LoggerModule() = default;
        ~LoggerModule() override;

        OperationResult start() override;
        OperationResult stop() override;
        OperationResult tryReceive(ByteArray data) override;

    private:
        void threadCallback(std::stop_token stopToken);

        bool isRunning = false;

        std::jthread workerThread;

        BufferQueue<TimeStampedArray> inputBuffer{100};
        std::vector<TimeStampedArray> history;
};
