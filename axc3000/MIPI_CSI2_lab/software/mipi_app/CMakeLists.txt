cmake_minimum_required(VERSION 3.14)

add_subdirectory(../mipi_bsp mipi_bsp)

include(../mipi_bsp/toolchain.cmake)

project(mipi_app)

enable_language(ASM)
enable_language(C)
enable_language(CXX)

add_executable(mipi_app.elf)

target_sources(mipi_app.elf
    PRIVATE
        niosv_mipi_mira220.c
)

target_include_directories(mipi_app.elf
    PRIVATE
    PUBLIC
)

target_link_libraries(mipi_app.elf
    PRIVATE
        -T "${BspLinkerScript}" -nostdlib
        "${ExtraArchiveLibraries}"
        -Wl,--start-group "${BspLibraryName}" -lc -lstdc++ -lgcc -lm -Wl,--end-group
)

# Create objdump from ELF.
set(objdump mipi_app.elf.objdump)
add_custom_command(
    OUTPUT "${objdump}"
    DEPENDS mipi_app.elf
    COMMAND "${ToolchainObjdump}" "${ToolchainObjdumpFlags}" mipi_app.elf >
            "${objdump}"
    COMMENT "Creating ${objdump}."
    VERBATIM
)
add_custom_target(create-objdump ALL DEPENDS "${objdump}")

# Report space free for stack + heap. Note that the file below is never created
# so the report is always output on build.
set(stack_report_file mipi_app.elf.stack_report)
add_custom_command(
    OUTPUT "${stack_report_file}"
    DEPENDS mipi_app.elf
    COMMAND niosv-stack-report -p "${ToolchainPrefix}" mipi_app.elf
    COMMENT "Reporting memory available for stack + heap in mipi_app.elf."
    VERBATIM
)
add_custom_target(niosv-stack-report ALL DEPENDS "${stack_report_file}")

# Generate HEX file(s) from mipi_app.elf using elf2hex tool.
# Note : If ECC Full is enabled, width of 39 is set for NiosV TCM. Otherwise, 32.
add_custom_command(
    OUTPUT "ocm.hex"
    DEPENDS mipi_app.elf
    COMMAND elf2hex mipi_app.elf -o ocm.hex -b 0x00000000 -w 32 -e 0x0000FFFF -r 4
    COMMENT "Creating ocm.hex."
    VERBATIM
)
add_custom_command(
    OUTPUT "vid_frame_buf.hex"
    DEPENDS mipi_app.elf
    COMMAND elf2hex mipi_app.elf -o vid_frame_buf.hex -b 0x00080000 -w 32 -e 0x000AFFFF -r 4
    COMMENT "Creating vid_frame_buf.hex."
    VERBATIM
)
add_custom_target(create-hex ALL DEPENDS "vid_frame_buf.hex" "ocm.hex")
