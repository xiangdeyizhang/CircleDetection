#1.cmake verson，指定cmake的最小版本号 
cmake_minimum_required(VERSION 2.8)

#2.project name，指定项目的名称，一般和项目的文件夹名称对应
project(vibe C CXX)

#修改成捏OpenCV路径即可
set(OpenCV_DIR "/home/xz/opencv3.4.3/opencv-3.4.3/build")
set(OpenCV_INCLUDE_DIRS "/home/xz/opencv3.4.3/opencv-3.4.3/include") 
find_package(OpenCV REQUIRED)

#3.set environment variable，设置环境变量
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

#7.1.add executable file，编译为可执行文件
add_executable(circle Main.cpp)

#8.add link library，添加工程所依赖的库
target_link_libraries(circle ${OpenCV_LIBS}  )
