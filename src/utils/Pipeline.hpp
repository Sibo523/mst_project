#pragma once
#include <vector>
#include <functional>
#include <memory>

class Pipeline {
public:
    void addStage(std::function<std::shared_ptr<void>(std::shared_ptr<void>)> stage);
    
    template<typename T>
    T process(std::function<std::shared_ptr<void>()> inputGenerator) {
        std::shared_ptr<void> data = inputGenerator();
        for (const auto& stage : stages) {
            data = stage(data);
        }
        return *std::static_pointer_cast<T>(data);
    }

private:
    std::vector<std::function<std::shared_ptr<void>(std::shared_ptr<void>)>> stages;
};

