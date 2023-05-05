cmake_minimum_required(VERSION 3.0)

project(SM9)

# 设置头文件路径
set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${INCLUDE})

# 设置编译选项
set(CMAKE_C_FLAGS "-O2")

add_subdirectory(src)

set(TEST ON)

# 测试
if(TEST)
    add_subdirectory(test)
endif()

# 安装
# ...