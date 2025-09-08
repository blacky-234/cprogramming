cmake_minimum_required(VERSION 3.20)
project(week1 VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic MOC, UIC, and RCC (for .ui and resources later)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Tell compiler to search inside "include/" folder for headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect source files
set(SOURCES
    src/main.cpp
    src/app.cpp
    src/utils.cpp
)

# Create executable
add_executable(MyQtApp ${SOURCES})


target_link_libraries(weekone PRIVATE Qt6::Widgets)