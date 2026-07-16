cmake_minimum_required(VERSION 4.2)
project(rsmc-example C)

set(CMAKE_C_STANDARD 11)

include(FetchContent)
FetchContent_Declare(rsmc
        GIT_REPOSITORY https://github.com/01eksa/rsmc.git
        GIT_TAG v0.1.0
)
FetchContent_MakeAvailable(rsmc)

add_executable(rsmc_example main.c)
target_link_libraries(rsmc_example PRIVATE rsmc)
