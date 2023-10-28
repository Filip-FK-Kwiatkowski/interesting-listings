if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(target PRIVATE _GLIBCXX_DEBUG)
endif()
