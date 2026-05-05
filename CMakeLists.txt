cmake_minimum_required(VERSION 3.16)    # 限制最低CMake版本


# 指定交叉编译工具所使用的C语言编译器
set(CMAKE_C_COMPILER D:/Toolchains/msys2/mingw64/bin/gcc.exe)
# 指定交叉编译工具所使用的C++编译器
set(CMAKE_CXX_COMPILER D:/Toolchains/msys2/mingw64/bin/g++.exe)

# 项目名称
project(main)


include_directories(D:/Toolchains/msys2/mingw64/include/opencv4/opencv2)

find_library(OPENCV_CORE      opencv_core      D:/Toolchains/msys2/mingw64/lib)
find_library(OPENCV_IMGPROC   opencv_imgproc   D:/Toolchains/msys2/mingw64/lib)
find_library(OPENCV_HIGHGUI   opencv_highgui   D:/Toolchains/msys2/mingw64/lib)
find_library(OPENCV_VIDEOIO   opencv_videoio   D:/Toolchains/msys2/mingw64/lib)
find_library(OPENCV_IMGCODECS opencv_imgcodecs D:/Toolchains/msys2/mingw64/lib)

# 由源文件生成一个可执行程序
add_executable(crop )

