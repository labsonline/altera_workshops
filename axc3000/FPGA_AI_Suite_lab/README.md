# FPGA AI Suite Lab Setup

This directory contains the setup and scripts needed for the FPGA AI Suite Lab.

## Quick Start

1. **Setup Environment**: Run the setup script to activate the Python virtual environment and configure the FPGA AI Suite environment:

   ```bash
   source setup_fpga_ai.sh
   ```

2. **Run the PyTorch to ONNX Conversion**:

   ```bash
   python3 convert-pt-2-onnx.py
   ```

3. **Build the Runtime** (optional):

   ```bash
   cd runtime
   ./build_runtime.sh -target_emulation
   ```

## Files

- `setup_fpga_ai.sh` - Main setup script that:
  - Creates and activates Python virtual environment
  - Installs required packages from requirements.txt
  - Sources OpenVINO and FPGA AI Suite environments (if available)
  - Sets up COREDLA_ROOT and COREDLA_WORK environment variables
  - Sets compiler flags to handle gflags warnings with newer GCC

- `requirements.txt` - Python package dependencies for PyTorch and ONNX conversion

- `convert-pt-2-onnx.py` - Script to convert PyTorch model to ONNX format

- `.venv/` - Python virtual environment with all required packages

## Manual Setup (Alternative)

If you need to set up the environment manually:

1. Create virtual environment:

   ```bash
   python3 -m venv .venv
   source .venv/bin/activate
   ```

2. Install requirements:

   ```bash
   pip install -r requirements.txt
   ```

3. Set environment variables:

   ```bash
   export COREDLA_WORK=$(pwd)
   export COREDLA_ROOT=/opt/altera/25.3pro/fpga_ai_suite/ubuntu/dla
   export CFLAGS="-Wno-error=implicit-fallthrough -Wno-error=cast-function-type"
   export CXXFLAGS="-Wno-error=implicit-fallthrough -Wno-error=cast-function-type"
   ```

## Dependencies

The setup requires:

- Python 3.12+
- PyTorch 2.9.1+
- ONNX and related packages
- OpenCV development libraries
- OpenVINO (optional, for full FPGA AI Suite functionality)
- Intel FPGA AI Suite (optional, for full functionality)

## System Dependencies

Install OpenCV development packages:

```bash
sudo apt update
sudo apt install -y libopencv-dev libopencv-contrib-dev
```

## Troubleshooting

### Common Issues

1. **Missing PyTorch**: If you get `ModuleNotFoundError: No module named 'torch'`, run the setup script which will automatically create the virtual environment and install dependencies.

2. **OpenCV Missing**: Install OpenCV development packages with:

   ```bash
   sudo apt install -y libopencv-dev libopencv-contrib-dev
   ```

3. **Runtime Build Fails**: The build script automatically sets compiler flags to handle gflags warnings. If you still get compilation errors, ensure you've sourced the setup script first.

4. **FPGA AI Suite Not Found**: The script will fall back to manual configuration. PyTorch functionality will still work correctly.

## Environment Variables

The setup script configures these important variables:

- `COREDLA_WORK` - Points to the lab directory
- `COREDLA_ROOT` - Points to FPGA AI Suite installation
- `CFLAGS`/`CXXFLAGS` - Compiler flags for successful builds
