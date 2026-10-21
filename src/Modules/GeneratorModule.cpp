#include <random>
#include <utility>

#include <Modules/GeneratorModule.hpp>

GeneratorModule::~GeneratorModule(){
    stop();
}

OperationResult GeneratorModule::start() {
    if (isRunning || workerThread.joinable()) {
        return OperationResult::InvalidOperation;
    }

    if(outTarget == nullptr) {
        return OperationResult::BadConfiguration;
    }

    
    workerThread = std::jthread([this](std::stop_token stopToken) {
        threadCallback(stopToken);
    });
    isRunning = true;

    return OperationResult::Correct;
}

OperationResult GeneratorModule::stop() {
    if (!workerThread.joinable()) {
        isRunning = false;
        return OperationResult::Correct;
    }

    workerThread.request_stop();
    workerThread.join();

    isRunning = false;

    return OperationResult::Correct;
}

ConfigurationResult GeneratorModule::setOutTarget(IModule* target){
    if (isRunning) {
        return ConfigurationResult::NotAllowed;
    }

    outTarget = target;
    return ConfigurationResult::Applied;

}

ConfigurationResult GeneratorModule::setGenerationInterval(std::chrono::milliseconds value){
    if (isRunning) {
        return ConfigurationResult::NotAllowed;
    }

    if (value.count() <= 0) {
        return ConfigurationResult::NotAllowed;
    }

    generationInterval=value;
    return ConfigurationResult::Applied;
}


void GeneratorModule::threadCallback(std::stop_token stopToken){
    std::mt19937 randomNumberGenerator(std::random_device{}());
    std::uniform_int_distribution<std::size_t> randomSizeDistribution(1, 100);
    std::uniform_int_distribution<int> randomByteDistribution(0, 255);

    while (!stopToken.stop_requested()){

        const std::size_t size = randomSizeDistribution(randomNumberGenerator);
        ByteArray dataArray(size);
        
        for (auto& byte: dataArray){
            byte = static_cast<std::uint8_t>(randomByteDistribution(randomNumberGenerator));
        }
        
        const OperationResult deliveryResult = outTarget->tryReceive(std::move(dataArray));

        if (deliveryResult != OperationResult::Correct) {
            //tratamiento según el tipo de resultado 
            //rechazado por buffer
            //rechazado por módulo inactivo
        }

        std::unique_lock<std::mutex> lock(sleepMutex);
        cvSleep.wait_for(lock,stopToken,generationInterval, [] {return false;});
        
    }

}