#include "lune/mlp.hpp"
#include "maelle/transformer.hpp"

#include <iostream>

int main() {
    // Pull a primitive straight from lune.
    lune::MLP probe(16, 32);

    // Pull a full model from maelle (which internally uses lune).
    maelle::Transformer model(128, 6);

    std::cout << "hello from verso\n";
    std::cout << "  lune primitive: " << probe.describe() << "\n";
    std::cout << "  maelle model:   " << model.describe() << "\n";
    return 0;
}
