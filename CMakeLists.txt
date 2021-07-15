# vim: set filetype=cmake tabstop=4 shiftwidth=4 expandtab :
cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(zoidbol
    VERSION 1.0.0
    LANGUAGES CXX)

# Developer's choice.
set(default_build_type "RelWithDebInfo")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

option(BUILD_SHARED_LIBS "Build shared libraries." ON)
option(BUILD_DOCS "Enable Doxygen where available." ON)
option(BUILD_TESTING "Enable ctest support." ON)
option(BUILD_ZIP_PACKAGE "Enable cpack ZIP generator" ON)
option(BUILD_TGZ_PACKAGE "Enable cpack TGZ generator" ON)
option(BUILD_DEB_PACKAGE "Enable cpack DEB generator when applicable" ON)
option(BUILD_RPM_PACKAGE "Enable cpack RPM generator when applicable" ON)

include(GNUInstallDirs)

# Minimum requirement is C++14.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_subdirectory(include)
add_subdirectory(tests)
add_subdirectory(cmake)

install(FILES
    README.md
    CHANGELOG.md
    DESTINATION ${CMAKE_INSTALL_DOCDIR})

if (BUILD_DOCS AND ${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.9.0")
    set(DOXYGEN_PROJECT_BRIEF "zoidbol option parser")
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "README.md")
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_STRIP_FROM_INC_PATH include)

    find_package(Doxygen)

    if (DOXYGEN_FOUND)
        doxygen_add_docs(docs
            include
            README.md
            CHANGELOG.md
            LICENSE.txt
            NOTICE
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            ALL)
            install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION ${CMAKE_INSTALL_DOCDIR})
    endif (DOXYGEN_FOUND)
endif (BUILD_DOCS AND ${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.9.0")

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
SET(CPACK_PACKAGE_CONTACT "BigBoss1964@gmail.com")
set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE.txt")
set(CPACK_RESOURCE_FILE_README "${PROJECT_SOURCE_DIR}/README.md")
set(CPACK_GENERATOR)
if (BUILD_ZIP_PACKAGE)
    list(APPEND CPACK_GENERATOR "ZIP")
endif (BUILD_ZIP_PACKAGE)
if (BUILD_TGZ_PACKAGE)
    list(APPEND CPACK_GENERATOR "TGZ")
endif (BUILD_TGZ_PACKAGE)
if (BUILD_DEB_PACKAGE)
    # Does cmake use dpkg-buildpackage, dpkg-deb, debuild, dh-make, or what?
    find_program(DPKG_PATH dpkg)
    if (DPKG_PATH)
        list(APPEND CPACK_GENERATOR "DEB")
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
    endif (DPKG_PATH)
endif (BUILD_DEB_PACKAGE)
if (BUILD_RPM_PACKAGE)
    find_program(RPMBUILD_PATH rpmbuild)
    if (RPMBUILD_PATH)
        list(APPEND CPACK_GENERATOR "RPM")
        set(CPACK_RPM_PACKAGE_DEPENDS "")
    endif (RPMBUILD_PATH)
endif (BUILD_RPM_PACKAGE)
include(CPack)

