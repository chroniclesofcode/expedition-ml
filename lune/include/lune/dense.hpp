#pragma once

#include <cstddef>

#include "lune/activation.hpp"

namespace lune
{


class Dense : public Layer
{
public:
    Dense(size_t id, size_t od) : Layer(id, od) {}
    
    virtual Tensor forward(const Tensor& input)
    {

    }
};


}