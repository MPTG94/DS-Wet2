cmake_minimum_required(VERSION 3.15)
project(wet2)


set(GCC_COVERAGE_COMPILE_FLAGS "-Wall")
# need to check which exact compiler flags will be used (PDF only states DNDEBUG and Wall
#set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Werror -pedantic-errors")
set(CMAKE_CXX_STANDARD 11)
# might need these
#set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_executable(wet2 main2.cpp library2.h library2.cpp library2.cpp MusicManager.cpp MusicManager.h HashTable.h List.h ThreeParamKey.cpp ThreeParamKey.h Tree.h RankTree.h Artist.cpp Artist.h Song.cpp Song.h TwoParamKey.cpp TwoParamKey.h RankTreeSingle.h RankTreeExceptions.h)