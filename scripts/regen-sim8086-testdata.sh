#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
TESTDATA_DIR="$ROOT_DIR/testdata/sim8086"
SIM8086="$ROOT_DIR/computerenhance"

MODE="${1:-all}"

for bin in "$TESTDATA_DIR"/*; do
    # Skip .asm and .txt files
    if [[ "$bin" == *.asm ]] || [[ "$bin" == *.txt ]]; then
        continue
    fi

    name=$(basename "$bin")

    if [[ "$MODE" == "decode" ]] || [[ "$MODE" == "all" ]]; then
        echo "Regenerating $name.asm"
        "$SIM8086" sim8086 -d -f "$bin" > "$bin.asm"
    fi

    if [[ "$MODE" == "exec" ]] || [[ "$MODE" == "all" ]]; then
        echo "Regenerating $name.txt"
        "$SIM8086" sim8086 -f "$bin" > "$bin.txt" 2>&1 || true
    fi
done

echo "Done."
