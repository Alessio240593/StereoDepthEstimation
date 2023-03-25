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
    test_seq_par_32x32
    test_seq_par_64x64
    test_seq_par_128x128
    test_seq_par_256x256
    test_seq_par_512x512
    #test_seq_par_1024x1024
    #test_seq_par_2048x2048
)

foreach(TEST ${UNIT_TESTS_SEQ_PAR})
    add_executable(${TEST} ${TEST}.cu)
    add_test(${TEST} ${TEST})
    target_link_libraries(${TEST} stereodepth)
endforeach()

add_dependencies(tests ${UNIT_TESTS_SEQ_PAR})