#include "lune/mlp.hpp"

#include <string>

namespace lune {

MLP::MLP(int in_dim, int out_dim) : in_dim_(in_dim), out_dim_(out_dim) {}

std::string MLP::describe() const {
    return "lune::MLP(" + std::to_string(in_dim_) + " -> " +
           std::to_string(out_dim_) + ")";
}

}  // namespace lune
