#pragma once

#include "lune/model.hpp"
#include <cstddef>

namespace maelle
{

struct TrainingConfig
{
    double learning_rate = 0.01;
    size_t epochs = 100;
    size_t batch_size = 32;
};

class Trainer
{
public:
    Trainer(lune::Model m) : m_{m} {}
private:
    lune::Model m_;
};

}