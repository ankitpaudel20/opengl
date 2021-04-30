cmake_minimum_required(VERSION 3.16)
project(opengl_core)

set(CMAKE_CXX_STANDARD 17)

add_executable(Application
        Application/src/vendor/include/glm/detail/glm.cpp
        Application/src/vendor/glad/glad.c
        Application/src/vendor/include/stb_image/stb_image.cpp
        Application/src/vendor/include/imgui/imgui.cpp
        Application/src/vendor/include/imgui/imgui_draw.cpp
        Application/src/vendor/include/imgui/imgui_impl_glfw.cpp
        Application/src/vendor/include/imgui/imgui_impl_opengl3.cpp
        Application/src/vendor/include/imgui/imgui_widgets.cpp
        Application/src/application3.cpp
)

target_include_directories(Application PRIVATE Application/src/include)
target_include_directories(Application PRIVATE Application/src/vendor/include)


target_link_directories(Application PRIVATE Application/libs/glfw/)
target_link_directories(Application PRIVATE Application/libs/assimp/Debug/)
target_link_directories(Application PRIVATE Application/libs/zlib/debug/)
target_link_directories(Application PRIVATE Application/libs/assimp/Release/)
target_link_directories(Application PRIVATE Application/libs/zlib/release/)


if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_link_libraries(Application PRIVATE glfw3.lib)
    target_link_libraries( Application PRIVATE debug glfw3.lib assimp-vc142-mtd.lib assimpd.lib zlibstaticd.lib)
    #target_link_libraries( Application PRIVATE optimized glfw3.lib assimp-vc142-mt.lib assimp.lib zlibstatic.lib)
    message("Linked to windows using MSVC")

    add_custom_command(TARGET Application POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/Application/libs/assimp/Debug/assimpd.exe"      # <--this is in-file
        ${PROJECT_BINARY_DIR})                 # <--this is out-file path

          add_custom_command(TARGET Application POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${PROJECT_SOURCE_DIR}/Application/libs/assimp/Release/assimp.exe"      # <--this is in-file
        ${PROJECT_BINARY_DIR})                 # <--this is out-file path
    
        message("current binary directory ${PROJECT_SOURCE_DIR}")

else()
    if (CMAKE_SYSTEM_NAME STREQUAL Linux)
        # find_package(ASSIMP REQUIRED)
        # target_include_directories(Application PRIVATE ${ASSIMP_INCLUDE_DIR})
        # target_link_libraries(Application PRIVATE libglfw3_linux.a pthread dl GL assimp z zip m)       
        target_link_libraries(Application PRIVATE libglfw3_linux.a pthread dl GL assimp z X11)       

        message("Linked to Linux")

    elseif (CMAKE_SYSTEM_NAME STREQUAL Windows)
        # find_package(ASSIMP REQUIRED)
        # target_include_directories(Application PRIVATE ${ASSIMP_INCLUDE_DIR})
        target_link_libraries(Application PRIVATE libglfw3_windows.a libassimp)
        message("Linked to windows")
    endif ()
endif()




#add_custom_command(
#        TARGET Application POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy
#        ${CMAKE_SOURCE_DIR}/Application/libs/assimp/libassimp.so.5.0.0
#        ${CMAKE_CURRENT_BINARY_DIR}/libassimp.so.5.0.0)

