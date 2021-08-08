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
	output_device_factory.h
	output_device_factory.cpp
	output_device_interface.h
	output_device_interface.cpp
	output_device_console.h
	output_device_console.cpp
	)

if (WIN32)
	target_sources(${PROJECT_NAME} PRIVATE
		output_device_console_win.h
		output_device_console_win.cpp
		)
elseif(UNIX)
	target_sources(${PROJECT_NAME} PRIVATE
		output_device_console_ansi.h
		output_device_console_ansi.cpp
		)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
	)

configure_file(scripts/test_palette_pixels.txt test_palette_pixels.txt COPYONLY)
configure_file(scripts/test_palette_boxes.txt test_palette_boxes.txt COPYONLY)
configure_file(scripts/test_palette_bars.txt test_palette_bars.txt COPYONLY)
configure_file(scripts/test_lines.txt test_lines.txt COPYONLY)
configure_file(scripts/test_triangles.txt test_triangles.txt COPYONLY)
