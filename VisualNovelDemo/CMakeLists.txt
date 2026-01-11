cmake_minimum_required(VERSION 3.15)
project(VisualNovelDemo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找依赖
find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)

# 包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

# 源文件
set(SOURCES
    src/main.cpp
    src/VisualNovelEngine.cpp
    src/DialogueSystem.cpp
    src/CharacterRenderer.cpp
    src/ScriptInterpreter.cpp
)

# 可执行文件
add_executable(VisualNovelDemo ${SOURCES})

# 链接库
target_link_libraries(VisualNovelDemo
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
)

# 复制数据文件
file(COPY data DESTINATION ${CMAKE_BINARY_DIR})
