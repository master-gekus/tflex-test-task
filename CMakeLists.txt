cmake_minimum_required(VERSION 3.5)

project(test_task LANGUAGES CXX)

add_executable(${PROJECT_NAME}
	main.cpp
	app.h
	app.cpp
	color.h
	color.cpp
	canvas.h
	canvas.cpp
	)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	)

configure_file(test_colors.txt test_colors.txt COPYONLY)
