cmake_minimum_required(VERSION 3.5)

project(test_task LANGUAGES CXX)

add_executable(${PROJECT_NAME}
	main.cpp
	)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	)
