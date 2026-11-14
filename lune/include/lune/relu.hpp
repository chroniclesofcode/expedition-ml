#pragma once

#include "lune/activation.hpp"
#include <cstddef>

namespace lune
{


class ReLU : public Activation
{
public:
    void tmp() {}
private:
    size_t value_;
};


}