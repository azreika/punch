#pragma once

#include <memory>
#include <vector>

namespace Tools {

template <class T>
std::vector<T*> toPtrVector(const std::vector<std::unique_ptr<T>>& uniqVec) {
    std::vector<T*> result;
    for (const auto& e : uniqVec) {
        result.push_back(e.get());
    }
    return result;
}

} // namespace Tools
