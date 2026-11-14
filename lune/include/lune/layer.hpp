#pragma once

#include <cstddef>
#include "lune/init_type.hpp"
#include "lune/tensor.hpp"

namespace lune
{
    
class Layer
{
public:
    Layer(size_t id, size_t od) : in_dim_{id}, out_dim_{od} {}

    virtual Tensor forward(const Tensor& input) = 0;

    virtual ~Layer() = default;
protected:
    size_t in_dim_;
    size_t out_dim_;


    InitType init_ = InitType::Random;
};

}