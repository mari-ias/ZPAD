#!/bin/bash

set -e

echo "[1/4] Updating packages..."
sudo apt update

echo "[2/4] Installing build tools..."
sudo apt install -y build-essential cmake

echo "[3/4] Installing OpenCV..."
sudo apt install -y libopencv-dev

echo "[4/4] Installing camera utils..."
sudo apt install -y v4l-utils x11-apps

echo "Preinstall complete!"