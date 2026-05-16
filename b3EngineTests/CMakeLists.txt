cmake_minimum_required(VERSION 4.1)

project(b3EngineTests VERSION 1.0 LANGUAGES CXX)
add_executable(b3EngineTests 
  test1.cpp
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_compile_definitions(${PROJECT_NAME} PUBLIC $<$<CONFIG:Debug>:DEBUG>)
if (MSVC)
  target_compile_options(${PROJECT_NAME} PUBLIC /utf-8)
  target_compile_options(${PROJECT_NAME} PUBLIC /W4)
else()
  target_compile_options(${PROJECT_NAME} PUBLIC 
    -Wall -Wextra -Wpedantic 
    -Wno-unused-parameter 
    -Wno-nullability-extension 
    -Wno-missing-designated-field-initializers
  )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE b3Engine)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.15.0
)
FetchContent_MakeAvailable(Catch2)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)