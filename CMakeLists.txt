cmake_minimum_required(VERSION 3.27)
project(MAE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

add_executable(MAE mae.cpp
        fjsp.h
        schedule.h
        solution.cpp
        solution.h
        pr_distance.cpp
        pr_distance.h
        pr_neighborhood.cpp
        pr_neighborhood.h
        pathrelinking.cpp
        pathrelinking.h
        const.h
        ts_neighborhood.cpp
        ts_neighborhood.h
        tabu_search.cpp
        tabu_search.h
        ts_tabulist.cpp
        ts_tabulist.h
        ts_retain.cpp
        ts_retain.h
        list.cpp
        list.h
)
