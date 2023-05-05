macro(ADD_MODULE MODULE)
	SET(CMAKE_C_FLAGS "-O2")
	add_executable(test_${MODULE} test_${MODULE}.c)
	target_link_libraries(test_${MODULE} ${PROJECT_NAME})
endmacro(ADD_MODULE)

ADD_MODULE(example)