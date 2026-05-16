cmake_minimum_required(VERSION 4.1)
project(appLib VERSION 1.0 LANGUAGES CXX)
add_library(${PROJECT_NAME}
  app.hpp app.cpp
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
      -Wno-missing-designated-field-initializers)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE src)
