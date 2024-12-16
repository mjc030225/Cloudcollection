cmake_minimum_required(VERSION 3.22)

#
# User is free to modify the file as much as necessary
#

#list(APPEND CMAKE_MODULE_PATH "C:/Users/user1/Downloads/CmakeTestProject/cmake")
list(APPEND CMAKE_MODULE_PATH "{{sr:cmake_path}}")
message("Build CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})
include("cmake/gcc-arm-none-eabi.cmake")
message("Build CMAKE_MODULE_PATH: " ${CMAKE_MODULE_PATH})

# Core project settings
project(Yun_project)
enable_language(C CXX ASM)
message("Build type: " ${CMAKE_BUILD_TYPE})

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Core MCU flags, CPU type, instruction set and FPU setup
set(cpu_PARAMS 
    # Other parameters
    # -mthumb
    # -mcpu, -mfloat, -mfloat-abi, ...
)

# Sources
set(sources_SRCS)

# Include directories for all compilers
set(include_DIRS)
# message(STATUS "Include directories: ${include_DIRS}")
# Include directories for each compiler
set(include_c_DIRS)
set(include_cxx_DIRS)
set(include_asm_DIRS)

# Symbols definition for all compilers
set(symbols_SYMB)

# Symbols definition for each compiler
set(symbols_c_SYMB)
set(symbols_cxx_SYMB)
set(symbols_asm_SYMB)

# Link directories and names of libraries
set(link_DIRS)
set(link_LIBS)

# Linker script
set(linker_script_SRC)

# Compiler options
set(compiler_OPTS)

# Linker options
set(linker_OPTS)
# set(STARTUP_FILE "./Startup/*")
# # 添加启动文件到源文件列表
# set(SOURCES ${SOURCES} ${STARTUP_FILE})
# Now call generated cmake
# This will add script generated
# information to the project
include("cmake/vscode_generated.cmake")
include_directories("./Inc")
include_directories("./Drivers/FWlib/inc")
include_directories("./Drivers/FWlib/dsplib")
include_directories("./Drivers/FWlib/F1dsplib")
include_directories("./Drivers/wPeri/Inc")
# include_directories("./Startup")
# 获取所有源文件
file(GLOB_RECURSE sources_SRCS "src/*.c" "./Drivers/wPeri/Src/*.c" "./Drivers/FWlib/src/*.c" "./Startup/startup_stm32f405rgtx.s")
list(FILTER sources_SRCS EXCLUDE REGEX ".wAdc.c$")


# 打印源文件列表以进行检查
# message(STATUS "Source files: ${SOURCES}")
# 递归获取所有子文件夹
# Link directories setup
# Must be before executable is added
set(link_DIRS "./Drivers/FWlib/lib")
# message("Link directories: ${link_DIRS}")
link_directories(${CMAKE_PROJECT_NAME} ${link_DIRS})
# include_directories(${CMAKE_PROJECT_NAME}/inc)
# include_directories(${CMAKE_PROJECT_NAME}/Drivers/FWlib/inc)
# file(GLOB_RECURSE STARTUP_FILE "Startup/sta.s")
# set(STARTUP_FILE "Startup/startup_stm32f40_41xxx.s")
# message("Startup file: ${STARTUP_FILE}")
# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})
# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PUBLIC ${sources_SRCS})
# target_sources(${STARTUP_FILE})
# message(STATUS "Source files: ${sources_SRCS}")
# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${include_DIRS}
    $<$<COMPILE_LANGUAGE:C>: ${include_c_DIRS}>
    $<$<COMPILE_LANGUAGE:CXX>: ${include_cxx_DIRS}>
    $<$<COMPILE_LANGUAGE:ASM>: ${include_asm_DIRS}>
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    ${symbols_SYMB}
    $<$<COMPILE_LANGUAGE:C>: ${symbols_c_SYMB}>
    $<$<COMPILE_LANGUAGE:CXX>: ${symbols_cxx_SYMB}>
    $<$<COMPILE_LANGUAGE:ASM>: ${symbols_asm_SYMB}>

    # Configuration specific
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>: >
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME} ${link_LIBS})

# Compiler options
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    ${cpu_PARAMS}
    ${compiler_OPTS}
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:C>: >
    $<$<COMPILE_LANGUAGE:CXX>:

    # -Wno-volatile
    # -Wold-style-cast
    # -Wuseless-cast
    # -Wsuggest-override
    >
    $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp -MMD -MP>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>
)

# Linker options
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -T${linker_script_SRC}
    ${cpu_PARAMS}
    ${linker_OPTS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    -u _printf_float # STDIO float formatting support (remove if not used)
    --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -lsupc++
    -Wl,--end-group
    -Wl,-z,max-page-size=8 # Allow good software remapping across address space (with proper GCC section making)
    -Wl,--print-memory-usage
)

# Execute post-build to print size, generate hex and bin
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
)
