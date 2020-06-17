cmake_minimum_required( VERSION 2.8 )
set (CMAKE_CXX_STANDARD 11)
project( HSV_COLOR_TESTER )
find_package( OpenCV REQUIRED )
add_executable( HSV_COLOR_TESTER ColorPicker.cpp )
target_link_libraries( HSV_COLOR_TESTER ${OpenCV_LIBS} )

