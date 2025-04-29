cmake_minimum_required(VERSION 3.10.0)

add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(imgui)
add_subdirectory(spdlog)
add_subdirectory(stb_image)

set_target_properties(glad PROPERTIES FOLDER "Dependencies")
set_target_properties(glm PROPERTIES FOLDER "Dependencies")
set_target_properties(imgui PROPERTIES FOLDER "Dependencies")
set_target_properties(spdlog PROPERTIES FOLDER "Dependencies")
set_target_properties(stb_image PROPERTIES FOLDER "Dependencies")
