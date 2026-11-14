#include "lune/mlp.hpp"

#include <iostream>

int main() {
    
    lune::MLP model({
        Dense(2, 4),
        ReLU(),
        Dense(4, 1),
        Sigmoid()
    });
    return 0;
}
