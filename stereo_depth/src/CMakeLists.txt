set(MAIN
    main
)

find_package(CUDA  REQUIRED)
include_directories("${CUDA_INCLUDE_DIRS}")
enable_language(CUDA)

foreach(FILE ${MAIN})
    add_executable(${FILE} ${FILE}.cu)
    target_link_libraries(${FILE} ${CUDA_LIBRARIES})
endforeach()
