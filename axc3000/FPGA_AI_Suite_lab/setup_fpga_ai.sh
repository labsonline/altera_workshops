#!/usr/bin/env bash
# FPGA AI Suite Environment Setup Script
# This script properly sets up the environment for Intel FPGA AI Suite and PyTorch lab

echo "Setting up FPGA AI Suite Lab environment..."

export PATH="/opt/altera/25.3pro/syscon/bin${PATH:+:$PATH}"

# Check if virtual environment exists, if not create it and install requirements
if [ ! -d ".venv" ]; then
    echo "Creating Python virtual environment..."
    python3 -m venv .venv
    echo "Installing Python requirements..."
    source .venv/bin/activate
    pip install -r requirements.txt
else
    echo "Activating existing Python virtual environment..."
    source .venv/bin/activate
fi

# Optionally source OpenVINO and FPGA AI Suite if available
if [ -f "/opt/openvino/l_openvino_toolkit_ubuntu24_2024.6.0.17404.4c0f47d2335_x86_64/setupvars.sh" ]; then
    echo "Sourcing OpenVINO 2024.6.0..."
    source /opt/openvino/l_openvino_toolkit_ubuntu24_2024.6.0.17404.4c0f47d2335_x86_64/setupvars.sh
else
    echo "OpenVINO not found at expected path, skipping..."
fi

if [ -f "/opt/altera/25.3pro/fpga_ai_suite/ubuntu/dla/setupvars.sh" ]; then
    echo "Sourcing FPGA AI Suite..."
    source /opt/altera/25.3pro/fpga_ai_suite/ubuntu/dla/setupvars.sh
else
    echo "FPGA AI Suite not found at expected path, skipping..."
fi

# Source the coredla_work script to set COREDLA_WORK (only if FPGA AI Suite is available)
if [ ! -z "$COREDLA_ROOT" ]; then
    echo "Setting up COREDLA_WORK environment..."
    source ./coredla_work.sh
else
    echo "FPGA AI Suite not available, setting COREDLA_WORK manually..."
    export COREDLA_WORK='/home/schubert/Downloads/altera_workshops/axc3000/FPGA_AI_Suite_lab'
fi

echo "Environment setup complete!"
echo "COREDLA_WORK: $COREDLA_WORK"
if [ ! -z "$COREDLA_ROOT" ]; then
    echo "COREDLA_ROOT: $COREDLA_ROOT"
fi
if [ ! -z "$COREDLA_VERSION" ]; then
    echo "COREDLA_VERSION: $COREDLA_VERSION"
fi
echo "Virtual environment activated with PyTorch $(python3 -c 'import torch; print(torch.__version__)' 2>/dev/null || echo 'Not available')"
echo ""
echo "You can now run Python scripts that require PyTorch, such as:"
echo "  python3 convert-pt-2-onnx.py"
echo ""
echo "To exit the virtual environment, run: deactivate"

# Export the variables so they persist
export COREDLA_WORK
export COREDLA_ROOT
export COREDLA_VERSION

# Set compiler flags to handle gflags warnings in newer GCC
export CFLAGS="-Wno-error=implicit-fallthrough -Wno-error=cast-function-type"
export CXXFLAGS="-Wno-error=implicit-fallthrough -Wno-error=cast-function-type"

# If the parent shell is zsh, drop into bash with the environment set up
if [[ "$0" == *"zsh"* ]] || [[ "$(ps -p $PPID -o comm=)" == *"zsh"* ]]; then
    echo "Detected zsh as parent shell, launching bash with the configured environment..."
    bash -l
fi
