#pragma once

#include <optional>
#include <stop_token>
#include <condition_variable> 
#include <mutex>
#include <cstddef>
#include <queue>

template <typename T>
class BufferQueue {
    public:
        explicit BufferQueue(std::size_t capacity = 100) : capacity(capacity) {}
        ~BufferQueue() = default;

        bool tryPush(T element)
        {   
            {   
            std::lock_guard<std::mutex> lock(mutex);
            if (container.size() >= capacity){
                return false;
            }
            container.push(std::move(element));
            }
            
            elementsAvailable.notify_one();
            return true;

        }

        std::optional<T> pop(std::stop_token stopToken)
        {
            std::unique_lock<std::mutex> lock(mutex);

            elementsAvailable.wait(lock,stopToken,[this] 
            {
                return !container.empty();
            });
            
            if (container.empty()) {
                return std::nullopt; 
            }

            T returnElement = std::move(container.front());
            container.pop();

            return returnElement;
        }

    private:
        std::size_t capacity; 
        std::queue<T> container;
        std::mutex mutex;
        std::condition_variable_any elementsAvailable;
}; 