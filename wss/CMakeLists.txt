cmake_minimum_required(VERSION 3.10)

# set the project name
project(wss VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

string(APPEND CMAKE_EXE_LINKER_FLAGS "-pthread")
# configure the header file
#configure_file(TutorialConfig.h.in TutorialConfig.h)

#add the executable
add_executable(wss wss.cpp)
add_executable(client sensorClient.cpp)

target_include_directories(wss PUBLIC
                           "${PROJECT_BINARY_DIR}"
                          )
