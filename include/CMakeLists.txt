# vim: set filetype=cmake tabstop=4 shiftwidth=4 expandtab :

add_library(zoidbol INTERFACE)

set(zoidbol_HEADERS
    zoidbol/CommandLineError.hpp
    zoidbol/CommandLineOption.hpp
    zoidbol/CommandLineParser.hpp
    zoidbol/DebugStream.hpp)

target_include_directories(zoidbol INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

install(FILES
    ${zoidbol_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/zoidbol)

install(TARGETS zoidbol
    EXPORT zoidbolTargets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/zoidbol"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
