#include <algorithm>
#include <utility>

#include <Modules/FilterModule.hpp>

FilterModule::~FilterModule() {
    stop();
}

OperationResult FilterModule::start() {
    if (isRunning || workerThread.joinable()) {
        return OperationResult::InvalidOperation;
    }

    if (outTarget == nullptr || targetSequence.empty()) {
        return OperationResult::BadConfiguration;
    }

    workerThread = std::jthread([this](std::stop_token stopToken) {
        threadCallback(stopToken);
    });
    isRunning = true;

    return OperationResult::Correct;
}

OperationResult FilterModule::stop() {
    if (!workerThread.joinable()) {
        isRunning = false;
        return OperationResult::Correct;
    }

    workerThread.request_stop();
    workerThread.join();

    isRunning = false;

    return OperationResult::Correct;
}

OperationResult FilterModule::tryReceive(ByteArray data) {
    if (!isRunning) {
        return OperationResult::InactiveTarget;
    }

    if (!inputBuffer.tryPush(std::move(data))) {
        return OperationResult::BufferFull;
    }

    return OperationResult::Correct;
}

ConfigurationResult FilterModule::setOutTarget(IModule* target) {
    if (isRunning) {
        return ConfigurationResult::NotAllowed;
    }

    outTarget = target;
    return ConfigurationResult::Applied;
}

ConfigurationResult FilterModule::setTargetSequence(ByteArray sequence) {
    if (isRunning) {
        return ConfigurationResult::NotAllowed;
    }

    if (sequence.empty() || sequence.size() > 100) {
        return ConfigurationResult::NotAllowed;
    }

    targetSequence = std::move(sequence);
    return ConfigurationResult::Applied;
}

void FilterModule::threadCallback(std::stop_token stopToken) {
    while (auto item = inputBuffer.pop(stopToken)) {
        ByteArray array = std::move(*item);

        if (targetSequence.empty() || array.size() < targetSequence.size()) {
            continue;
        }

        auto match = std::ranges::search(array, targetSequence);
        if (!match.empty()) {
            if (outTarget != nullptr) {
                const OperationResult deliveryResult = outTarget->tryReceive(std::move(array));
                if (deliveryResult != OperationResult::Correct) {
                    //gestión de casos en los q no se inserta la salida en el target
                }
            }
        }
    }
}

