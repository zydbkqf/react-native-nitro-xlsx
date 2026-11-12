#!/usr/bin/env bash
# Materialize the `OpenXLSX/` header namespace inside
# thirdparty/OpenXLSX/OpenXLSX/headers.
#
# Why real files instead of a symlink: `headers/OpenXLSX -> .` worked in a git
# checkout, but npm does not put symlinks in a tarball, so consumers installing
# this package from npm got no `headers/OpenXLSX` entry at all and every
# `#include <OpenXLSX/...>` failed to resolve. Upstream's install layout copies
# the headers into `include/OpenXLSX`, so we mirror that inside the vendored
# tree with plain files.
#
# Re-run this after updating thirdparty/OpenXLSX to a newer upstream tag.
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SRC="$ROOT/thirdparty/OpenXLSX/OpenXLSX/headers"
DST="$SRC/OpenXLSX"

if [ ! -d "$SRC" ]; then
  echo "missing headers dir: $SRC" >&2
  exit 1
fi

rm -rf "$DST"
mkdir -p "$DST"

find "$SRC" -type f -name '*.hpp' -not -path "$DST/*" -print0 |
  while IFS= read -r -d '' f; do
    rel="${f#"$SRC"/}"
    mkdir -p "$DST/$(dirname "$rel")"
    cp "$f" "$DST/$rel"
  done

echo "synced $(find "$DST" -type f -name '*.hpp' | wc -l | tr -d ' ') headers into ${DST#"$ROOT"/}"
