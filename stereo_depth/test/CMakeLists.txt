include_directories(.)

set(UNIT_TESTS
    test_math
    test_core
)

foreach(TEST ${UNIT_TESTS})
    add_executable(${TEST} ${TEST}.cpp)
    add_test(${TEST} ${TEST})
    target_link_libraries(${TEST} stereodepth)
endforeach()

add_dependencies(tests ${UNIT_TESTS})