cmake_minimum_required(VERSION 4.1)
include(FetchContent)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

project(b3Engine)

add_library(${PROJECT_NAME}
  src/b3/b3.hpp src/b3/b3.cpp
  src/b3/engine.hpp src/b3/engine.cpp
  src/b3/common.hpp src/b3/common.cpp
  src/b3/types.hpp src/b3/types.cpp
  src/b3/mesh.hpp src/b3/mesh.cpp
  src/b3/texture.hpp src/b3/texture.cpp
  src/b3/node.hpp src/b3/node.cpp
  src/b3/camera.hpp src/b3/camera.cpp
  src/b3/frustum_culling.hpp src/b3/frustum_culling.cpp

  src/b3/primitives/CubeMesh.hpp src/b3/primitives/CubeMesh.cpp
  src/b3/primitives/PlaneMesh.hpp src/b3/primitives/PlaneMesh.cpp
  src/b3/primitives/SphereMesh.hpp src/b3/primitives/SphereMesh.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:DEBUG>)
target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_USE_STD_FORMAT)
if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /utf-8)
    target_compile_options(${PROJECT_NAME} PUBLIC /W4)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC 
      -Wall -Wextra -Wpedantic 
      -Wno-unused-parameter 
      -Wno-nullability-extension
      -Wno-missing-designated-field-initializers)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Vulkan
find_package(Vulkan REQUIRED COMPONENTS glslc)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${Vulkan_INCLUDE_DIR})
# target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES})

# glfw
if(APPLE)
  enable_language(OBJC)
endif(APPLE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4)
FetchContent_MakeAvailable(glfw)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${glfw_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

# spdlog
set(SPDLOG_USE_STD_FORMAT ON CACHE BOOL "" FORCE)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.17.0)
FetchContent_MakeAvailable(spdlog)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${spdlog_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog)

# glm
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.3)
FetchContent_MakeAvailable(glm)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${glm_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC glm::glm)

# vk-bootstrap
FetchContent_Declare(
  vk_bootstrap
  GIT_REPOSITORY https://github.com/charles-lunarg/vk-bootstrap
  GIT_TAG v1.4.350 
)
FetchContent_MakeAvailable(vk_bootstrap)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${vk_bootstrap_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PUBLIC vk-bootstrap::vk-bootstrap)

# Vulkan Memory Allocator
FetchContent_Declare(
  vma
  GIT_REPOSITORY https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator.git
  GIT_TAG v3.3.0 
)
FetchContent_MakeAvailable(vma)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${vma_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC VulkanMemoryAllocator)

# volk
FetchContent_Declare(
  volk
  GIT_REPOSITORY https://github.com/zeux/volk.git
  GIT_TAG vulkan-sdk-1.4.350.0 
)
FetchContent_MakeAvailable(volk)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${volk_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC volk::volk)

# stb
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG 31c1ad37456438565541f4919958214b6e762fb4
)
FetchContent_MakeAvailable(stb)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${stb_SOURCE_DIR})

# json
FetchContent_Declare(
  json 
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(json)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)

# fastgltf
FetchContent_Declare(
  fastgltf
  GIT_REPOSITORY https://github.com/spnda/fastgltf.git
  GIT_TAG v0.9.0
)
FetchContent_MakeAvailable(fastgltf)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${fastgltf_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC fastgltf)

# imgui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.92.8
)
FetchContent_MakeAvailable(imgui)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${imgui_SOURCE_DIR})

# miniaudio
FetchContent_Declare(
  miniaudio
  GIT_REPOSITORY https://github.com/mackron/miniaudio.git
  GIT_TAG 0.11.25)
FetchContent_MakeAvailable(miniaudio)
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${miniaudio_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC miniaudio)

# shaders

function(compile_shaders name)
  set(GLSLC ${Vulkan_GLSLC_EXECUTABLE})
  set(GLSLC_OPTIONS -g --target-env=vulkan1.3)

  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/shaders/${name}.vert.spv 
    COMMAND ${GLSLC} ${GLSLC_OPTIONS} ${CMAKE_CURRENT_SOURCE_DIR}/shaders/${name}.vert -o ${CMAKE_BINARY_DIR}/shaders/${name}.vert.spv
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/${name}.vert
  )
  add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/shaders/${name}.frag.spv 
    COMMAND ${GLSLC} ${GLSLC_OPTIONS} ${CMAKE_CURRENT_SOURCE_DIR}/shaders/${name}.frag -o ${CMAKE_BINARY_DIR}/shaders/${name}.frag.spv
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shaders/${name}.frag
  )
  add_custom_target(
    compile_${name}
    DEPENDS ${CMAKE_BINARY_DIR}/shaders/${name}.vert.spv
            ${CMAKE_BINARY_DIR}/shaders/${name}.frag.spv
  )
  add_dependencies(${CMAKE_PROJECT_NAME} compile_${name})
endfunction()

compile_shaders("scene")
compile_shaders("shadow")
