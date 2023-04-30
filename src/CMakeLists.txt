# 把当前目录下的所有源文件放到LIB_SRCS
aux_source_directory(. LIB_SRCS)

# 生成动态链接库
add_library(${PROJECT_NAME} SHARED ${LIB_SRCS})
