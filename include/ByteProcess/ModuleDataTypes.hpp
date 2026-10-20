#pragma once

#include <vector>
#include <cstdint>

using ByteArray = std::vector<std::uint8_t>;

enum class ConfigurationResult {
    Applied,
    NotAllowed
};

enum class OperationResult {
    Correct,
    BufferFull,
    InactiveTarget,
    NotSupported,
    InvalidOperation,
    BadConfiguration
};

