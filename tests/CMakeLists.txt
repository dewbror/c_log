# Specify minimum cmake version
cmake_minimum_required(VERSION 3.5)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# GLOB all the test source files
file(GLOB_RECURSE TEST_SRC ${CMAKE_CURRENT_SOURCE_DIR}/*.c)

# Add executables
add_executable(${PROJECT_NAME} ${TEST_SRC})

# Compiler-specific flags
# GCC
if(CMAKE_C_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PUBLIC 
        -pedantic 
        -Wall 
        -Wextra 
        -fmessage-length=0 
        -Wno-unused-variable 
        -Wfloat-equal 
        -Wundef 
        -Wshadow 
        -Wpointer-arith 
        -Wcast-align 
        -Wwrite-strings 
        -Wcast-qual 
        # -Wswitch-enum 
        -Wconversion 
        -Wunreachable-code 
        -Wunused-macros 
        -Wduplicated-cond 
        -Wduplicated-branches
        -Wvla
        -Wvector-operation-performance
        -Wsuggest-attribute=pure
        -Wsuggest-attribute=const
        -Wsuggest-attribute=noreturn
        -Wmissing-noreturn
        -Wsuggest-attribute=malloc
        -Wsuggest-attribute=format
        -Wmissing-format-attribute
        -Wsuggest-attribute=cold
        -fstack-protector-strong
        # suppress
        -Wno-format-zero-length
    )
    # Build type specific flags
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        target_compile_options(${PROJECT_NAME} PUBLIC
            -O0
            -ggdb3
        )
    elseif(CMAKE_BUILD_TYPE MATCHES "DebWithAsan")
        target_compile_options(${PROJECT_NAME} PUBLIC
            -O0
            -ggdb3
            -fsanitize=address,undefined
        )
        target_link_options(${PROJECT_NAME} PUBLIC
            -fsanitize=address,undefined
        )
    elseif(CMAKE_BUILD_TYPE MATCHES "DebWithCov")
        target_compile_options(${PROJECT_NAME} PUBLIC
            -O0
            -ggdb3
            --coverage
        )
        target_link_options(${PROJECT_NAME} PUBLIC
            --coverage
        )
    endif()

# CLANG
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PUBLIC 
        -Wall 
        -Wextra 
        -Wpedantic 
        -Wshadow 
        -Wundef
    )
    # Build type specific flags
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        target_compile_options(${PROJECT_NAME} PUBLIC
            -O0
            -g3
        )
    elseif(CMAKE_BUILD_TYPE MATCHES "DebWithAsan")
        target_compile_options(${PROJECT_NAME} PUBLIC
            -O0
            -g3
            -fsanitize=address,undefined
        )
        target_link_options(${PROJECT_NAME} PUBLIC
            -fsanitize=address,undefined
        )
    endif()

# MSVC
elseif(CMAKE_C_COMPILER_ID MATCHES "MSVC")
    target_compile_options(${PROJECT_NAME} PUBLIC 
        /W4
    )
    # Build type specific flags
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE
            /Zi
        )
    endif()
endif()

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE cmocka)
