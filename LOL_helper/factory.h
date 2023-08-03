#pragma once
#include <map>
#include <string>

template <typename T>
class Factory {
private:
    std::map<T, std::string> _constants;

public:
    Factory(std::map<T, std::string> constants) : _constants(constants) {}

    std::string GetFirst(T value) {
        for (const auto& pair : _constants) {
            if (pair.second == value) {
                return pair.first;
            }
        }
        return "";
    }

    std::string GetSecond(T key) {
        if (_constants.find(key) != _constants.end()) {
            return _constants[key];
        }
        return "";
    }
};
