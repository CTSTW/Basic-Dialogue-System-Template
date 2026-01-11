cmake_minimum_required(VERSION 3.15)
project(DialogueSystem VERSION 1.0.0 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 设置Release模式的优化选项
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(/O2 /GL)
    add_link_options(/LTCG)
endif()

# 查找SDL2（如果需要）
find_package(SDL2 QUIET)
if(SDL2_FOUND)
    message(STATUS "SDL2 found: ${SDL2_INCLUDE_DIRS}")
    include_directories(${SDL2_INCLUDE_DIRS})
endif()

# 添加所有根目录的cpp文件作为独立目标
file(GLOB ROOT_CPP_FILES "*.cpp")

foreach(CPP_FILE ${ROOT_CPP_FILES})
    # 获取文件名（不带扩展名）
    get_filename_component(TARGET_NAME ${CPP_FILE} NAME_WE)
    
    # 跳过CMakeLists.txt文件
    if(NOT ${TARGET_NAME} STREQUAL "CMakeLists")
        message(STATUS "添加目标: ${TARGET_NAME}")
        
        # 创建可执行文件
        add_executable(${TARGET_NAME} ${CPP_FILE})
        
        # 链接库
        if(SDL2_FOUND)
            target_link_libraries(${TARGET_NAME} ${SDL2_LIBRARIES})
        endif()
        
        # 设置输出文件名
        set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME ${TARGET_NAME})
        
        # Windows特定设置
        if(WIN32)
            target_link_libraries(${TARGET_NAME} kernel32 user32 gdi32 winspool shell32 ole32 oleaut32 uuid comdlg32 advapi32)
        endif()
    endif()
endforeach()

# 添加VisualNovelDemo项目（如果存在VisualNovelDemo目录）
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/VisualNovelDemo")
    message(STATUS "发现VisualNovelDemo目录")
    
    # 添加VisualNovelDemo的子目录
    add_subdirectory(VisualNovelDemo)
endif()

# 安装目标（可选）
install(TARGETS ${ALL_TARGETS}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 安装配置文件
install(FILES README.md LICENSE
    DESTINATION .
)
