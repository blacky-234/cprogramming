cmake_minimum_required(VERSION 3.20)
project(weekone VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic MOC, UIC, and RCC (for .ui and resources later)
find_package(Qt6 CONFIG REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_AUTOMOC TRUE)
set(CMAKE_AUTORCC TRUE)
set(CMAKE_AUTOUIC TRUE)

# Tell compiler to search inside "include/" folder for headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Collect source files
set(SOURCES
    src/main.cpp
    src/app.cpp
    src/utils.cpp
)

# Create executable
add_executable(weekone ${SOURCES})


target_link_libraries(weekone PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui)