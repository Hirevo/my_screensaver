cmake_minimum_required(VERSION 3.0)
project(my_screensaver C)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated -W -Wall -Wextra -O3 -march=native -mtune=native")
# -O3 -march=native -mtune=native
add_subdirectory(lib/vec)
find_package(SFML REQUIRED graphics window system)
link_directories(/usr/local/lib)

add_executable(my_screensaver
    ${PROJECT_SOURCE_DIR}/src/draw.c
    ${PROJECT_SOURCE_DIR}/src/utils.c
    ${PROJECT_SOURCE_DIR}/src/window.c
    ${PROJECT_SOURCE_DIR}/src/lights/mod.c
    ${PROJECT_SOURCE_DIR}/src/trailing/mod.c
    ${PROJECT_SOURCE_DIR}/src/blurred_trailing/mod.c
    ${PROJECT_SOURCE_DIR}/src/meta_gpu/mod.c
    ${PROJECT_SOURCE_DIR}/src/meta/mod.c
)
target_link_libraries(my_screensaver ${SFML_LIBRARIES} m csfml-graphics csfml-window vec)
target_include_directories(my_screensaver PRIVATE ${PROJECT_SOURCE_DIR}/include ${SFML_INCLUDE_DIR} /usr/local/include)
