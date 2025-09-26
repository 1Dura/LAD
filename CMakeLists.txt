cmake_minimum_required(VERSION 4.0.1)
project(LAD_APP)

option(BUILD_TESTS "Build tests" ON)

add_library(lad_lib STATIC
    src/Commands.cpp
    src/Helpers.cpp
    src/Interpretator.cpp
    src/Reciever.cpp
    include/CommandIO.h
    include/Commands.h
    include/Helpers.h
    include/Interpretator.h
    include/Reciever.h
    include/token_converter.h
    include/tokenizator.h
)

target_include_directories(lad_lib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

add_executable(LAD_APP "src/LAD_AP v.2.0.0.cpp")
target_link_libraries(LAD_APP PRIVATE lad_lib)

if(BUILD_TESTS)
    include(FetchContent)
    
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.x
    )
    
    FetchContent_MakeAvailable(googletest)
    
    add_executable(TEST_APP tests/test_token_converter.cpp)
    
    target_link_libraries(TEST_APP PRIVATE 
        lad_lib
        GTest::gtest_main
        GTest::gmock_main
    )
    
    target_include_directories(TEST_APP PRIVATE include)
    
    include(GoogleTest)
    gtest_discover_tests(TEST_APP
        TEST_PREFIX "LAD_"
        TEST_SUFFIX ""
        PROPERTIES LABELS "LAD"
    )
endif()