set(EXAMPLES
    main_cpu
)

foreach(EX ${EXAMPLES})
    add_executable(${EX} ${EX}.cpp)
    target_link_libraries(${EX} stereodepth)
endforeach()

add_executable(main_gpu main_gpu.cu)
target_link_libraries(main_gpu stereodepth)

add_executable(main_gpu_shared main_gpu_shared.cu)
target_link_libraries(main_gpu_shared stereodepth)