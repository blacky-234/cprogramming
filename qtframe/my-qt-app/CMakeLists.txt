cmake_minimum_required(VERSION 3.16)
project(MyQtApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable automatic MOC, UIC, and RCC (for .ui and resources later)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(myqtapp
    src/main.cpp
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
)

target_link_libraries(myqtapp PRIVATE Qt6::Widgets)
