#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
cd "$ROOT/cpp/tests"

if [ -z "${OPENXLSX_PATH:-}" ]; then
  OPENXLSX_PATH="$ROOT/OpenXLSX"
fi

cmake -B build -DOPENXLSX_PATH="$OPENXLSX_PATH"
cmake --build build
cd build
ctest --output-on-failure
