#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."
./build/person_vla --source 0 "$@"

