cmake_minimum_required(VERSION 3.5)

project(GradeManager)

#解决奇数个数中文在msvc上无法编译utf-8不带bom的代码文件
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

#visual studio热重载
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    set(CMAKE_CXX_FLAGS_DEBUG "/MDd /ZI /Ob0 /Od /RTC1") #启用热重载
endif()
set(CMAKE_CXX_STANDARD 20)

set(FTXUI_BUILD_EXAMPLES OFF)

set(SQLITECPP_RUN_CPPLINT OFF)
#set(CMAKE_INSTALL_PREFIX D:\\temp\\out) #输出路径


add_subdirectory(external/ftxui)
add_subdirectory(external/SQLiteCpp)
set(ZLIB_ROOT ${CMAKE_SOURCE_DIR}/external/zlib) #zlib
add_subdirectory(external/libxlsxwriter)
add_subdirectory(external/nativefiledialog-extended)

file(GLOB SRC ./src/*.cpp) #遍历所有cpp文件
file(GLOB INC ./src/*.hpp) #遍历所有cpp文件g


add_executable(GradeManager ${SRC} ${INC})


target_link_libraries(GradeManager
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component # Not needed for this example.
        PRIVATE SQLiteCpp
        PRIVATE sqlite3
        PRIVATE nfd
        PRIVATE xlsxwriter
)