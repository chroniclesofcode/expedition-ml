#pragma once

#include <string>
#include <vector>
#include <memory>

#include "lune/layer.hpp"
#include "lune/model.hpp"
#include "lune/tensor.hpp"

namespace lune {

class MLP {
public:
    MLP(std::vector<std::unique_ptr<Layer>> layers) : layers_{std::move(layers)} {}

    Tensor forward_pass(const Tensor& input)
    {
        return Tensor{};
    }

    std::string describe() const
    {
        return "describing lune lol";
    }

private:
    std::vector<std::unique_ptr<Layer>> layers_;
};

}  // namespace lune
