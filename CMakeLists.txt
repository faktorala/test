# 包含交叉编译工具链
#include(cross.cmake)

# 设置CMake的最低版本要求
cmake_minimum_required(VERSION 3.16)
# set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /opt/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.3-1/bin/loongarch64-linux-gnu-g++)  # 对于 C++ 编译器
set(CMAKE_C_COMPILER /opt/loongson-gnu-toolchain-8.3-x86_64-loongarch64-linux-gnu-rc1.3-1/bin/loongarch64-linux-gnu-gcc)      # 对于 C 编译器

# 定义项目名称和版本，并指定使用C和C++语言
project(test)

# 包含项目的自定义库路径
include_directories(/usr/include)
#include_directories(&{PROJECT_BINARY_DIR}/lib)
include_directories(/home/cuichan/test/lib)
# 查找源文件所在的目录
aux_source_directory(/home/cuichan/test/src DIR_SRCS)
#aux_source_directory(&{PROJECT_BINARY_DIR}/src/ DIR_SRCS)  #将源文件路径改名为  DIR_SRCS



#opencv头文件路径
include_directories(/usr/opencv-4.11.0.tar./opencv-4.11.0/install/include/opencv4)
#opencv所需的动态库文件保存到变量
find_library(OPENCV_CORE        opencv_core        /usr/opencv-4.11.0.tar./opencv-4.11.0/install/lib)
find_library(OPENCV_IMGPROC     opencv_imgproc     /usr/opencv-4.11.0.tar./opencv-4.11.0/install/lib)
find_library(OPENCV_HIGHGUI     opencv_highgui     /usr/opencv-4.11.0.tar./opencv-4.11.0/install/lib)
find_library(OPENCV_VIDEOIO     opencv_videoio     /usr/opencv-4.11.0.tar./opencv-4.11.0/install/lib)
find_library(OPENCV_IMGCODECS   opencv_imgcodecs   /usr/opencv-4.11.0.tar./opencv-4.11.0/install/lib)



# 创建可执行文件，使用项目名称作为目标名称
add_executable(test  ${DIR_SRCS})



#连接opencv动态库
target_link_libraries(test PUBLIC ${OPENCV_CORE} ${OPENCV_IMGPROC} ${OPENCV_HIGHGUI} ${OPENCV_VIDEOIO} ${OPENCV_IMGCODECS})
#连接多线程pthread库
target_link_libraries(test PUBLIC pthread)