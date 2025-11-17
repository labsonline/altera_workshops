cmake_minimum_required(VERSION 3.14)

add_subdirectory(../OneWare_bsp OneWare_bsp)

include(../OneWare_bsp/toolchain.cmake)

project(OneWare_app)

enable_language(ASM)
enable_language(C)
enable_language(CXX)

add_executable(OneWare_app.elf)

target_sources(OneWare_app.elf
    PRIVATE
        niosv_mira220.c
)

target_include_directories(OneWare_app.elf
    PRIVATE
    PUBLIC
)

target_link_libraries(OneWare_app.elf
    PRIVATE
        -T "${BspLinkerScript}" -nostdlib
        "${ExtraArchiveLibraries}"
        -Wl,--start-group "${BspLibraryName}" -lc -lstdc++ -lgcc -lm -Wl,--end-group
)

# Create objdump from ELF.
set(objdump OneWare_app.elf.objdump)
add_custom_command(
    OUTPUT "${objdump}"
    DEPENDS OneWare_app.elf
    COMMAND "${ToolchainObjdump}" "${ToolchainObjdumpFlags}" OneWare_app.elf >
            "${objdump}"
    COMMENT "Creating ${objdump}."
    VERBATIM
)
add_custom_target(create-objdump ALL DEPENDS "${objdump}")

# Report space free for stack + heap. Note that the file below is never created
# so the report is always output on build.
set(stack_report_file OneWare_app.elf.stack_report)
add_custom_command(
    OUTPUT "${stack_report_file}"
    DEPENDS OneWare_app.elf
    COMMAND niosv-stack-report -p "${ToolchainPrefix}" OneWare_app.elf
    COMMENT "Reporting memory available for stack + heap in OneWare_app.elf."
    VERBATIM
)
add_custom_target(niosv-stack-report ALL DEPENDS "${stack_report_file}")

# Generate HEX file(s) from OneWare_app.elf using elf2hex tool.
# Note : If ECC Full is enabled, width of 39 is set for NiosV TCM. Otherwise, 32.
add_custom_command(
    OUTPUT "ocm.hex"
    DEPENDS OneWare_app.elf
    COMMAND elf2hex OneWare_app.elf -o ocm.hex -b 0x00000000 -w 32 -e 0x0000FFFF -r 4
    COMMENT "Creating ocm.hex."
    VERBATIM
)
add_custom_target(create-hex ALL DEPENDS "ocm.hex")
