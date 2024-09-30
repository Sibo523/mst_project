#include "Pipeline.hpp"

void Pipeline::addStage(std::function<std::shared_ptr<void>(std::shared_ptr<void>)> stage)
{
    stages.push_back(stage);
}