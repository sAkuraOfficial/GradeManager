cmake_minimum_required(VERSION 3.5)

project(GradeManager)

#解决奇数个数中文在msvc上无法编译utf-8不带bom的代码文件
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_CXX_STANDARD 17)

set(FTXUI_BUILD_EXAMPLES ON)


add_subdirectory(external/ftxui)
add_subdirectory(external/SQLiteCpp)

file(GLOB SRC ./src/*.cpp) #遍历所有cpp文件
file(GLOB INC ./src/*.hpp) #遍历所有cpp文件g


add_executable(GradeManager ${SRC} ${INC})


target_link_libraries(GradeManager
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component # Not needed for this example.
        PRIVATE SQLiteCpp
        PRIVATE sqlite3
)