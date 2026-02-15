#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"
TESTDATA_DIR="$ROOT_DIR/testdata/sim8086"
SIM8086="$ROOT_DIR/computerenhance"

MODE="${1:-decode}"

case "$MODE" in
    decode)
        SIM_FLAGS="-d"
        EXPECTED_EXT=".asm"
        ;;
    exec)
        SIM_FLAGS=""
        EXPECTED_EXT=".txt"
        ;;
    *)
        echo "Usage: $0 [decode|exec]"
        exit 1
        ;;
esac

PASSED=0
FAILED=0
SKIPPED=0

for bin in "$TESTDATA_DIR"/*; do
    # Skip .asm and .txt files
    if [[ "$bin" == *.asm ]] || [[ "$bin" == *.txt ]]; then
        continue
    fi

    name=$(basename "$bin")
    expected="$bin$EXPECTED_EXT"

    if [[ ! -f "$expected" ]]; then
        echo "SKIP: $name (no $EXPECTED_EXT file)"
        ((SKIPPED++))
        continue
    fi

    actual=$("$SIM8086" sim8086 $SIM_FLAGS -f "$bin" 2>&1) || true

    if diff -q <(echo "$actual") "$expected" > /dev/null 2>&1; then
        echo "PASS: $name"
        ((PASSED++))
    else
        echo "FAIL: $name"
        echo "  Expected:"
        sed 's/^/    /' "$expected"
        echo "  Actual:"
        echo "$actual" | sed 's/^/    /'
        ((FAILED++))
    fi
done

echo ""
echo "Results: $PASSED passed, $FAILED failed, $SKIPPED skipped"

if [[ $FAILED -gt 0 ]]; then
    exit 1
fi
