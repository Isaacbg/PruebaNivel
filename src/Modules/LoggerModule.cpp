#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <utility>

#include <Modules/LoggerModule.hpp>

LoggerModule::~LoggerModule() {
    stop();
}

OperationResult LoggerModule::start() {
    if (isRunning || workerThread.joinable()) {
        return OperationResult::InvalidOperation;
    }

    workerThread = std::jthread([this](std::stop_token stopToken) {
        threadCallback(stopToken);
    });
    isRunning = true;

    return OperationResult::Correct;
}

OperationResult LoggerModule::stop() {
    if (!workerThread.joinable()) {
        isRunning = false;
        return OperationResult::Correct;
    }

    workerThread.request_stop();
    workerThread.join();

    isRunning = false;

    return OperationResult::Correct;
}

OperationResult LoggerModule::tryReceive(ByteArray data) {
    if (!isRunning) {
        return OperationResult::InactiveTarget;
    }

    const auto arrivalTime = std::chrono::system_clock::now();
    if (!inputBuffer.tryPush(TimeStampedArray{arrivalTime, std::move(data)})) {
        return OperationResult::BufferFull;
    }

    return OperationResult::Correct;
}

void LoggerModule::threadCallback(std::stop_token stopToken) {
    while (auto item = inputBuffer.pop(stopToken)) {
        if (history.empty() || history.back().timestamp <= item->timestamp){
            history.push_back(std::move(*item));
        } else{
            auto insertPosition = std::upper_bound(
                        history.begin(), history.end(), item->timestamp,
                        [](const auto& time, const TimeStampedArray& record) {
                            return time < record.timestamp;
                            }
                        );
            history.insert(insertPosition, std::move(*item));
        }

        for (const auto& record : history) {
            std::cout << std::format("[{:%H:%M:%S}]: ", std::chrono::floor<std::chrono::milliseconds>(record.timestamp));
            for (std::size_t i = 0; i < record.data.size(); ++i) {
                std::cout << std::format("0x{:02X}{}",
                    static_cast<unsigned int>(record.data[i]),
                    (i + 1 < record.data.size() ? " " : ""));
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}

