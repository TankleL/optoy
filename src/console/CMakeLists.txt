project(console)

add_executable(console
    main.cpp
)

target_link_libraries(console PRIVATE optoy::liboptoy)

set_property(TARGET console PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )
