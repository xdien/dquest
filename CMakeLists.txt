cmake_minimum_required(VERSION 3.16)
project(dquest VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Support both Qt5 and Qt6
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Sql)

file(GLOB DQUEST_SOURCES src/dquest/*.cpp)
file(GLOB DQUEST_HEADERS src/dquest/*.h)

add_library(dquest SHARED ${DQUEST_SOURCES})
target_sources(dquest PRIVATE ${DQUEST_HEADERS})

target_compile_definitions(dquest PRIVATE DQUEST_EXPORTS)

target_include_directories(dquest
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/dquest>
        $<INSTALL_INTERFACE:include/dquest>
)

target_link_libraries(dquest PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Sql
)

# Export CMake configuration for find_package(dquest)
include(GNUInstallDirs)

install(TARGETS dquest
    EXPORT dquestTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${DQUEST_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dquest
)

install(EXPORT dquestTargets
    FILE dquestTargets.cmake
    NAMESPACE dquest::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dquest
)

# Generate dquestConfig.cmake
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/dquestConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/dquestConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dquest
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/dquestConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/dquestConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/dquestConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dquest
)
