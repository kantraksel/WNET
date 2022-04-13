cmake_minimum_required(VERSION 3.10)

project(WNET VERSION 1.0)

add_subdirectory(src)

target_include_directories(WNET
			PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
			)
