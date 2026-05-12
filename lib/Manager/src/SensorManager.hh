#pragma once

#include <string>

class SensorManager {
public:
    SensorManager() = default;
    std::string measure() const;
};
