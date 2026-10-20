add_executable(byte_process
    main.cpp
    ModuleFactory.cpp
    Modules/GeneratorModule.cpp
    Modules/FilterModule.cpp
    Modules/LoggerModule.cpp
)

target_include_directories(byte_process PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/commons
)


find_package(Threads REQUIRED)
target_link_libraries(byte_process PRIVATE Threads::Threads)