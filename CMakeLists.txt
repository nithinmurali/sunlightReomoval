cmake_minimum_required(VERSION 2.8)

project( Test )
 
include_directories("${PROJECT_BINARY_DIR}")
find_package( OpenCV REQUIRED )

add_definitions(-std=c++11)
add_definitions(-fpermissive)
add_executable( resun retinaDemo.cpp )
#add_executable( sun main.cpp sunlight_flicker_removal.cpp butterworthfilter.cpp)
target_link_libraries( resun ${OpenCV_LIBS} )
