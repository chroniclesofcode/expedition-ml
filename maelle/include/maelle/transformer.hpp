#pragma once

#include <string>

#include "lune/mlp.hpp"

namespace maelle {

class Transformer {
public:
    Transformer(int d_model, int n_layers) : d_model_(d_model), n_layers_(n_layers) {}

    std::string describe() const
    {
        // Toy use of a lune primitive so the cross-folder import is exercised.
        lune::MLP ffn(d_model_, d_model_ * 4);
        return "maelle::Transformer(d=" + std::to_string(d_model_) +
            ", layers=" + std::to_string(n_layers_) +
            ", ffn=" + ffn.describe() + ")";
    }

private:
    int d_model_;
    int n_layers_;
};

}  // namespace maelle
