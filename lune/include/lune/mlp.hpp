#pragma once

#include <string>

namespace lune {

class MLP {
public:
    MLP(int in_dim, int out_dim);

    std::string describe() const;

private:
    int in_dim_;
    int out_dim_;
};

}  // namespace lune
