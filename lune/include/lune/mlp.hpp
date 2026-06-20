#pragma once

#include <string>

namespace lune {

class MLP {
public:
    MLP(int in_dim, int out_dim) : in_dim_{in_dim}, out_dim_{out_dim} {}

    std::string describe() const
    {
        return "lune::MLP(" + std::to_string(in_dim_) + " -> " + std::to_string(out_dim_) + ")";
    }

private:
    int in_dim_;
    int out_dim_;
};

}  // namespace lune
