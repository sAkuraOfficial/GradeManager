cmake_minimum_required(VERSION 3.5)

project(GradeManager)

add_subdirectory(external/ftxui)

add_executable(GradeManager src/main.cpp)

target_link_libraries(GradeManager
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)