# verso -- the research / experimentation playground.
#
# Each experiment is its own executable. To add a new experiment:
#   1. Drop the source at  src/your_experiment.cpp  (must define main()).
#   2. Append an add_executable + target_link_libraries block below.
# Link maelle when you want full models, lune when you only need
# primitives. Linking maelle transitively brings lune along.

add_executable(train_transformer src/train_transformer.cpp)
target_link_libraries(train_transformer PRIVATE maelle lune)
