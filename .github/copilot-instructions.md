# Altera Workshops Development Guide

This repository contains FPGA development lab guides and source code for Intel/Altera FPGA platforms, targeting the DE25-Nano development board with Agilex 5 FPGAs (porting from AXC3000/Agilex 3).

## Architecture Overview

The workshop labs are organized into specific technology domains under `axc3000/` (being ported to DE25-Nano), following a progressive complexity structure:

### Lab Progression (Beginner → Advanced)

1. **My_First_Lab/**: Basic VHDL design with PLL, counter, and reset IP components
2. **NIOSV_lab/**: RISC-V soft processor (Intel Nios V) with BSP and software development
3. **Timing_closure_lab/**: Multi-clock domain designs and timing constraint management
4. **SignalTap_lab/**: FPGA debugging using SignalTap logic analyzer
5. **Power_estimation_lab/**: Power analysis and optimization techniques
6. **OneWare_AI_lab/**: AI inference using OneWare Studio with GUI interfaces and system console integration
7. **FPGA_AI_Suite_lab/**: Advanced AI/ML acceleration using Intel FPGA AI Suite with OpenVINO
8. **MIPI_CSI2_lab/**: Camera interface and video processing pipeline

### Complexity Levels

- **Foundational (Labs 1-2)**: Basic FPGA concepts, IP integration, processor fundamentals
- **Intermediate (Labs 3-5)**: Timing analysis, debugging methodologies, power optimization
- **Advanced (Labs 6-8)**: AI acceleration, computer vision, complex system integration

## Key Development Workflows

### Quartus FPGA Build Process

Projects follow standard Quartus Prime workflow:
1. **Project Setup**: `.qpf` (project file) and `.qsf` (settings file) define project structure
2. **Compilation Flow**: `quartus_syn` → `quartus_fit` → `quartus_sta` → `quartus_asm`
3. **Output**: `.sof` bitstream files for FPGA programming

Example compilation (from `generate_sof.tcl`):
```tcl
qexec "quartus_syn --read_settings_files=off --write_settings_files=off $project_name -c $revision_name"
qexec "quartus_fit --read_settings_files=on --write_settings_files=off $project_name -c $revision_name" 
qexec "quartus_sta $project_name -c $revision_name --mode=finalize --do_report_timing"
qexec "quartus_asm --read_settings_files=on --write_settings_files=off $project_name -c $revision_name"
```

### AI/ML Development Pattern

FPGA AI Suite labs use a specific workflow:
1. **Environment Setup**: Virtual environment with PyTorch, ONNX, OpenVINO dependencies via `setup_fpga_ai.sh`
2. **Model Conversion**: PyTorch → ONNX → Intel DLA format using `convert-pt-2-onnx.py`
3. **Hardware Generation**: `dla_create_ip` generates FPGA IP from neural network
4. **Integration**: Copy generated IP into Quartus project structure

Key environment variables:
- `COREDLA_ROOT`: FPGA AI Suite installation path
- `COREDLA_WORK`: Working directory for AI suite operations

### NIOS V Software Development

Intel Nios V processor uses RISC-V toolchain with specific patterns:
- **BSP Generation**: Board Support Package defines hardware abstraction
- **Toolchain**: `riscv32-unknown-elf-gcc` with march=rv32i flags
- **Build System**: CMake-based with `toolchain.cmake` configuration
- **Debug**: Built-in debug stub support with `ebreak` instruction
- **Memory Map**: System memory layout defined in `system.h` and `memory.gdb`

Build commands:
```bash
niosv-app --bsp_dir=software/bsp --app_dir=software/app -s=software/app/main.c
riscfree -data software  # IDE compilation
niosv-download -g -r -c 1 software/app/build/Debug/app.elf
```

## Critical File Patterns

### Timing Constraints (SDC Files)

All projects require precise timing constraints in `.sdc` files:
- **Clock Definitions**: `create_clock` for input clocks, `create_generated_clock` for PLLs
- **Clock Groups**: `set_clock_groups -asynchronous` for independent clock domains  
- **I/O Constraints**: `set_false_path` for asynchronous signals like reset/buttons
- **JTAG Constraints**: Standard JTAG timing specs using `jtag.sdc` patterns

### Component Integration

VHDL designs use component-based architecture:
- **IP Instantiation**: Quartus IP cores (PLLs, memory, etc.) with `.ip` files
- **Custom Components**: User logic in `.vhd` files with standard entity/architecture
- **System Integration**: Platform Designer (`.qsys`) for complex system assembly

## Platform-Specific Considerations

### DE25-Nano Board Constraints (Target Platform)
- **Device**: Agilex 5 (porting from AXC3000 Agilex 3)
- **Clock**: 50MHz base clock (other speeds obtained using PLLs)
- **I/O**: LED outputs, button inputs with false path constraints
- **Memory**: DDR-free design using on-chip RAM (OCRAM)
- **Migration Notes**: Design patterns from `axc3000/` labs are being adapted for DE25-Nano hardware

### Development Tools Integration
- **Quartus Prime**: Version 25.1+ for synthesis and implementation
- **System Console**: TCL-based hardware debugging and communication
- **SignalTap**: Embedded logic analyzer for runtime debugging
- **Platform Designer**: System-level IP integration

## Common Build Issues

1. **Virtual Environment**: Always source `setup_fpga_ai.sh` before AI lab work
2. **Timing Closure**: Use `dla_adjust_pll.tcl` for automatic PLL frequency adjustment
3. **NIOS V Debugging**: Ensure debug stub is enabled in CPU configuration
4. **IP Generation**: Regenerate IP when moving between Quartus versions

When modifying timing-critical designs, always check `.sdc` files and run timing analysis. For AI acceleration projects, maintain proper environment variable setup and follow the DLA compiler workflow sequence.
