set(TESTS
    test_main
)

foreach(T ${TESTS})
    add_executable(${T} ${T}.cpp)
    add_test(${T} ${T})
    target_link_libraries(${T} stereodepth)
endforeach()