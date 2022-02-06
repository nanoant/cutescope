#
# Copyright (c) 2022 Adam Strzelecki, All rights reserved.
#
# Released under GPL v3 license.
#

cmake_minimum_required(VERSION 3.1.0)

project(Cutescope VERSION 1.0.0 LANGUAGES CXX)
set(PROJECT_DOMAIN "com.nanoant.Cutescope")
set(PROJECT_VENDOR "Adam Strzelecki")
set(PROJECT_COPYRIGHT_YEAR 2022)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)
# set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Charts REQUIRED)
find_package(libm2k REQUIRED)

include_directories(
    ${CMAKE_INSTALL_PREFIX}/include
    ${CMAKE_INSTALL_PREFIX}/lib/iio.framework/Headers
    )

link_directories(
    ${CMAKE_INSTALL_PREFIX}/lib
    )

add_executable(${PROJECT_NAME}
    MACOSX_BUNDLE
    resources/${PROJECT_NAME}.icns
    Main.cpp
    ScopeView.cpp
    ScopeView.h
    )

set_source_files_properties(resources/${PROJECT_NAME}.icns
    PROPERTIES MACOSX_PACKAGE_LOCATION "Resources"
    )

set(MACOSX_BUNDLE_INFO_STRING          "${PROJECT_NAME} ${PROJECT_VERSION}")
set(MACOSX_BUNDLE_BUNDLE_VERSION       "${PROJECT_NAME} ${PROJECT_VERSION}")
set(MACOSX_BUNDLE_LONG_VERSION_STRING  "${PROJECT_NAME} ${PROJECT_VERSION}")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")
set(MACOSX_BUNDLE_COPYRIGHT            "${PROJECT_COPYRIGHT_YEAR} ${PROJECT_VENDOR}")
set(MACOSX_BUNDLE_GUI_IDENTIFIER       "${PROJECT_DOMAIN}.${PROJECT_NAME}")
set(MACOSX_BUNDLE_BUNDLE_NAME          "${PROJECT_NAME}")
set(MACOSX_BUNDLE_ICON_FILE            ${PROJECT_NAME}.icns)

# hack-in NSPrincipalClass NSApplication for Retina support
set(MACOSX_BUNDLE_COPYRIGHT "${MACOSX_BUNDLE_COPYRIGHT}</string>")
set(MACOSX_BUNDLE_COPYRIGHT "${MACOSX_BUNDLE_COPYRIGHT}\n\t<key>NSPrincipalClass</key>")
set(MACOSX_BUNDLE_COPYRIGHT "${MACOSX_BUNDLE_COPYRIGHT}\n\t<string>NSApplication")

target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Charts libm2k)
