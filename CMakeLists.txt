cmake_minimum_required(VERSION 3.10.0)

project(RayMagic)

set(CMAKE_CXX_STANDARD 17)

### Configure output locations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(deps)

file(GLOB_RECURSE SRCS "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(RayMagic "${SRCS}" "${HEADERS}")
target_precompile_headers(RayMagic PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")
target_include_directories(RayMagic PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_definitions(RayMagic PRIVATE RM_PLATFORM_WINDOWS)
target_compile_definitions(RayMagic PRIVATE "$<$<CONFIG:DEBUG>:RM_DEBUG>")
target_compile_definitions(RayMagic PRIVATE "$<$<CONFIG:RELEASE>:RM_NDEBUG>")

target_link_libraries(RayMagic glad glfw glm imgui spdlog stb_image)

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT RayMagic)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/assets/config/imgui.ini" DESTINATION "${CMAKE_BINARY_DIR}/RayMagic")
