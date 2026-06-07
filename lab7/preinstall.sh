#!/bin/bash

set -e

echo "[1/4] Updating packages..."
sudo apt update

echo "[2/4] Installing build tools..."
sudo apt install -y build-essential cmake

echo "[3/4] Installing OpenCV..."
sudo apt install -y libopencv-dev

echo "[4/4] Installing camera utils..."
sudo apt install -y v4l-utils x11-apps wget

echo "[5/5] Downloading models..."
mkdir -p models
wget -N -P models https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt
wget -N -P models https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel

echo "Preinstall complete! All assets secured."