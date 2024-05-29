cmake_minimum_required(VERSION 3.10)

project(task-manager)

set(CMAKE_CXX_STANDARD 11)

# Find necessary libraries
find_package(Threads REQUIRED)
find_package(IPC REQUIRED)

# Set executable name
set(EXECUTABLE_NAME task-manager)

# Add source files
add_executable(${EXECUTABLE_NAME} src/main.cpp)

# Link libraries
target_link_libraries(${EXECUTABLE_NAME} Threads::Threads IPC::IPC)

# Generate build files
if(MSVC)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
endif()