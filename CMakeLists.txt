cmake_minimum_required(VERSION 3.10)
project(minesweeper)

# generate symbols for IDE indexer
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-3.0 glibmm-2.4)

#main program
add_executable(main
    ${CMAKE_CURRENT_SOURCE_DIR}/src/logic.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
target_link_libraries(main PRIVATE ${GTKMM_LIBRARIES} ${OpenCV_LIBS})
target_include_directories(main
    PRIVATE ${GTKMM_INCLUDE_DIRS}
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_directories(main PRIVATE ${GTKMM_LIBRARY_DIRS})