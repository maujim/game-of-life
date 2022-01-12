add_executable(game-of-life main.cpp)

set_target_properties(
    game-of-life
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    CXX_EXTENSIONS OFF
)

target_compile_features(
    game-of-life
    PUBLIC
    cxx_std_17
)
