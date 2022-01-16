find_package(GTest CONFIG REQUIRED)
add_executable(optoytests
    io-tests.cpp
    ray-tests.cpp
    shape-tests.cpp
    threadpool-tests.cpp
)

target_link_libraries(optoytests PRIVATE GTest::gtest GTest::gtest_main optoy::liboptoy)
include(GoogleTest)
gtest_discover_tests(optoytests WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_property(TARGET optoytests PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )
