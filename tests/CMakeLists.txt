# vim: set filetype=cmake tabstop=4 shiftwidth=4 expandtab :

if (BUILD_TESTING)

    add_executable(help_test help_test.cpp)
    target_link_libraries(help_test zoidbol)
    add_test(help_short help_test -h)
    add_test(help_long help_test --help)
    add_test(help_dos help_test /?)
    add_test(help_slash help_test /help)

    add_executable(values_test values_test.cpp)
    target_link_libraries(values_test zoidbol)
    add_test(values_bool_long values_test --boolean)
    add_test(values_bool_short values_test -b)
    add_test(values_string_long values_test --string ctest)
    add_test(values_string_long_equals values_test --string=equals)
    add_test(values_string_short values_test -s ctest)
    add_test(values_composite values_test -bs foo --optional bar)
    add_test(values_short_nospace values_test -sctest)


    add_executable(bad_test bad_test.cpp)
    target_link_libraries(bad_test zoidbol)
    add_test(bad_test bad_test --option-required)
    add_test(bad_test_short bad_test -o)
    add_test(bad_test_short2 bad_test -o -notoptions)

    add_executable(example example.cpp)
    target_link_libraries(example zoidbol)

endif()
