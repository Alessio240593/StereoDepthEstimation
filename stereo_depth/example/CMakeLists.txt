set(EXAMPLES
    mainExample
)

foreach(EX ${EXAMPLES})
    add_executable(${EX} ${EX}.cpp)
    target_link_libraries(${EX} stereodepth)
endforeach()