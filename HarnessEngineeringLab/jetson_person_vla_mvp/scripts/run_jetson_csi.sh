#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."

PIPELINE="nvarguscamerasrc ! video/x-raw(memory:NVMM),width=1280,height=720,framerate=30/1 ! nvvidconv flip-method=0 ! video/x-raw,width=1280,height=720,format=BGRx ! videoconvert ! video/x-raw,format=BGR ! appsink drop=true sync=false"

./build/person_vla --source "$PIPELINE" "$@"

