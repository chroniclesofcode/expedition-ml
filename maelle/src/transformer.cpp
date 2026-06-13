#include "maelle/transformer.hpp"

#include "lune/mlp.hpp"

#include <string>

namespace maelle {

Transformer::Transformer(int d_model, int n_layers)
    : d_model_(d_model), n_layers_(n_layers) {}

std::string Transformer::describe() const {
    // Toy use of a lune primitive so the cross-folder import is exercised.
    lune::MLP ffn(d_model_, d_model_ * 4);
    return "maelle::Transformer(d=" + std::to_string(d_model_) +
           ", layers=" + std::to_string(n_layers_) +
           ", ffn=" + ffn.describe() + ")";
}

}  // namespace maelle
