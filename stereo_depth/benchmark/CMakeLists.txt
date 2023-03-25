include_directories(test)

set(BENCHMARK
    benchmark
)

foreach(BE ${BENCHMARK})
    add_executable(${BE} ${BE}.cu)
    target_link_libraries(${BE} stereodepth)
    target_link_libraries(${BE} stdc++fs)
endforeach()