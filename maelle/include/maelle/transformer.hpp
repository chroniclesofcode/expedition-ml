#pragma once

#include <string>

namespace maelle {

class Transformer {
public:
    Transformer(int d_model, int n_layers);

    std::string describe() const;

private:
    int d_model_;
    int n_layers_;
};

}  // namespace maelle
