set(NAME "App")

include(${PROJECT_SOURCE_DIR}/cmake/StaticAnalyzers.cmake)

file(COPY App/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${NAME} App/Main.cpp)

target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(${NAME} PRIVATE cxx_std_20)
target_link_libraries(${NAME} PRIVATE project_warnings Core)
