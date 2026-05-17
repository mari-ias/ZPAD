#!/bin/bash
set -e

mkdir -p build

# Автоматично копіюємо моделі туди, звідки запускається програма
cp -r models build/ 2>/dev/null || true

cd build
cmake ..
make -j$(nproc)