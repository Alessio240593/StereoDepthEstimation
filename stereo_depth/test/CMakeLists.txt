include_directories(.)

list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

#TEST VERSIONE SEQUENZIALE
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

# TEST VERSIONE SEQUENZIALE CON COPIA, SEQUENZIALE SENZA COPIA E PARALLELA
set(UNIT_TESTS_SEQ_PAR
    test_seq_par
)

foreach(TEST ${UNIT_TESTS_SEQ_PAR})
    add_executable(${TEST} ${TEST}.cu)
    add_test(${TEST} ${TEST})
    target_link_libraries(${TEST} stereodepth)
endforeach()

add_dependencies(tests ${UNIT_TESTS_SEQ_PAR})
