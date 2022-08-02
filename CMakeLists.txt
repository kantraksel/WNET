cmake_minimum_required(VERSION 3.10)

project(WNET)

add_subdirectory(src)

target_include_directories(WNET
			PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
			)
