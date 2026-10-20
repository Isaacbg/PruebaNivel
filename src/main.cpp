#include <chrono>
#include <memory>
#include <thread>

#include <ByteProcess/IModule.hpp>
#include <ByteProcess/ModuleFactory.hpp>
#include <ByteProcess/ModuleDataTypes.hpp>


int main(void) { 
    //Create modules 
    std::unique_ptr<IModule> m1 = ModuleFactory::createGeneratorModule(std::chrono::milliseconds(10));
    std::unique_ptr<IModule> m2 = ModuleFactory::createFilterModule({0xFF, 0xAA});
    std::unique_ptr<IModule> m3 = ModuleFactory::createLoggerModule();
    
    m1->setOutTarget(m2.get());
    m2->setOutTarget(m3.get());
    
    
    if (m3->start() != OperationResult::Correct){
            return -1;
        }

    if (m2->start() != OperationResult::Correct){
            return -1;
        }
        
    if (m1->start() != OperationResult::Correct){
            return -1;
        }


    std::this_thread::sleep_for(std::chrono::seconds(100));
    

    if (m1->stop() != OperationResult::Correct){
            return -1;
        }

    if (m2->stop() != OperationResult::Correct){
            return -1;
        }
        
    if (m3->stop() != OperationResult::Correct){
            return -1;
        }

    m1.reset();
    m2.reset();
    m3.reset();

    return 0; 
} 